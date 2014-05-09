/*
 * ExpandedModel2.cpp
 *
 *  Created on: 17 Oct 2011
 *      Author: s0965328
 */


#include <boost/foreach.hpp>
#include <boost/unordered_map.hpp>
#include <cassert>
#include <iomanip>
#include <list>
#include <limits>

#include "ExpandedModel.h"

#include "../util/util.h"

#include "IndexSet.h"
#include "ModelContext.h"
#include "ColSparseMatrix.h"
#include "Set.h"

#include "../model/ConsComp.h"
#include "../model/SetComp.h"
#include "../model/VarComp.h"
#include "../model/SyntaxNodeIDREF.h"
#include "../model/SyntaxNodeID.h"
#include "../model/AmplModel.h"
#include "BlockDep.h"
#include "BlockCons.h"
#include "BlockObj.h"
#include "BlockHV.h"
#include "BlockLP.h"
#include "Var.h"
#include "VarSingle.h"

#include "../model/ObjComp.h"
#include "../parser/sml.tab.h"

using namespace boost::numeric::ublas;
using namespace boost;
using namespace AutoDiff;
using namespace std;

ExpandedModel* ExpandedModel::root = NULL; //initialize root to NULL

ExpandedModel::ExpandedModel(AmplModel* mod,ModelContext* context)
: model(mod),ctx(context),parent(NULL),numLocalVars(0),numLocalCons(0),name(mod->name),
  blockdep(NULL),cblockFull(NULL),hvblockFull(NULL),blockobj(NULL),y(NULL)
{
	this->name = model->name;
	ctx->em = this;
	LOG("ExpandedModel2 constructor for name["<<this->name<<"]");
}

void ExpandedModel::addModelDummy(string dummyVar, ModelComp* comp, string value)
{
	assert(comp!=NULL);
	pair<boost::unordered_map<string,model_dummy_t>::iterator,bool> ret1;
	model_dummy_t a_model_dummy = std::make_pair<ModelComp*,string>(comp,value);
	ret1 = this->dummyMap.insert(pair<string,model_dummy_t>(dummyVar,a_model_dummy));
	assert(ret1.second);
//	assert(ret1.second && ret2.second);
//	pair<boost::unordered_map<string,string>::iterator,bool> ret1;
//	pair<boost::unordered_map<string,ModelComp*>::iterator,bool> ret2;
//	ret1 = this->dummyValueMap.insert(pair<string,string>(dummyVar,value));
//	ret2 = this->dummySetMap.insert(pair<string,ModelComp*>(dummyVar,comp));
//	assert(ret1.second && ret2.second);
	this->name += "_"+value;
}

ExpandedModel::~ExpandedModel() {
	LOG("ExpandedModel2 destructor called !!!");
	LOG("delete context recurisivly!");
	delete ctx;
	dummyMap.clear();
//	dummySetMap.clear();
//	dummyValueMap.clear();
	for(uint i=0;i<this->children.size();i++)
	{
		delete children.at(i);
	}
	this->children.clear();
}

BlockLP* ExpandedModel::getBlockLP(ExpandedModel* emcol)
{
	BlockLP* rval = NULL;
	boost::unordered_map<ExpandedModel*, BlockLP*>::iterator it = lp_cblockMap.find(emcol);
	if(it!=lp_cblockMap.end())
	{
		rval =  it->second;
	}
	else
	{
		rval = new BlockLP(this,emcol);
		//init context and set, param model comp if necessary for this(emrow) and emcol
		this->recursiveInitContext();
		emcol->recursiveInitContext();
		this->model->calculateModelCompRecursive(this->ctx);
		emcol->model->calculateModelCompRecursive(emcol->ctx);
		emcol->model->calculateLocalVar(emcol->ctx);

		BOOST_FOREACH(ConsComp* con, this->model->con_comps)
		{
			//now build each constraint in autodiff format
			LOG(" -- con [ "<<con->name<<"]");
			unordered_map<int,SyntaxNode*>::iterator it = con->partial.find(emcol->model->level);
			SyntaxNode* attribute = it==con->partial.end()? NULL: it->second;
			SyntaxNode* indexing = con->indexing;
			if(indexing!=NULL)
			{
				LOG(" -- con index["<<indexing->print()<<"]");
				IndexSet* iset = indexing->createIndexSet(this->ctx);
				assert(iset->tuples.size()==1); //so far support only 1-demonsianl set
				iset_tuple& tuple = *(iset->tuples.begin());
				string dummy = tuple.dummyVar;
				Set* set = tuple.set;
				SetComp* setcomp =  setcomp;
				std::vector<string>::iterator itset = set->setValues_data_order.begin();
				for(;itset!=set->setValues_data_order.end();itset++)
				{
					string value = *itset;
					this->ctx->addDummyCompValueMapTemp(dummy,setcomp,value);
					string conName = con->name + "_" + value;
					LOG(" constraint - ["<<conName<<"]");
					//now, build autodiff constraint
					Node* acon = attribute==NULL? NULL: attribute->buildAutoDiffDAG(this,emcol,true);
					rval->cons.push_back(acon);
					this->ctx->removeDummySetValueMapTemp(dummy);
				}
				delete iset;
			}
			else
			{
				LOG(" -- con index is NULL - constraint - ["<<con->name<<"]");
				//now, build autodiff constraint
				Node* acon = attribute==NULL?NULL:attribute->buildAutoDiffDAG(this,emcol,true);
				rval->cons.push_back(acon);
			}
		}
		if(GV(logBlock)){
			ostringstream oss;
			oss<<GV(logdir)<<this->qualifiedName()<<"-"<<emcol->qualifiedName()<<".lpblk";
			ofstream fout(oss.str().c_str());
			rval->logBlock(this,emcol,fout);
		}

		lp_cblockMap.insert(pair<ExpandedModel*,BlockLP*>(emcol,rval));
	}
	return rval;
}

uint ExpandedModel::nz_cons_jacobs_lp(ExpandedModel* emcol)
{
	BlockLP* b = this->getBlockLP(emcol);
	boost::unordered_set<AutoDiff::Node*> vSet;
	emcol->copyVariables(vSet);
	assert(vSet.size()==emcol->numLocalVars);
	uint nz = 0;
	BOOST_FOREACH(AutoDiff::Node* cnode, b->cons)
	{
		if(cnode!=NULL)	nz+=nzGrad(cnode,vSet);
		LOG("nz now "<<nz<<" - cnode"<<cnode);
	}
	LOG("row["<<name<<"] col["<<emcol->name<<"] - nz["<<nz<<"]");
	Stat::numNZJacLPCall++;
	return nz;
}

void ExpandedModel::cons_jacobs_lp(ExpandedModel* emcol,col_compress_matrix& m)
{
	std::vector<AutoDiff::Node*> vnodes;
	emcol->copyVariables(vnodes);
	assert(emcol->numLocalVars == vnodes.size());
//	LOG("vnode size -"<<vnodes.size());
//	m.resize(this->numLocalCons,emcol->numLocalVars,false);

	LOG("row["<<name<<"] col["<<emcol->name<<"]");
	BlockLP* b = this->getBlockLP(emcol);
	int r = 0;
	BOOST_FOREACH(AutoDiff::Node* cnode, b->cons)
	{

		col_compress_matrix_row rgrad(m,r);
		if(cnode!=NULL) grad_reverse(cnode,vnodes,rgrad);
		r++;
		LOG(" "<< (cnode==NULL?"null":visit_tree(cnode)) );
		LOG("grad r["<<r<<"] "<<rgrad);
	}
	assert(r == this->numLocalCons);
	LOG("Full Jacobian: "<<m);
	assert(m.size1()==this->numLocalCons);
	assert(m.size2()==emcol->numLocalVars);

	Stat::numJacLPCall++;
	LOG("end cons_jacobs_lp - emrow["<<this->name<<"] emcol["<<emcol->name<<"]");
}



/** Return the constraints function value declared at this expanded model
 *
 *  @param fvals	double pointer to be set the value
 *         			memory has to be already located for fvals
 */
void ExpandedModel::cons_feval_local(double* fvals)
{
	BlockCons* emb = this->getBlockConsFull();
	uint i = 0;
	assert(emb->cons.size()==this->numLocalCons);
	BOOST_FOREACH(AutoDiff::Node* con, emb->cons)
	{
		assert(con != NULL);
		fvals[i] = (eval_function(con));
		i++;
	}
	assert(i == this->numLocalCons);
	LOG("end cons_feval - this["<<this->name<<"] - fvals size["<<i<<"]");
	Stat::numConsFevalLocalCall++;
	assert(i==this->numLocalCons);
}

/** Return the objective function value declared at this expanded model
 *
 * 	@param oval  - the objective double reference to be assigned to the value
 * 					the oval wll be undefined NaN if this expanded model does not have objective comp
 *	Note that: full objective value will be Sum{m in all expandedmodel of this problem} (m->obj_feval())
 */
double& ExpandedModel::obj_feval(double& oval)
{
	oval = NaN_D;
	BlockObj* ob = this->getBlockObj();
	if(ob->objective!=NULL);
	{
		oval = eval_function(ob->objective);
		LOG(""<<visit_tree(ob->objective));
		assert(!isnan(oval));
	}
	LOG("end obj_feval - this["<<this->name<<"] - oval["<<oval<<"]");
	Stat::numObjFevalCall++;
	return oval;
}

void ExpandedModel::obj_grad(double* vals)
{
	BlockObj* ob = this->getBlockObj();
	if(ob->objective != NULL)
	{
		std::vector<AutoDiff::Node*> vnodes;
		this->copyVariables(vnodes);
		assert(vnodes.size()==this->numLocalVars);
		std::vector<double> grad;
		LOG("objective expression  ---- ");
		LOG("--- "<<visit_tree(ob->objective));
		grad_reverse(ob->objective,vnodes,grad);
		assert(grad.size()==this->numLocalVars);
		for(uint i=0;i<grad.size();i++)
		{
			//set to 0 if variable is not in the objective expression
			vals[i] = isnan(grad[i])?0:grad[i];
		}
	}
	Stat::numGradObjCall++;
	LOG("end obj_grad - emrow["<<this->name<<"] size["<<this->numLocalVars<<"]");
}

/*
 * The Local Interface methods
 *
 * The precondition for calling the Local interface methods is:
 * A call to update the primary variable/decision variable specified in ExpandedModel(s)
 * involved by Local Interface calls. The variables defined in this em's parent and ancester and all
 * of its children and decendents will need to be updated before calling any Local Interface calls.
 * Those em(s) are defined by getParentEM() and getAllEM()
 */


/* ----------------------------------------------------------------------------
ExpandedModel::nz_jacobs_local
---------------------------------------------------------------------------- */
/** Return the number of nonzeros in the Jacobian of a section of the model.
 *
 *  The matrix is defined by the intersection of the local constraints in
 *  this model with the local variables of this or another model.
 *
 *  @param emcol_
 *         The model w.r.t. whose local variables the Jacobian should be
 *         evaluated. This parameter can be NULL, in which case the method
 *         works on the intersection of the local constraints with the local
 *         variables (a "diagonal" block).
 *
 *  @return The number of nonzeros in the given part of the Jacobian.
 */
uint ExpandedModel::nz_cons_jacobs_nlp_local(ExpandedModel *emcol)
{
	assert(this!=emcol || (this->model->level == emcol->model->level && this==emcol)); //the diagonal block
	BlockCons* emb = this->getBlockConsPartial(emcol);

	//create variable map for emcol only
	boost::unordered_set<AutoDiff::Node*> vSet;
	this->copyVariables(vSet);
	assert(vSet.size()==this->numLocalVars);
	uint nz = 0;
	BOOST_FOREACH(AutoDiff::Node* con, emb->cons)
	{
//		LOG("con \n"<<visit_tree(con)<<"");
		nz+=con==NULL?0:nzGrad(con,vSet);
		LOG("nz now "<<nz);
	}

	LOG("end nz_cons_jacobs_local -- this["<<this->name<<"] emcol["<<emcol->name<<"]  - Num of Nonzero["<<nz<<"]");
	Stat::numNZConsJacLocalCall++;
	return nz;
}

/* ----------------------------------------------------------------------------
ExpandedModel::cons_jacobs_distribute
---------------------------------------------------------------------------- */
void ExpandedModel::cons_jacobs_nlp_local(ExpandedModel *emcol, col_compress_matrix& block)
{
	assert(this!=emcol || (this->model->level == emcol->model->level && this==emcol)); //the diagonal block
	assert(blockdep!=NULL); //blockdep has to be initialized in nz_cons_jacobs_local method
	assert(block.size1()==this->numLocalCons && block.size2()== emcol->numLocalVars);
	BlockCons* emb = this->getBlockConsPartial(emcol);

	std::vector<AutoDiff::Node*> vnodes;
	uint colstart  = 0;
	uint currcol = 0;
	BOOST_FOREACH(ExpandedModel* em, emb->block->ems)
	{
		LOG("em["<<em->name<<"]");
		if(em == emcol) {
			colstart = currcol;
		}
		em->copyVariables(vnodes);
		currcol += em->numLocalVars;
		assert(vnodes.size() == currcol);
	}
//	LOG("vnode size -"<<vnodes.size());
	assert(blockdep->getNumDepVars()==vnodes.size());
	col_compress_matrix m(emb->cons.size(),vnodes.size());
	int r = 0;
	BOOST_FOREACH(AutoDiff::Node* con, emb->cons)
	{
		col_compress_matrix_row rgrad(m,r);
		if(con!=NULL);
		{
			LOG("constraint expression  ---- ");
			LOG("--- "<<visit_tree(con));
			AutoDiff::grad_reverse(con,vnodes,rgrad);
		}
		r++;
//		LOG("jacobian now"<<m);
	}
//	LOG("Full Jacobian: "<<m);

	int colrange = emcol->numLocalVars + colstart;
	matrix_range<col_compress_matrix > mr(m,range(0,m.size1()),range(colstart,colrange));
	assert(mr.size2()==emcol->numLocalVars);
	assert(mr.size1()==this->numLocalCons);
	LOG("submatrix -- ["<<this->numLocalCons<<"] x ["<<emcol->numLocalVars<<"]");
	block = mr; //TODO: can possibly reduce a copy by setting ColSparseMatrix pointer directly

	Stat::numConsJacLocalCall++;
	LOG("end cons_jacobs_distribute - emrow["<<this->name<<"] emcol["<<emcol->name<<"] -- nnz["<<block.nnz()<<"]");
}

uint ExpandedModel::nz_lag_hess_nlp_local(ExpandedModel* emcol)
{
	LOG("enter nz_cons_hess_local called -- this["<<this->name<<"] emcol["<<emcol->name<<"]");

	//the nonlinear edges from all constraints in the block
	AutoDiff::EdgeSet edgeSet;
	BlockHV* hvb = this->getBlockHVFull();

	BOOST_FOREACH(AutoDiff::Node* cnode, hvb->blkcons->cons)
	{
		assert(cnode!=NULL);
		LOG("constraint expression  ---- ");
		LOG("-- "<<visit_tree(cnode));
		nonlinearEdges(cnode,edgeSet);
	}
	if(hvb->blkobj->objective!=NULL)
	{//if obj_comp is declared in this expanded model
		assert(model->obj_comp!=NULL);
		LOG("objective expression  ---- ");
		LOG("-- "<<visit_tree(hvb->blkobj->objective));
		nonlinearEdges(hvb->blkobj->objective,edgeSet);
	}
	LOG("nonlinearEdges - now: -- "<<edgeSet.size());

	//create variable map for emcol and emrow
	boost::unordered_set<Node*> colvSet;
	emcol->copyVariables(colvSet);
	boost::unordered_set<Node*> rowvSet;
	this->copyVariables(rowvSet);
	LOG("rowvSet size["<<rowvSet.size()<<"]  -- colvSet size["<<colvSet.size()<<"]");

	uint nz = nzHess(edgeSet,colvSet,rowvSet);
	LOG("nonlinearEdges - removed other edges - -"<<edgeSet.toString());
	LOG("end nz_cons_hess_local -- this["<<this->name<<"] emcol["<<emcol->name<<"]  - Num of Nonzero["<<nz<<"]");
	Stat::numNZConsHessLocalCall++;
	return nz;
}

void ExpandedModel::lag_hess_nlp_local(ExpandedModel* emcol,col_compress_matrix& block)
{
	LOG("enter lag_hess_local - emrow["<<this->name<<"] emcol"<<emcol->name<<"]");
	assert(block.size1() == 0 && block.size2() == 0);
	BlockHV* hvb = this->getBlockHVFull();

	uint colstart = 0;
	uint rowstart = 0;
	uint currvar = 0;
	std::vector<AutoDiff::Node*> vnodes;
	BOOST_FOREACH(ExpandedModel* em, hvb->blkcons->block->ems)
	{
		LOG("em["<<em->name<<"]");
		if(em == emcol) {
			colstart = currvar;
		}
		if(em == this) {
			rowstart = currvar;
		}
		em->copyVariables(vnodes);
		currvar += em->numLocalVars;
		assert(vnodes.size() == currvar);
	}

	col_compress_matrix laghess(vnodes.size(),vnodes.size());
	uint i=0;
	BOOST_FOREACH(AutoDiff::Node* cnode, hvb->blkcons->cons)
	{
		col_compress_matrix hess(vnodes.size(),vnodes.size());
		for(uint c = 0;c<vnodes.size();c++)
		{
			//initialize the weight for computing column c of Hessian m
			//this is required by Autodiff_library
			BOOST_FOREACH(AutoDiff::Node* v, vnodes)
			{
				static_cast<AutoDiff::VNode*>(v)->u = 0;
			}
			static_cast<AutoDiff::VNode*>(vnodes[c])->u = 1; //set weight = 1 for column c varible only, rest are 0

			//calculate the column c for Hessian m
			col_compress_matrix_col chess(hess,c);
			assert(cnode!=NULL);
			hess_reverse(cnode,vnodes,chess);
			c++;
		}
		i++;
		laghess = laghess + y[i]*hess;
	}

	if(hvb->blkobj->objective!=NULL)
	{
		col_compress_matrix hess(vnodes.size(),vnodes.size());
		for(uint c = 0;c<vnodes.size();c++)
		{
			//initialize the weight for computing column c of Hessian m
			//this is required by Autodiff_library
			BOOST_FOREACH(AutoDiff::Node* v, vnodes)
			{
				static_cast<AutoDiff::VNode*>(v)->u = 0;
			}
			static_cast<AutoDiff::VNode*>(vnodes[c])->u = 1; //set weight = 1 for column c varible only, rest are 0

			//calculate the column c for Hessian m
			col_compress_matrix_col chess(hess,c);
			hess_reverse(hvb->blkobj->objective,vnodes,chess);
			c++;
		}
		laghess = laghess + hess;
	}

//	LOG("Full hessian: "<<fullhess);
	uint colrange = colstart + emcol->numLocalVars;
	uint rowrange = rowstart + this->numLocalVars;
	col_compress_matrix_range mr(laghess,range(rowstart,rowrange),range(colstart,colrange));
	block = mr;
	assert(mr.size1()==this->numLocalVars);
	assert(mr.size2()==emcol->numLocalVars);
	LOG("submatrix -- ["<<this->numLocalVars<<"] x ["<<emcol->numLocalVars<<"]");
	block = mr;

	Stat::numConsHessLocalCall++;
	LOG("end lag_hess_local - emrow["<<this->name<<"] emcol"<<emcol->name<<"]");
}


BlockHV* ExpandedModel::getBlockHVFull()
{
	LOG("enter - getBlockHVFull - this em["<<this->name<<"]");
	if(hvblockFull != NULL){
		LOG("getBlockHVFull -- already in initialized ");
	}
	else
	{
		BlockCons* cblock = this->getBlockConsFull();
		BlockObj* oblock = this->getBlockObj();
		hvblockFull = new BlockHV(cblock,oblock);

		if(GV(logBlock)){
			ostringstream oss;
			oss<<GV(logdir)<<this->qualifiedName()<<".fullhvblk";
			ofstream fout(oss.str().c_str());
			hvblockFull->logBlock(this,NULL,fout);
		}
	}
	assert(hvblockFull!=NULL);
	LOG("exit - getBlockHVFull - row["<<this->name<<"]");
	return hvblockFull;
}



//createConsBlockLocal
//emcol is passed for the purpose of sum{i in COMM} hack only
//eg. when this is declares the parent level of emcol, and the sum index can be the same of the model index of emcol.
//if this is the case, there is no need to construct the full summation computation graph, instead only the dummy index value
//of the emcol will be used for creating the computation graph.
BlockCons* ExpandedModel::getBlockConsPartial(ExpandedModel* emcol)
{
	assert(this!=emcol || (this->model->level == emcol->model->level && this==emcol)); //the diagonal block
	boost::unordered_map<ExpandedModel*,BlockCons* >::iterator it=nlp_cblockMap_lo.find(emcol);
	BlockCons* emb = NULL;
	if(it!=nlp_cblockMap_lo.end()){
		LOG("createConsBlockLocal -- already in Map");
		emb = (*it).second;
	}
	else{
		BlockDep* b = this->getBlockDep();
		emb = new BlockCons(b);
		BOOST_FOREACH(ConsComp* con, this->model->con_comps)
		{  //now build each constraint in autodiff format
			LOG(" -- con [ "<<con->name<<"]");
			unordered_map<int,SyntaxNode*>::iterator itt = con->partial.find(emcol->model->level);
			SyntaxNode* attribute = itt ==con->partial.end()?NULL:itt->second;
			SyntaxNode* indexing = con->indexing;
			if(indexing!=NULL)
			{
				LOG(" -- con index["<<indexing->print()<<"]");
				IndexSet* iset = indexing->createIndexSet(ctx);
				assert(iset->tuples.size()==1); //TODO: so far support only 1-demonsianl set
				iset_tuple& tuple = *(iset->tuples.begin());
				string dummy = tuple.dummyVar;
				Set* set = tuple.set;
				SetComp* setcomp = tuple.setcomp;
				std::vector<string>::iterator itset = set->setValues_data_order.begin();
				for(;itset!=set->setValues_data_order.end();itset++)
				{
					string value = *itset;
					this->ctx->addDummyCompValueMapTemp(dummy,setcomp,value);
					string conName = con->name + "_" + value;
					LOG(" constraint - ["<<conName<<"]");
					//now, build autodiff constraint using the partial attributes
					Node* acon = attribute==NULL?NULL:attribute->buildAutoDiffDAG(this,emcol);
					emb->cons.push_back(acon);
					this->ctx->removeDummySetValueMapTemp(dummy);
				}
				delete iset;
			}
			else
			{
				LOG(" -- con index is NULL");
				string consName = con->name + "_";
				LOG(" constraint - ["<<consName<<"]");
				//now, build autodiff constraint
				Node* acon = attribute==NULL?NULL:attribute->buildAutoDiffDAG(this,emcol);
				emb->cons.push_back(acon);
			}
		}

		if(GV(logBlock)){
			ostringstream oss;
			oss<<GV(logdir)<<this->qualifiedName()<<"-"<<emcol->qualifiedName()<<".lconsblk";
			ofstream fout(oss.str().c_str());
			emb->logBlock(this,emcol,fout);
		}
		nlp_cblockMap_lo.insert(pair<ExpandedModel*,BlockCons*>(emcol,emb));
	}
	LOG("exit - getBlockConsPartial - row["<<this->name<<"] X col["<<emcol->name<<"] -- ncon["<<emb->cons.size()<<"] nvar["<<emcol->numLocalVars<<"]");
	return emb;
}

BlockCons* ExpandedModel::getBlockConsFull()
{
	if(this->cblockFull != NULL){
		LOG("getBlockConsFull -- already initialised");
		return cblockFull;
	}

	BlockDep* b = this->getBlockDep();
	cblockFull = new BlockCons(b);
	BOOST_FOREACH(ConsComp* con, this->model->con_comps)
	{  //now build each constraint in autodiff format
		LOG(" -- con [ "<<con->name<<"]");
		SyntaxNode* attribute = con->attributes;
		SyntaxNode* indexing = con->indexing;
		if(indexing!=NULL)
		{
			LOG(" -- con index["<<indexing->print()<<"]");
			IndexSet* iset = indexing->createIndexSet(ctx);
			assert(iset->tuples.size()==1); //TODO: so far support only 1-demonsianl set
			iset_tuple& tuple = *(iset->tuples.begin());
			string dummy = tuple.dummyVar;
			Set* set = tuple.set;
			SetComp* setcomp = tuple.setcomp;
			std::vector<string>::iterator itset = set->setValues_data_order.begin();
			for(;itset!=set->setValues_data_order.end();itset++)
			{
				string value = *itset;
				this->ctx->addDummyCompValueMapTemp(dummy,setcomp,value);
				LOG(" constraint - ["<<con->name <<"_"<<value<<"]");
				//now, build autodiff constraint using full attributes
				SyntaxNode* assgin_expr = attribute->findDirectChild(ASSIGN);
				Node* acon = assgin_expr->buildAutoDiffDAG(this);
				assert(acon!=NULL);
				cblockFull->cons.push_back(acon);
				this->ctx->removeDummySetValueMapTemp(dummy);
			}
			delete iset;
		}
		else
		{
			LOG(" -- con index is NULL");
			string conName = con->name + "_";
			LOG(" constraint - ["<<conName<<"]");
			//now, build autodiff constraint
			SyntaxNode* assgin_expr = attribute->findDirectChild(ASSIGN);
			Node* acon = assgin_expr->buildAutoDiffDAG(this);
			assert(acon!=NULL);
			cblockFull->cons.push_back(acon);
		}
	}

	if(GV(logBlock)){
		ostringstream oss;
		oss<<GV(logdir)<<this->qualifiedName()<<".fullconsblk";
		ofstream fout(oss.str().c_str());
		cblockFull->logBlock(this,NULL,fout);
	}

	LOG("exit - getBlockConsFull - row["<<this->name<<"] -- ncon["<<this->cblockFull->cons.size()<<"]");
	assert(this->cblockFull!=NULL && this->cblockFull->cons.size()==this->numLocalCons);
	return this->cblockFull;
}

//! return the pointer to block objective
//! the objective autodiff node is created if this expanded model has an objective comp delcared.
//! otherwise the node is set to NULL
BlockObj* ExpandedModel::getBlockObj()
{
	ObjComp* objcomp = this->model->obj_comp;
	string objname = objcomp==NULL?"null":objcomp->name;

	if(blockobj!=NULL)
	{
		LOG("getBlockObjPartial -- already in Map");
	}
	else
	{
		blockobj = new BlockObj(this);
		this->recursiveInitContext();
		this->model->calculateModelCompRecursive(this->ctx);
		this->model->calculateLocalVar(this->ctx);
		if(objcomp!=NULL)
		{//compute objective if objcomp != NULL
			assert(objcomp->indexing == NULL); //TODO: no multi-objective supported yet!

			LOG(" -- objcomp [ "<<objname<<"]");
			assert(objcomp->attributes!=NULL);
			//now, build autodiff constraint node for object constraint
			Node* obj = objcomp->attributes->buildAutoDiffDAG(this);
			blockobj->objective = obj;
		}

		if(GV(logBlock)){
			ostringstream oss;
			oss<<GV(logdir)<<this->qualifiedName()<<".objblk";
			ofstream fout(oss.str().c_str());
			blockobj->logBlock(this,fout);
		}
	}
	LOG("exit - getBlockObjPartial - row["<<this->name<<"] - nvar["<<this->numLocalVars<<"] --- with OBJ["<<objname<<"]");
	return blockobj;
}

BlockDep* ExpandedModel::getBlockDep()
{
	if(blockdep == NULL)
	{
		blockdep = new BlockDep();
		std::vector<ExpandedModel*>& ems = blockdep->ems;
		//the constraints declared in this, can reference variables declared in all it's descendants and ascendants
		this->getParentEM(ems);
		this->getAllEM(ems);

		for(std::vector<ExpandedModel*>::iterator it = ems.begin();it != ems.end();it++) {
			(*it)->recursiveInitContext();
		}
		for(std::vector<ExpandedModel*>::iterator it = ems.begin();it != ems.end();it++) {
			(*it)->model->calculateModelCompRecursive((*it)->ctx);
		}
		for(std::vector<ExpandedModel*>::iterator it = ems.begin();it != ems.end();it++) {
			(*it)->model->calculateLocalVar((*it)->ctx);
		}
	}
	else
	{
		LOG("getBlockLocal - already initialized");
	}
	LOG("getBlockLocal - em["<<this->name<<"] ");
	assert(blockdep!=NULL);
	return blockdep;
}
/*
 * End of Local Interface methods
 */


/*
 * For both Local and Distribute Interface
 * Access information of this expaned model
 *
 */
void ExpandedModel::get_vars_names(std::vector<string>& names)
{
	std::vector<VarComp*>::iterator ivar=this->model->var_comps.begin();
	for(;ivar!=model->var_comps.end();ivar++)
	{
		Var* var = static_cast<Var*>(ctx->getCompValue(*ivar));
		var_multi_map_by_order& var_by_order = var->varMultiMap.get<0>();
		var_multi_map_by_order::iterator i = var_by_order.begin();
		for(;i!=var_by_order.end();i++)
		{
			ostringstream oss;
			oss<<this->qualifiedName()<<"-"<<var->name<<"_"<<(*i)->indicies;
			names.push_back(oss.str());
		}
	}
}


/* -------------------------------------------------------------------------
ExpandedModel::getColUpBounds
-------------------------------------------------------------------------- */
/** Return the upper bounds for the local variables defined in this model.
 *
 *  @param[out] elts
 *              The variable upper bounds.
 */
void ExpandedModel::get_var_up_bounds(double *elts)
{
	int i = 0;
	std::vector<VarComp*>::iterator ivar=this->model->var_comps.begin();
	for(;ivar!=model->var_comps.end();ivar++)
	{
		Var* var = static_cast<Var*>(ctx->getCompValue(*ivar));
		assert(var!=NULL);
		var_multi_map_by_order& var_by_order = var->varMultiMap.get<0>();
		var_multi_map_by_order::iterator iv = var_by_order.begin();
		for(;iv!=var_by_order.end();iv++)
		{
			VarSingle* varsingle = *iv;
			elts[i] = varsingle->ub;
			i++;
		}
	}
}

/* -------------------------------------------------------------------------
ExpandedModel::getColLowBounds
-------------------------------------------------------------------------- */
/** Return the lower bounds for the local variables defined in this model.
 *
 *  @param[out] elts
 *              The variable lower bounds.
 */
void ExpandedModel::get_var_low_bounds(double *elts)
{
	int i = 0;
	std::vector<VarComp*>::iterator ivar=this->model->var_comps.begin();
	for(;ivar!=model->var_comps.end();ivar++)
	{
		Var* var = static_cast<Var*>(ctx->getCompValue(*ivar));
		assert(var!=NULL);
		var_multi_map_by_order& var_by_order = var->varMultiMap.get<0>();
		var_multi_map_by_order::iterator iv = var_by_order.begin();
		for(;iv!=var_by_order.end();iv++)
		{
			VarSingle* varsingle = *iv;
			elts[i] = varsingle->lb;
			i++;
		}
	}
}


/* -------------------------------------------------------------------------
ExpandedModel::getRowBounds
-------------------------------------------------------------------------- */
/** Return the upper and lower bounds for the constraints in this model.
 *
 *  @param[out] lower
 *              The lower bounds on the constraints.
 *  @param[out] upper
 *              The upper bounds on the constraints.
 *
 *  TODO: purhapse to record the constraint bounds information in a Con , SinglCon classes.
 *  Just like how we stores variables as Var, SingleVar class.
 */
void ExpandedModel::get_cons_bounds(double *lower, double *upper)
{
	LOG("getRowBounds -- ["<<this->name<<"]");
	uint i = 0;
	BOOST_FOREACH(ConsComp* con, this->model->con_comps)
	{
		//can be retrieved from context
		SyntaxNode* indexing = con->indexing;
		if(indexing!=NULL)
		{
			IndexSet* iset = indexing->createIndexSet(ctx);
			assert(iset->tuples.size()==1); //TODO: so far support only 1-demonsianl set
			iset_tuple& tuple = *(iset->tuples.begin());
			string dummy = tuple.dummyVar;
			Set* set = tuple.set;
			SetComp* setcomp = tuple.setcomp;
			BOOST_FOREACH(string& val, set->setValues_data_order)
			{
				this->ctx->addDummyCompValueMapTemp(dummy,setcomp,val);
				con->attributes->calculateConsBounds(ctx,lower[i],upper[i]);
				this->ctx->removeDummySetValueMapTemp(dummy);
				i++;
			}
			delete iset;
		}
		else
		{
			con->attributes->calculateConsBounds(ctx,lower[i],upper[i]);
			i++;
		}
	}
}

void ExpandedModel::get_cons_names(std::vector<string>& names)
{
//	assert(this->ctx->isCompsCalculated == true);
	std::vector<ConsComp*>::iterator it = this->model->con_comps.begin();
	for(;it!=this->model->con_comps.end();it++)
	{
		ConsComp* con = *it;
		SyntaxNode* indexing = con->indexing;
		if(indexing!=NULL)
		{
			IndexSet* iset = indexing->createIndexSet(ctx);
			assert(iset->tuples.size()==1); //TODO: so far support only 1-demonsianl set
			iset_tuple& tuple = *(iset->tuples.begin());
			string dummy = tuple.dummyVar;
			Set* set = tuple.set;
//			SetComp* setcomp = tuple.setcomp;
			std::vector<string>::iterator itset = set->setValues_data_order.begin();
			for(;itset!=set->setValues_data_order.end();itset++)
			{
				ostringstream oss;
				oss<<this->qualifiedName()<<"-"<<con->name<<"_"<<*itset;
				names.push_back(oss.str());
			}
			delete iset;
		}
		else
		{
			string conName = this->name+"-"+con->name + "_";
			names.push_back(conName);
		}
	}
}

///* -------------------------------------------------------------------------
//ExpandedModel::update_primal_var_soln
//-------------------------------------------------------------------------- */
void ExpandedModel::update_primal_x(double *elts)
{
	LOG("update_primal_var_soln() -- ["<<this->name<<"] numOfLocalVar["<<numLocalVars<<"]");
	uint index = 0;
	BOOST_FOREACH(VarComp* vc, this->model->var_comps)
	{
		Var* var = static_cast<Var*>(this->ctx->getCompValue(vc));
		assert(var!=NULL);
		var_multi_map_by_order& var_by_order = var->varMultiMap.get<0>();
		var_multi_map_by_order::iterator i = var_by_order.begin();
		for(;i!=var_by_order.end();i++)
		{
			VarSingle* varsingle = *i;
			static_cast<AutoDiff::VNode*>(varsingle->adv)->val = elts[index];
			index++;
			LOG("update_primal_var_soln - ["<<varsingle->toString()<<"]");
		}
	}
	assert(index==this->numLocalVars);
}

void ExpandedModel::update_lag(double* elts)
{
	LOG("update_lag() -- ["<<this->name<<"] numOfLocalVar["<<numLocalVars<<"]");
	for(uint i = 0;i<this->numLocalCons;i++)
	{
		y[i] = elts[i];
	}
}


/*
 * End of common interface methods for both Local and Distribute
 * **********************************************************************************************
 */



/* ***********************************************************************************************
 *  Helper Methods Section
 */
void ExpandedModel::addChildren(ExpandedModel* em2)
{
	this->children.push_back(em2);
	em2->parent = this;
	LOG("addChildren  - parent["<<this->name<<"] child["<<em2->name<<"] -- num_child["<<this->children.size()<<"]");
}

void ExpandedModel::clearAllContextTreeKeepRoot()
{
	if(this!=ExpandedModel::root)
	{
		delete this->ctx;
		this->ctx = NULL;
	}

	for(std::vector<ExpandedModel*>::iterator it=this->children.begin();it!=this->children.end();it++)
	{
		(*it)->clearAllContextTreeKeepRoot();
	}
}


ModelContext* ExpandedModel::recursiveInitContext()
{
	LOG("recursiveInitContext for ["<<this->name<<"]");
	if(this->ctx == NULL){
		ExpandedModel* parentEm2 = this->parent;
		ModelContext* parentCtx = parentEm2->ctx;
		parentCtx = parentEm2->recursiveInitContext();
		this->ctx = new ModelContext(parentCtx);
		this->ctx->em = this;
	}
//	LOG("recursiveInitContext for ["<<this->name<<"] - contextId["<<this->ctx->getContextId()<<"]");

	return this->ctx;
}

void ExpandedModel::logEMRecursive(string& line, ostream& out)
{
	string header;
	if(this==ExpandedModel::root)
	{
		header = line;
	}
	else
	{
		header = line+"|--";
	}
	out<<header<<this->name<<"["<<this->numLocalCons<<" X "<<this->numLocalVars<<"]"<<endl;

	if(ctx!=NULL){
		std::vector<string> nameCons;
		this->get_cons_names(nameCons);
		for(std::vector<string>::iterator it = nameCons.begin();it!=nameCons.end();it++){
			out<<line<<"\t constraint:"<<(*it)<<endl;
		}


		std::vector<string> nameVars;
		this->get_vars_names(nameVars);
		for(std::vector<string>::iterator it = nameVars.begin();it!=nameVars.end();it++){
			out<<line<<"\t variable:"<<(*it)<<endl;
		}

		string pre = line;
		line += "\t";
		for(std::vector<ExpandedModel*>::iterator it=this->children.begin();it!=this->children.end();it++)
		{
			(*it)->logEMRecursive(line,out);
		}
		line = pre;
	}
	else{
		out<<"[This EM's Context is not Initialized Locally]"<<endl;
	}
}

void ExpandedModel::getAllEM(std::vector<ExpandedModel*>& ems)
{
	LOG("getAllEM -- "<<this->name);
	ems.push_back(this);

	for(std::vector<ExpandedModel*>::iterator it=this->children.begin();it!=this->children.end();it++)
	{
		(*it)->getAllEM(ems);
	}
}

void ExpandedModel::getParentEM(std::vector<ExpandedModel*>& ems)
{
	if(this->parent!=NULL){
		LOG("getParentEM -- "<<this->parent->name);
		this->parent->getParentEM(ems);
		ems.push_back(this->parent);
	}
}

void ExpandedModel::convertToColSparseMatrix(col_compress_matrix& m,ColSparseMatrix* sm)
{
	LOG("convertToColSparseMatrix - "<<m<<" nz"<<m.nnz());
	assert(sm!=NULL);
	if(m.nnz()!=0){
		uint index =0;
		uint prev = 0;
		BOOST_FOREACH(uint i, m.index1_data())
		{
			sm->colstarts[index] = i>prev && i<=m.nnz()? i:prev;
			LOG(" "<<i<<" -> "<<sm->colstarts[index]);
			prev = sm->colstarts[index];
			index ++;
		}
		index =0;
		BOOST_FOREACH(uint i, m.index2_data())
		{
			LOG(" "<<i);
			sm->rownos[index] = i;
			index++;
			if(index==m.nnz()) break;
		}
		index=0;
		BOOST_FOREACH(double i, m.value_data())
		{
			LOG(" "<<i);
			sm->values[index] = i;
			index++;
			if(index==m.nnz()) break;
		}
		if(sm->collen!=NULL)
		{
			for(int j=0;j<m.size2();j++)
			{
				sm->collen[j] = sm->colstarts[j+1] - sm->colstarts[j];;
			}
		}
	}
}


void ExpandedModel::copyVariables(boost::unordered_set<AutoDiff::Node*>& vSet)
{
	BOOST_FOREACH(VarComp* vc, this->model->var_comps)
	{
		Var* var = static_cast<Var*>(this->ctx->getCompValue(vc));
		assert(var!=NULL);
		var_multi_map_by_order& var_by_order = var->varMultiMap.get<0>();
		var_multi_map_by_order::iterator i = var_by_order.begin();
		for(;i!=var_by_order.end();i++)
		{
			vSet.insert((*i)->adv);
//			LOG("copyVariables - ["<<v.toString()<<"]");
		}
	}
}

void ExpandedModel::copyVariables(std::vector<AutoDiff::Node*>& vSet)
{
	BOOST_FOREACH(VarComp* vc, this->model->var_comps)
	{
		Var* var = static_cast<Var*>(this->ctx->getCompValue(vc));
		assert(var!=NULL);
		var_multi_map_by_order& var_by_order = var->varMultiMap.get<0>();
		var_multi_map_by_order::iterator i = var_by_order.begin();
		for(;i!=var_by_order.end();i++)
		{
			vSet.push_back((*i)->adv);
			LOG("copyVariables - ["<<(*i)->adv->toString(0)<<"]");
		}
	}
}

string ExpandedModel::qualifiedName()
{
	ostringstream oss;
	if(parent!=NULL){
		oss<<parent->qualifiedName();
		oss<<"_";
	}
	oss<<this->name;
	return oss.str();
}

/*
 * Finding the sub-context from it's children problem, so that
 * the dummy variables value equals to dummyval
 * Linear case: the ctx returned can be null, for LP problem only context initialised is the ancestor of emrow or emcol.
 */
ModelContext* ExpandedModel::locateCtx(AmplModel* model, string& dummyval)
{
	ModelContext* rval = NULL;
	BOOST_FOREACH(ExpandedModel* em, this->children)
	{
		assert(em->dummyMap.size()==1); //only 1 model dummy supported for now.
		if(em->model == model)
		{
//			string dumval = em->dummyValueMap.begin()->second;
			string dumval = em->dummyMap.begin()->second.second;
			if(dumval.compare(dummyval)==0) {
				rval = em->ctx;
				break;
			}
		}
	}
	return rval;
}
/*
 * End Helper Methods Section
 * ******************************************************************************************
 */

 /*
  * Statistics
  *
  *  calculateMemoryUsage
 */
void ExpandedModel::calculateMemoryUsage(unsigned long& size_str,unsigned long& size_data)
{
	LOG_MEM("ExpandedModel2::calculateMemoryUsage  -- name["<<indicies<<"]");
	unsigned long pre = size_data;
	if(ctx!=NULL)
	{
		ctx->calculateMemoryUsage(size_data);
		LOG_MEM(" --- context memory usage ["<<size_data-pre<<"]");
	}


	for(boost::unordered_map<string,model_dummy_t>::iterator it=dummyMap.begin();it!=dummyMap.end();it++)
	{
		size_str += sizeof(pair<string,model_dummy_t>);
		size_str += (*it).first.size() + 1;
		size_str += (*it).second.second.size() +1;
	}

//	for(boost::unordered_map<string,ModelComp*>::iterator it=dummySetMap.begin();it!=dummySetMap.end();it++)
//	{
//		size_str += sizeof(pair<string,ModelComp*>);
//		size_str += (*it).first.size() + 1;
//	}
//	for(boost::unordered_map<string,string>::iterator it=dummyValueMap.begin();it!=dummyValueMap.end();it++)
//	{
//		size_str += sizeof(pair<string,string>);
//		size_str += (*it).first.size() + 1;
//		size_str += (*it).second.size()+ 1;
//	}

	size_str += sizeof(ExpandedModel);
	size_str += name.size() + 1;
	pre = size_str;
	LOG_MEM(" --- solution std::vector usage ["<<size_str-pre<<"]");

	pre = size_str;
	LOG_MEM(" --- holder usage ["<<size_str-pre<<"]");

	for(std::vector<ExpandedModel*>::iterator it=children.begin();it!=children.end();it++)
	{
		size_str += sizeof(ExpandedModel*);
		(*it)->calculateMemoryUsage(size_str,size_data);
	}
}


//
//void ExpandedModel::convertToColSparseMatrix(compress_matrix& m,ColSparseMatrix* sm)
//{
////	LOG("convertToColSparseMatrix - "<<m);
////	assert(sm!=NULL);
////	uint index =0;
////	BOOST_FOREACH(uint i, m.index1_data())
////	{
////		LOG(" "<<i);
////		sm->rownos[index] = i;
////		index ++;
////	}
////	index =0;
////	BOOST_FOREACH(uint i, m.index2_data())
////	{
////		LOG(" "<<i);
////		sm->colstarts[index] = i;
////		index++;
////	}
////	index=0;
////	BOOST_FOREACH(double i, m.value_data())
////	{
////		LOG(" "<<i);
////		sm->values[index] = i;
////		index++;
////	}
//
//	boost::numeric::ublas::compressed_matrix<double>::iterator1 it1;
//	boost::numeric::ublas::compressed_matrix<double>::iterator2 it2;
//	uint i = 0;
//	int prev_col = -1;
//	int curr_col = 0;
//	for(it2=m.begin2();it2!=m.end2();it2++)
//	{
//		for(it1=it2.begin();it1!=it2.end();it1++)
//		{
//			LOG(it1.index1()<<","<<it2.index2()<<"="<<*it1);
//			curr_col = it2.index2();
//			sm->rownos[i] = it1.index1();
//			sm->values[i] = *it1;
//			if(prev_col==-1) //for leading zero columns
//			{
//				for(prev_col=0;prev_col<=curr_col;prev_col++)
//				{
//					sm->colstarts[prev_col] = 0;
//					LOG("---- sm->colstarts[prev_col] "<<prev_col<<" - "<<sm->colstarts[prev_col]);
//				}
//			}
//			else
//			{
//				if(prev_col != curr_col)
//				{
//					for(prev_col=prev_col+1;prev_col<=curr_col;prev_col++) //for intermediate zero columns
//					{
//						sm->colstarts[prev_col] = i;
//						LOG("sm->colstarts[prev_col] "<<prev_col<<" - "<<sm->colstarts[prev_col]);
//					}
//				}
//			}
//			prev_col = curr_col;
//			i++;
//		}
//	}
//
//	for(curr_col=curr_col+1;curr_col<=m.size2();curr_col++) //for ending zero culumns
//	{
//		sm->colstarts[curr_col] = i;
//	}
//	assert(curr_col-1 == m.size2());
//
//	for(int j=0;j<i;j++)
//	{
//		LOG("rowno ["<<sm->rownos[j] <<" value["<<sm->values[j]);
//	}
//	for(int j=0;j<m.size2()+1;j++){
//		LOG("colstarts ["<<sm->colstarts[j]);
//	}
//
//	if(sm->collen!=NULL)
//	{
//		for(int j=0;j<m.size2();j++)
//		{
//			sm->collen[j] = sm->colstarts[j+1] - sm->colstarts[j];
//		}
//	}
//
//	for(int j=0;j<m.size2();j++){
//		LOG("collen ["<<sm->collen[j]);
//	}
//
//}


//
///* -------------------------------------------------------------------------
//ExpandedModel::setPrimalSolRows
//-------------------------------------------------------------------------- */
///** Upload the local constraints slacks */
//void ExpandedModel::setPrimalSolRows(const double *elts)
//{
//	LOG("setPrimalSolRows -- ["<<this->name<<"] numLocalCons["<<numLocalCons<<"]");
//
//	for (int i = 0; i < numLocalCons; ++i)
//	{
//		prow.push_back(elts[i]);
//	}
//}
//
///* -------------------------------------------------------------------------
//ExpandedModel::setDualSolColumns
//-------------------------------------------------------------------------- */
///** Upload the local variable duals (multipliers on bounds) */
//void ExpandedModel::setDualSolColumns(const double *elts)
//{
//	LOG("setDualSolColumns -- ["<<this->name<<"] numLocalVars["<<numLocalVars<<"]");
//	for (int i = 0; i < numLocalVars; ++i)
//	{
//		dvar.push_back(elts[i]);
//	}
//}
//
///* -------------------------------------------------------------------------
//ExpandedModel::setDualSolRows
//-------------------------------------------------------------------------- */
///** Upload the local constraints duals (multipliers on constraints) */
//void ExpandedModel::setDualSolRows(const double *elts)
//{
//	LOG("setDualSolRows -- ["<<this->name<<"] numLocalCons["<<numLocalCons<<"]");
//
//	for (int i = 0; i < numLocalCons; ++i)
//	{
//		drow.push_back(elts[i]);
//	}
//}
//
///* -------------------------------------------------------------------------
//ExpandedModel::outputSolution
//-------------------------------------------------------------------------- */
///** Output the solution to the supplied stream with the given indent */
//void ExpandedModel::outputSolution(ostream &out,int indent)
//{
//	LOG("outputSolution --- ["<<this->name<<"] numOfChildren["<<this->children.size()<<"]");
//	assert(this->ctx->localConNames.size() == this->numLocalCons);
//	assert(this->ctx->localVarNames.size() == this->numLocalVars);
//
//	string ind(indent, ' ');
//	vector<double>::iterator it_pvar=pvar.begin();
//	vector<double>::iterator it_dvar=dvar.begin();
//	vector<double>::iterator it_prow=prow.begin();
//	vector<double>::iterator it_drow=drow.begin();
//
//	list<string>::iterator it_varName =this->ctx->localVarNames.begin();
//	list<string>::iterator it_conName =this->ctx->localConNames.begin();
//
//	while(it_varName!=this->ctx->localVarNames.end())
//	{
//		out<<ind<<left<<setw(20)<<*it_varName<<" Value= "<<setw(15)<<*it_pvar;
//		out<<" Reduced cost= "<<*it_dvar;
//		out<<endl;
//
//		it_pvar++;
//		it_dvar++;
//		it_varName++;
//	}
//
//	while(it_conName!=this->ctx->localConNames.end())
//	{
//		out<<ind<<left<<setw(20)<<*it_conName<<" Slack= "<<setw(15)<<*it_prow;
//		out<<" Dual= "<<*it_drow;
//		out<<endl;
//
//		it_prow++;
//		it_drow++;
//		it_conName++;
//	}
//
//	for(vector<ExpandedModel*>::iterator i=children.begin();i!=children.end();i++)
//	{
//		(*i)->outputSolution(out, indent+2);
//	}
//}


//
//
///* * ******************************************************************************************
// * Distribute interface implemenation
// * 		createConsBlockDistributed
// *
// * 		cons_feval_distribute
// * 		cons_nz_jacobs_distribute
// * 		cons_nz_hessian_distribute
// * 		cons_jacobs_distribute
// * 		cons_hessian_distribute
// * 		obj_grad_distribute
// * 		obj_nz_hessian_distribute
// * 		obj_hessian_distribute
// *
//*/
//
//Block* ExpandedModel::getBlockDistributed(ExpandedModel* emcol)
//{
//	LOG("getBlockDistributed - this["<<this->name<<"] - emcol["<<emcol->name<<"]");
//	Block* block = NULL;
//	boost::unordered_map<ExpandedModel*,Block*>::iterator it = this->block_ds.find(emcol);
//	if(it == this->block_ds.end())
//	{
//		block = new Block();
//		//involve emcol's parent and ancestors
//		emcol->getParentEM(block->ems);
//		//involve emcol itself
//		block->ems.push_back(emcol);
//
//		std::vector<ExpandedModel*>& ems = block->ems;
//		for(std::vector<ExpandedModel*>::iterator it = ems.begin();it != ems.end();it++) {
//			(*it)->recursiveInitContext();
//		}
//		for(std::vector<ExpandedModel*>::iterator it = ems.begin();it != ems.end();it++) {
//			(*it)->model->calculateModelCompRecursive((*it)->ctx);
//		}
//		for(std::vector<ExpandedModel*>::iterator it = ems.begin();it != ems.end();it++) {
//			(*it)->model->calculateLocalVar((*it)->ctx);
//		}
//		this->block_ds.insert(pair<ExpandedModel*,Block*>(emcol,block));
//	}
//	else
//	{
//		LOG("getBlockDistributed -- already initilized ");
//		block = it->second;
//	}
//	return block;
//}
//
//BlockCons* ExpandedModel::createConsBlockDistributed(ExpandedModel* emcol)
//{
//	LOG("createEMBlockDistributed - row["<<this->name<<"] col["<<emcol->name<<"]");
//	Block* block_ds = this->getBlockDistributed(emcol);
//	BlockCons* emb = new BlockCons(block_ds);
//
//	int col_level = emcol->model->level;
//	std::vector<ConsComp*>::iterator it = this->model->con_comps.begin();
//	for(;it!=this->model->con_comps.end();it++)
//	{  //now build each constraint in autodiff format
//		ConsComp* con = *it;
//		LOG(" -- con [ "<<con->name<<"]");
//		boost::unordered_map<int,SyntaxNode*>::iterator pcon = con->partial.find(col_level);
//		SyntaxNode* attribute = NULL;
//		if(pcon!=con->partial.end())
//		{
//			attribute = (*pcon).second;
//			LOG(" -- partial attribute ["<<attribute->print()<<"]");
//		}
//
//		SyntaxNode* indexing = con->indexing;
//		if(indexing!=NULL)
//		{
//			LOG(" -- con index["<<indexing->print()<<"]");
//			IndexSet* iset = indexing->createIndexSet(ctx);
//			assert(iset->dummyCompMap.size()==1); //so far support only 1-demonsianl set
//			ModelComp* setcomp = iset->dummyCompMap.begin()->second;
//			Set* indset = iset->dummySetMap.begin()->second;
//			string dummy = iset->dummyCompMap.begin()->first;
//			std::vector<string>::iterator itset = indset->setValues_data_order.begin();
//			for(;itset!=indset->setValues_data_order.end();itset++)
//			{
//				string value = *itset;
//				this->ctx->addDummyCompValueMapTemp(dummy,setcomp,value);
//				string conName = con->name + "_" + value;
//				LOG(" constraint - ["<<conName<<"]");
//				//now, build autodiff constraint
//				AutoDiff::Node* acon = NULL;
//				if(attribute!=NULL)
//				{
//					acon = attribute->buildAutoDiffDAG(this,emcol);
//				}
//				emb->cons.push_back(acon);
//				this->ctx->removeDummySetValueMapTemp(dummy);
//			}
//			delete iset;
//		}
//		else
//		{
//			LOG(" -- con index is NULL");
//			string conName = con->name + "_";
//			LOG(" constraint - ["<<conName<<"]");
//			//now, build autodiff constraint
//			AutoDiff::Node* acon = NULL;
//			if(attribute!=NULL)
//			{
//				acon = attribute->buildAutoDiffDAG(this,emcol);
//			}
//			emb->cons.push_back(acon);
//		}
//	}
//
//	assert(this->numLocalCons==emb->cons.size());
//
//	if(GV(logBlock)){
//		ostringstream oss;
//		oss<<GV(logdir);
//		this->getQaulifiedName(oss);
//		emcol->getQaulifiedName(oss);
//		oss<<".dblk";
//		ofstream fout(oss.str().c_str());
//		emb->logBlock(this,emcol,fout);
//	}
//	LOG("createEMBlockDistributed - row["<<this->name<<"] col["<<emcol->name<<"] -- ncon["<<emb->cons.size()<<"] nvar["<<emcol->numLocalVars<<"]");
//	return emb;
//}
//
//void ExpandedModel::cons_jacobs_distribute(ExpandedModel *emcol, std::vector<boost::numeric::ublas::compressed_matrix<double> >& blocks)
//{
//	LOG("enter cons_jacobs_distribute - emrow["<<this->name<<"] emcol["<<emcol->name<<"]");
//	//will need to check the current emrow/emcol level, more than one nz block will be returned
//
//	boost::unordered_map<ExpandedModel*,BlockCons* >::iterator it=cblockMap_ds.find(emcol);
//	BlockCons* emb = NULL;
//	if(it==cblockMap_ds.end()){
//		emb = this->createConsBlockDistributed(emcol);
//		cblockMap_ds.insert(pair<ExpandedModel*,BlockCons*>(emcol,emb));
//	}
//	else{
//		emb = (*it).second;
//	}
//
//	std::vector<AutoDiff::Node*> vnodes;
//	for(std::vector<ExpandedModel*>::iterator it=emb->block->ems.begin();it!=emb->block->ems.end();it++)
//	{
//		LOG("em["<<(*it)->name);
//		(*it)->copyVariables(vnodes);
//	}
////	LOG("vnode size -"<<vnodes.size());
//	compressed_matrix<double> m(emb->cons.size(),vnodes.size());
//	int r = 0;
//	for(std::vector<AutoDiff::Node*>::iterator it=emb->cons.begin();it!=emb->cons.end();it++){
//		matrix_row<compressed_matrix<double> > rgrad (m,r);
//		if(*it!=NULL){
//			LOG("con - \n"<<visit_tree(*it));
//			AutoDiff::grad_reverse(*it,vnodes,rgrad);
//		}
//		r++;
////		LOG("jacobian now"<<m);
//	}
////	LOG("Full Jacobian: "<<m);
//
//	int prev_c  = 0;
//	for(std::vector<ExpandedModel*>::iterator it=emb->block->ems.begin();it!=emb->block->ems.end();it++)
//	{
//		int num_c = (*it)->numLocalVars + prev_c;
//		matrix_range<compressed_matrix<double> > mr(m,range(0,m.size1()),range(prev_c,num_c));
//		LOG("submatrix -- nvar["<<(*it)->numLocalVars<<"] - level["<<(*it)->model->level<<"] em["<<(*it)->name<<"]");
//		compressed_matrix<double> subm(mr);
//		blocks.push_back(subm);
//		prev_c=num_c;
//	}
//	assert(emcol->model->level+1 == blocks.size());
//	assert(blocks.size()==emb->block->ems.size());
//	LOG("end cons_jacobs_distribute");
//}
//
//void ExpandedModel::cons_hessian_distribute(ExpandedModel* emcol, std::vector<boost::numeric::ublas::compressed_matrix<double> >& blocks)
//{
//	LOG("enter cons_hessian_distribute - emrow["<<this->name<<"] emcol"<<emcol->name<<"]");
//	int nb = (emcol->model->level+2)*(emcol->model->level+1)/2;
//	LOG("number of block - "<<nb);
//
//	boost::unordered_map<ExpandedModel*,BlockCons* >::iterator it=cblockMap_ds.find(emcol);
//	BlockCons* emb = NULL;
//	if(it==cblockMap_ds.end()){
//		emb = this->createConsBlockDistributed(emcol);
//		cblockMap_ds.insert(pair<ExpandedModel*,BlockCons*>(emcol,emb));
//	}
//	else{
//		emb = (*it).second;
//	}
//
//	std::vector<AutoDiff::Node*> vnodes;
//	for(std::vector<ExpandedModel*>::iterator it=emb->block->ems.begin();it!=emb->block->ems.end();it++)
//	{
//		LOG("em["<<(*it)->name);
//		(*it)->copyVariables(vnodes);
//	}
//
//	boost::numeric::ublas::compressed_matrix<double> fullhess(vnodes.size(),vnodes.size());
//	for(std::vector<AutoDiff::Node*>::iterator cit=emb->cons.begin();cit!=emb->cons.end();cit++){
//		boost::numeric::ublas::compressed_matrix<double> m(vnodes.size(),vnodes.size());
//		for(std::vector<AutoDiff::Node*>::iterator nit=vnodes.begin();nit!=vnodes.end();nit++){
//			int c = 0;
//			boost::numeric::ublas::matrix_column<boost::numeric::ublas::compressed_matrix<double> > chess(m,c);
//			if(*cit!=NULL){
//				hess_reverse(*cit,vnodes,chess);
//			}
//			c++;
//		}
////		LOG("hessian curr"<<m);
//		fullhess = fullhess + m;
////		LOG("full hess now"<<fullhess);
//	}
////	LOG("Full hessian: "<<fullhess);
//
//	int pre_r = 0;
//	for(uint i=0;i<emb->block->ems.size();i++)
//	{
//		int pre_c = pre_r;
//		int num_r = pre_r + emb->block->ems.at(i)->numLocalVars;
//		for(uint j=i;j<emb->block->ems.size();j++)
//		{
//			int num_c = pre_c + emb->block->ems.at(j)->numLocalVars;
//			boost::numeric::ublas::matrix_range<boost::numeric::ublas::compressed_matrix<double> > mr(fullhess,range(pre_r,num_r),range(pre_c,num_c));
////			LOG("row block:"<<mr);
//			boost::numeric::ublas::compressed_matrix<double> subm(mr);
//			blocks.push_back(mr);
//			pre_c = num_c;
//		}
//		pre_r = num_r;
//	}
//}
//
//void ExpandedModel::cons_feval_distribute(ExpandedModel *emcol, std::vector<double>& fvals)
//{
//	LOG("enter cons_feval_distribute - this["<<this->name<<"] emcol["<<emcol->name<<"]");
//	assert(fvals.empty());
//	boost::unordered_map<ExpandedModel*,BlockCons* >::iterator it=cblockMap_ds.find(emcol);
//	BlockCons* emb = NULL;
//	if(it==cblockMap_ds.end()){
//		emb = this->createConsBlockDistributed(emcol);
//		cblockMap_ds.insert(pair<ExpandedModel*,BlockCons*>(emcol,emb));
//	}
//	else{
//		emb = (*it).second;
//	}
//
//	int i=0;
//	for(std::vector<AutoDiff::Node*>::iterator it=emb->cons.begin();it!=emb->cons.end();it++){
//		if(*it==NULL){
//			fvals.push_back(0);
//		}
//		else{
//			fvals.push_back(eval_function(*it));
//		}
//		LOG("fvals["<<i<<"]="<<fvals.at(fvals.size()-1));
//		i++;
//	}
//	LOG("end cons_feval_distribute - this["<<this->name<<"] emcol["<<emcol->name<<"]");
//	assert(fvals.size()==this->numLocalCons);
//}
//
//int ExpandedModel::nz_jacobs_distribute(ExpandedModel* emcol, std::vector<unsigned int>& nzs)
//{
//	LOG("enter nz_jacobs_distribute - emrow["<<this->name<<"] emcol"<<emcol->name<<"]");
//	//will need to check the current emrow/emcol level, more than one nz block will be returned
//	LOG("number of block - "<<emcol->model->level+1);
//
//	boost::unordered_map<ExpandedModel*,BlockCons* >::iterator it=cblockMap_ds.find(emcol);
//	BlockCons* emb = NULL;
//	if(it==cblockMap_ds.end()){
//		emb = this->createConsBlockDistributed(emcol);
//		cblockMap_ds.insert(pair<ExpandedModel*,BlockCons*>(emcol,emb));
//	}
//	else{
//		emb = (*it).second;
//	}
//
//	for(std::vector<ExpandedModel*>::iterator it=emb->block->ems.begin();it!=emb->block->ems.end();it++)
//	{
//		boost::unordered_set<Node*> vSet;
//		(*it)->copyVariables(vSet);
//		unsigned int nz=0;
//		for(std::vector<AutoDiff::Node*>::iterator cit=emb->cons.begin();cit!=emb->cons.end();cit++){
//			if(*cit!=NULL){
//				nz+=nzGrad(*cit,vSet);
//			}
//		}
//		LOG("Nz for block - "<<nz);
//		nzs.push_back(nz);
//	}
//	assert(emcol->model->level+1 == nzs.size());
//	LOG("end nz_jacobs_distribute");
//	return nzs.size();
//}
//
///* End of Distribute Interface Implemenation
// **********************************************************************************************
// * */
//
//
///* * ******************************************************************************************


//
//uint ExpandedModel::nz_cons_hess_local(ExpandedModel *emcol)
//{
//	LOG("enter nz_cons_hess_local called -- this["<<this->name<<"] emcol["<<emcol->name<<"]");
//	assert(this!=emcol || (this->model->level == emcol->model->level && this==emcol)); //the diagonal block
//	BlockCons* emb = this->getBlockConsFull();
//
//	//get the nonlinear edges from all constraints in the block
//	AutoDiff::EdgeSet edgeSet;
//	BOOST_FOREACH(AutoDiff::Node* con, emb->cons)
//	{
//		assert(con!=NULL);
//		LOG("constraint expression  ---- ");
//		LOG("-- "<<visit_tree(con));
//		nonlinearEdges(con,edgeSet);
//		LOG("nonlinearEdges - now: -- "<<edgeSet.toString());
//	}
//	//create variable map for emcol and emrow
//	boost::unordered_set<Node*> colvSet;
//	emcol->copyVariables(colvSet);
//	boost::unordered_set<Node*> rowvSet;
//	this->copyVariables(rowvSet);
//	LOG("rowvSet size["<<rowvSet.size()<<"]  -- colvSet size["<<colvSet.size()<<"]");
//
//	uint nz = nzHess(edgeSet,colvSet,rowvSet);
//	LOG("nonlinearEdges - removed other edges - -"<<edgeSet.toString());
//	LOG("end nz_cons_hess_local -- this["<<this->name<<"] emcol["<<emcol->name<<"]  - Num of Nonzero["<<nz<<"]");
//	Stat::numNZConsHessLocalCall++;
//	return nz;
//}
//
//void ExpandedModel::cons_hess_local(ExpandedModel* emcol, boost::numeric::ublas::compressed_matrix<double>& block)
//{
//	LOG("enter cons_hess_local - emrow["<<this->name<<"] emcol"<<emcol->name<<"]");
//	assert(block.size1() == 0 && block.size2() == 0);
//	assert(this!=emcol || (this->model->level == emcol->model->level && this==emcol)); //the diagonal block
//	BlockCons* emb = this->getBlockConsFull();
//
//	std::vector<AutoDiff::Node*> vnodes;
//	uint colstart = 0;
//	uint rowstart = 0;
//	uint currvar = 0;
//	BOOST_FOREACH(ExpandedModel* em, emb->block->ems)
//	{
//		LOG("em["<<em->name<<"]");
//		if(em == emcol) {
//			colstart = currvar;
//		}
//		if(em == this) {
//			rowstart = currvar;
//		}
//		em->copyVariables(vnodes);
//		currvar += em->numLocalVars;
//		assert(vnodes.size() == currvar);
//	}
//
//	boost::numeric::ublas::compressed_matrix<double> fullhess(vnodes.size(),vnodes.size());
//	BOOST_FOREACH(AutoDiff::Node* con, emb->cons)
//	{
//		boost::numeric::ublas::compressed_matrix<double> m(vnodes.size(),vnodes.size());
//		for(uint c = 0;c<vnodes.size();c++)
//		{
//			//initialize the weight for computing column c of Hessian m
//			BOOST_FOREACH(AutoDiff::Node* v, vnodes)
//			{
//				static_cast<AutoDiff::VNode*>(v)->u = 0;
//			}
//			static_cast<AutoDiff::VNode*>(vnodes[c])->u = 1; //set weight = 1 for column c varible only, rest are 0
//			//calculate the column c for Hessian m
//			typedef boost::numeric::ublas::matrix_column<boost::numeric::ublas::compressed_matrix<double> > compressed_matrix_col;
//			compressed_matrix_col chess(m,c);
//			assert(con!=NULL);
//			hess_reverse(con,vnodes,chess);
//			c++;
//		}
////		LOG("hessian curr"<<m);
//		fullhess = fullhess + m;
////		LOG("full hess now"<<fullhess);
//	}
////	LOG("Full hessian: "<<fullhess);
//
//	typedef boost::numeric::ublas::matrix_range<boost::numeric::ublas::compressed_matrix<double> > compressed_matrix_range;
//	uint colrange = colstart + emcol->numLocalVars;
//	uint rowrange = rowstart + this->numLocalVars;
//	compressed_matrix_range mr(fullhess,range(rowstart,rowrange),range(colstart,colrange));
//	block = mr;
//	assert(mr.size1()==this->numLocalVars);
//	assert(mr.size2()==emcol->numLocalVars);
//	LOG("submatrix -- ["<<this->numLocalVars<<"] x ["<<emcol->numLocalVars<<"]");
//	block = mr;
//
//	Stat::numConsHessLocalCall++;
//	LOG("end cons_hess_local - emrow["<<this->name<<"] emcol"<<emcol->name<<"]");
//}

//uint ExpandedModel::nz_obj_hess_local(ExpandedModel* emcol)
//{
//	LOG("enter nz_obj_hess_local - this["<<this->name<<"] x emcol["<<emcol->name<<"]");
//	assert(this!=emcol || (this->model->level == emcol->model->level && this==emcol)); //the diagonal block
//
//	uint nz = 0;
//	if(this->model->obj_comp != NULL){
//		BlockObj* ob = this->getBlockObjFull(emcol);
//		//get the nonlinear edges from all constraints in the block
//		AutoDiff::EdgeSet edgeSet;
//		uint nz = 0;
//		if(ob->objective!=NULL)
//		{
//			LOG("constraint expression  ---- ");
//			LOG("-- "<<visit_tree(ob->objective));
//			nonlinearEdges(ob->objective,edgeSet);
//			LOG("nonlinearEdges - now: -- "<<edgeSet.toString());
//			//create variable map for emcol and emrow
//			boost::unordered_set<Node*> colvSet;
//			emcol->copyVariables(colvSet);
//			boost::unordered_set<Node*> rowvSet;
//			this->copyVariables(rowvSet);
//			LOG("rowvSet size["<<rowvSet.size()<<"]  -- colvSet size["<<colvSet.size()<<"]");
//			nz = nzHess(edgeSet,colvSet,rowvSet);
//			LOG("nonlinearEdges - removed other edges - -"<<edgeSet.toString());
//		}
//	}
//	LOG("end nz_obj_hess_local -- this["<<this->name<<"] emcol["<<emcol->name<<"]  - Num of Nonzero["<<nz<<"]");
//	Stat::numNZObjHessLocalCall++;
//	return nz;
//}
//
//
//void ExpandedModel::obj_hess_local(ExpandedModel* emcol, boost::numeric::ublas::compressed_matrix<double>& block)
//{
//	LOG("enter obj_hess_local - this["<<this->name<<"] x emcol["<<emcol->name<<"]");
//	assert(this!=emcol || (this->model->level == emcol->model->level && this==emcol)); //the diagonal block
//	assert(block.size1() == 0 && block.size2()==0);
//	if(this->model->obj_comp == NULL){
//		block.resize(this->numLocalVars,emcol->numLocalVars,false);
//	}
//	else{
//		BlockObj* ob = this->getBlockObjFull(emcol);
//		std::vector<AutoDiff::Node*> vnodes;
//		uint colstart = 0;
//		uint rowstart = 0;
//		uint currvar = 0;
//		BOOST_FOREACH(ExpandedModel* em, ob->block->ems)
//		{
//			LOG("em["<<em->name<<"]");
//			if(em == emcol) {
//				colstart = currvar;
//			}
//			if(em == this) {
//				rowstart = currvar;
//			}
//			em->copyVariables(vnodes);
//			currvar += em->numLocalVars;
//			assert(vnodes.size() == currvar);
//		}
//
//		//computating the Hessian for the full objective declared at this em
//		boost::numeric::ublas::compressed_matrix<double> fullhess(vnodes.size(),vnodes.size());
//		for(uint c = 0;c<vnodes.size();c++)
//		{
//			//initialize the weight for computing column c of Hessian m
//			BOOST_FOREACH(AutoDiff::Node* v, vnodes)
//			{
//				static_cast<AutoDiff::VNode*>(v)->u = 0;
//			}
//			static_cast<AutoDiff::VNode*>(vnodes[c])->u = 1; //set weight = 1 for column c varible only, rest are 0
//			//calculate the column c for Hessian m
//			typedef boost::numeric::ublas::matrix_column<boost::numeric::ublas::compressed_matrix<double> > compressed_matrix_col;
//			compressed_matrix_col chess(fullhess,c);
//			assert(ob->objective!=NULL);
//			hess_reverse(ob->objective,vnodes,chess);
//			c++;
//		}
//		//	LOG("full hess now"<<fullhess);
//
//		//take the submatrix of the Hessian declared by intersection of variables declared at this em and emcol.
//		typedef boost::numeric::ublas::matrix_range<boost::numeric::ublas::compressed_matrix<double> > compressed_matrix_range;
//		uint colrange = colstart + emcol->numLocalVars;
//		uint rowrange = rowstart + this->numLocalVars;
//		compressed_matrix_range mr(fullhess,range(rowstart,rowrange),range(colstart,colrange));
//		block = mr;
//		LOG("submatrix -- ["<<this->numLocalVars<<"] x ["<<emcol->numLocalVars<<"]");
//	}
//	assert(block.size1()==this->numLocalVars);
//	assert(block.size2()==emcol->numLocalVars);
//	Stat::numObjHessLocalCall++;
//	LOG("end obj_hess_local - emrow["<<this->name<<"] emcol"<<emcol->name<<"]");
//}

//  getBlockObjFull disabled because we assume objective only has variables from this Expanded Model only.
//  therefore, getBlockObjPartial should be sufficient to compute objective for this block.
//	the full objective value will be the sum{all expandedmodel m}( m.obj_feval() )
//BlockObj* ExpandedModel::getBlockObjFull()
//{
//	ObjComp* objcomp = this->model->obj_comp;
//	assert(objcomp!=NULL);
//	if(oblockFull!=NULL)
//	{
//		LOG("constructObjBlockLocal -- already in init");
//		return oblockFull;
//	}
//	else
//	{
//		BlockDep* bdep = this->getBlockDep();
//		oblockFull = new BlockObj(bdep);
//		LOG(" -- objcomp [ "<<objcomp->name<<"]");
//		SyntaxNode* attribute = objcomp->attributes;
//		assert(objcomp->indexing == NULL); //TODO: no multi-objective supported yet!
//		string objName = objcomp->name;
//		LOG(" obj constraints - ["<<objName<<"]");
//		//now, build autodiff constraint node for object constraint
//		Node* obj = attribute->buildAutoDiffDAG(this);
//		assert(obj!=NULL);
//		oblockFull->objective = obj;
//
//		if(GV(logBlock)){
//			ostringstream oss;
//			oss<<GV(logdir)<<this->qualifiedName()<<".fullobjblk";
//			ofstream fout(oss.str().c_str());
//			oblockFull->logBlock(this,NULL,fout);
//		}
//	}
//	LOG("exit - getBlockObjFull - row["<<this->name<<"]  --- with OBJ["<<this->model->obj_comp->name<<"]");
//	assert(oblockFull!=NULL);
//	return oblockFull;
//}


//BlockObjLP* ExpandedModel::getBlockObjLP(ExpandedModel* emcol)
//{
//	assert(this->model->obj_comp!=NULL);
//	BlockObjLP* rval = NULL;
//	boost::unordered_map<ExpandedModel*, BlockObjLP*>::iterator it = lp_oblockMap.find(emcol);
//	if(it!=lp_oblockMap.end())
//	{
//		rval = it->second;
//	}
//	else
//	{
//		rval = new BlockObjLP(this,emcol);
//		this->recursiveInitContext();
//		emcol->recursiveInitContext();
//		this->model->calculateModelCompRecursive(this->ctx);
//		emcol->model->calculateModelCompRecursive(emcol->ctx);
//		emcol->model->calculateLocalVar(emcol->ctx);
//
//		unordered_map<int,SyntaxNode*>::iterator it= this->model->obj_comp->partial.find(emcol->model->level);
//		SyntaxNode* attr = it==this->model->obj_comp->partial.end()?NULL:it->second;
//		AutoDiff::Node* con = attr==NULL? NULL: attr->buildAutoDiffDAG(this,emcol,true);
//		rval->cnode = con;
//
//		if(GV(logBlock)){
//			ostringstream oss;
//			oss<<GV(logdir)<<this->qualifiedName()<<"-"<<emcol->qualifiedName()<<".objlpblk";
//			ofstream fout(oss.str().c_str());
//			rval->logBlock(this,emcol,fout);
//		}
//
//		this->lp_oblockMap.insert(pair<ExpandedModel*,BlockObjLP*>(emcol,rval));
//	}
//	return rval;
//}
