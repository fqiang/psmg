/*
 * ExpandedModel2.cpp
 *
 *  Created on: 17 Oct 2011
 *      Author: s0965328
 */

#include "ExpandedModel.h"
#include "../model/SyntaxNodeIDREF.h"
#include "../model/SyntaxNodeID.h"
#include "../model/AmplModel.h"
#include "Block.h"
#include "BlockCons.h"
#include "BlockObj.h"
#include "BlockHV.h"
#include "BlockLP.h"
#include "Var.h"
#include "VarSingle.h"
#include "Con.h"
#include "ConSingle.h"
#include "../model/ObjComp.h"
#include "../parser/sml.tab.h"
#include <boost/foreach.hpp>
#include <cassert>
#include <iomanip>
#include <list>
#include <limits>

using namespace boost::numeric::ublas;
using namespace std;

typedef AutoDiff::Node Node;

ExpandedModel* ExpandedModel::root = NULL; //initialize root to NULL

ExpandedModel::ExpandedModel(AmplModel* mod,ModelContext* context)
: model(mod),ctx(context),parent(NULL),block_lo(NULL),numLocalVars(0),numLocalCons(0),name(mod->name)
{
	this->name = model->name;
	ctx->em = this;
	LOG("ExpandedModel2 constructor for name["<<this->name<<"]");
}

ExpandedModel::~ExpandedModel() {
	LOG("ExpandedModel2 destructor called !!!");
	LOG("delete context recurisivly!");
	delete ctx;
	dummySetMap.clear();
	dummyValueMap.clear();
	for(uint i=0;i<this->children.size();i++)
	{
		delete children.at(i);
	}
	this->children.clear();
}

BlockLP* ExpandedModel::getBlockLP(ExpandedModel* emcol)
{
	BlockLP* rval = NULL;
	boost::unordered_map<ExpandedModel*, BlockLP*>::iterator it = lpBlockMap.find(emcol);
	if(it!=lpBlockMap.end())
	{
		rval =  it->second;
	}
	else
	{
		rval = new BlockLP(this,emcol);
		std::vector<ExpandedModel*> ems;
		ems.push_back(this);
		ems.push_back(emcol);
		for(std::vector<ExpandedModel*>::iterator it = ems.begin();it != ems.end();it++) {
			(*it)->recursiveInitContext();
		}
		for(std::vector<ExpandedModel*>::iterator it = ems.begin();it != ems.end();it++) {
			(*it)->model->calculateModelCompRecursive((*it)->ctx);
		}
		emcol->model->calculateLocalVar(emcol->ctx);

		BOOST_FOREACH(ConsComp* con, this->model->con_comps)
		{
			//now build each constraint in autodiff format
			LOG(" -- con [ "<<con->name<<"]");
			Con* conset = new Con(con->name);
			SyntaxNode* attribute = con->attributes;
			SyntaxNode* indexing = con->indexing;
			if(indexing!=NULL)
			{
				LOG(" -- con index["<<indexing->print()<<"]");
				IndexSet* iset = indexing->createIndexSet(this->ctx);
				assert(iset->dummySetMap.size()==1); //so far support only 1-demonsianl set
				Set* set = iset->dummySetMap.begin()->second;
				string dummy = iset->dummySetMap.begin()->first;
				SetComp* setcomp = iset->dummyCompMap.begin()->second;
				std::vector<string>::iterator itset = set->setValues_data_order.begin();
				for(;itset!=set->setValues_data_order.end();itset++)
				{
					string value = *itset;
					this->ctx->addDummyCompValueMapTemp(dummy,setcomp,value);
					string conName = con->name + "_" + value;
					LOG(" constraint - ["<<conName<<"]");
					//now, build autodiff constraint
					SyntaxNode* assgin_expr = attribute->findChildNode(ASSIGN);
					Node* acon = assgin_expr->buildAutoDiffDAG(this,emcol,true);
					assert(acon!=NULL);
					double lower = NEG_INFINITY_D;
					double upper = INFINITY_D;
					con->attributes->calculateConsBounds(this->ctx,lower,upper);
					ConSingle* consingle = new ConSingle(acon, lower, upper);
					conset->cons.push_back(consingle);
					this->ctx->removeDummySetValueMapTemp(dummy);
				}
				delete iset;
			}
			else
			{
				LOG(" -- con index is NULL - constraint - ["<<con->name<<"]");
				//now, build autodiff constraint
				double lower = NEG_INFINITY_D;
				double upper = INFINITY_D;
				SyntaxNode* assgin_expr = attribute->findChildNode(ASSIGN);
				Node* acon = assgin_expr->buildAutoDiffDAG(this,emcol,true);
				assert(acon!=NULL);
				con->attributes->calculateConsBounds(ctx,lower,upper);
				ConSingle* consingle = new ConSingle(acon, lower, upper);
				conset->cons.push_back(consingle);
			}
			rval->cons.push_back(conset);
		}
		if(GV(logBlock)){
			ostringstream oss;
			oss<<GV(logdir);
			this->getQaulifiedName(oss);
			oss<<"-";
			emcol->getQaulifiedName(oss);
			oss<<".lpblk";
			ofstream fout(oss.str().c_str());
			rval->logBlock(this,emcol,fout);
		}

		lpBlockMap.insert(pair<ExpandedModel*,BlockLP*>(emcol,rval));
	}
	return rval;
}

uint ExpandedModel::nz_cons_jacobs(ExpandedModel* emcol)
{
	BlockLP* b = this->getBlockLP(emcol);
	boost::unordered_set<AutoDiff::Node*> vSet;
	emcol->copyVariables(vSet);
	assert(vSet.size()==emcol->numLocalVars);
	uint nz = 0;
	BOOST_FOREACH(Con* con, b->cons)
	{
		BOOST_FOREACH(ConSingle* consingle, con->cons)
		{
			nz+=nzGrad(consingle->con,vSet);
			LOG("nz now "<<nz);
		}
	}
	LOG("row["<<name<<"] col["<<emcol->name<<"] - nz["<<nz<<"]");
	return nz;
}

void ExpandedModel::cons_jacobs(ExpandedModel* emcol,boost::numeric::ublas::compressed_matrix<double>& m)
{
	std::vector<AutoDiff::Node*> vnodes;
	emcol->copyVariables(vnodes);
	assert(emcol->getNLocalVars() == vnodes.size());
//	LOG("vnode size -"<<vnodes.size());
	m.resize(this->getNLocalCons(),emcol->getNLocalVars(),false);

	BlockLP* b = this->getBlockLP(emcol);
	int r = 0;
	BOOST_FOREACH(Con* con, b->cons)
	{

		BOOST_FOREACH(ConSingle* singlecon, con->cons)
		{
			matrix_row<compressed_matrix<double> > rgrad(m,r);
			assert(con!=NULL);
//			LOG("constraint expression  ---- ");
//			LOG("--- "<<AutoDiff::visit_tree(singlecon->con));
			grad_reverse(singlecon->con,vnodes,rgrad);
			r++;
		}
		LOG("row["<<name<<"] col["<<emcol->name<<"]");
		LOG("jacobian now"<<m);
	}
	assert(r == this->getNLocalCons());
//	LOG("Full Jacobian: "<<m);
	assert(m.size1()==this->getNLocalCons());
	assert(m.size2()==emcol->getNLocalVars());

	Stat::numConsJacLocalCall++;
	LOG("end cons_jacobs_distribute - emrow["<<this->name<<"] emcol["<<emcol->name<<"]");
}
void ExpandedModel::obj_grad(ExpandedModel* emcol, double* vals)
{
	if(this->model->obj_comp!=NULL){

		this->recursiveInitContext();
		emcol->recursiveInitContext();
		this->model->calculateModelCompRecursive(this->ctx);
		emcol->model->calculateModelCompRecursive(emcol->ctx);
		emcol->model->calculateLocalVar(emcol->ctx);

		AutoDiff::Node* con = this->model->obj_comp->attributes->buildAutoDiffDAG(this,emcol,true);
		assert(con!=NULL);
		std::vector<AutoDiff::Node*> vnodes;
		emcol->copyVariables(vnodes);

		std::vector<double> grad;
		LOG("objective expression  ---- ");
		LOG("--- "<<visit_tree(con));
		grad_reverse(con,vnodes,grad);
		for(uint i=0;i<grad.size();i++)
		{
			//set to 0 if variable is not in the objective expression
			vals[i] = isnan(grad[i])?0:grad[i];
		}
	}
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
ExpandedModel::cons_feval_local
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
void ExpandedModel::cons_feval_local(std::vector<double>& fvals){
	LOG("enter cons_feval_local - this["<<this->name<<"] emcol["<<this->name<<"]");
	assert(fvals.empty());
	BlockCons* emb = this->getConsBlockLocal(this);

	BOOST_FOREACH(AutoDiff::Node* con, emb->cons)
	{
		assert(con != NULL);
		fvals.push_back(eval_function(con));
	}
	LOG("end cons_feval_local - this["<<this->name<<"] emcol["<<this->name<<"] - fvals size["<<fvals.size()<<"]");
	Stat::numConsEvalLocalCall++;
	assert(fvals.size()==this->getNLocalCons());
}

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
uint ExpandedModel::nz_cons_jacobs_local(ExpandedModel *emcol)
{
	LOG("enter nz_cons_jacobs_local called -- this["<<this->name<<"] emcol["<<emcol->name<<"]");
	assert(this!=emcol || (this->model->level == emcol->model->level && this==emcol)); //the diagonal block
	BlockCons* emb = this->getConsBlockLocal(emcol);

	//create variable map for emcol only
	boost::unordered_set<AutoDiff::Node*> vSet;
	this->copyVariables(vSet);
	assert(vSet.size()==this->numLocalVars);
	uint nz = 0;
	BOOST_FOREACH(AutoDiff::Node* con, emb->cons)
	{
//		LOG("con \n"<<visit_tree(con)<<"");
		nz+=nzGrad(con,vSet);
		LOG("nz now "<<nz);
	}

	LOG("end nz_cons_jacobs_local -- this["<<this->name<<"] emcol["<<emcol->name<<"]  - Num of Nonzero["<<nz<<"]");
	Stat::numNZConsJacLocalCall++;
	return nz;
}

/* ----------------------------------------------------------------------------
ExpandedModel::cons_jacobs_distribute
---------------------------------------------------------------------------- */
void ExpandedModel::cons_jacobs_local(ExpandedModel *emcol, boost::numeric::ublas::compressed_matrix<double>& block)
{
	LOG("enter cons_jacobs_distribute - emrow["<<this->name<<"] emcol["<<emcol->name<<"]");
	assert(this!=emcol || (this->model->level == emcol->model->level && this==emcol)); //the diagonal block
	assert(block.size1()==0 && block.size2()== 0);
	BlockCons* emb = this->getConsBlockLocal(emcol);

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
	compressed_matrix<double> m(emb->cons.size(),vnodes.size());
	int r = 0;
	BOOST_FOREACH(AutoDiff::Node* con, emb->cons)
	{
//		typedef boost::numeric::ublas::matrix_row<boost::numeric::ublas::compressed_matrix<double> > compressed_matrix_row;
//		compressed_matrix_row rgrad(m,r);
		matrix_row<compressed_matrix<double> > rgrad(m,r);
		assert(con!=NULL);
		LOG("constraint expression  ---- ");
		LOG("--- "<<visit_tree(con));
		AutoDiff::grad_reverse(con,vnodes,rgrad);
		r++;
//		LOG("jacobian now"<<m);
	}
//	LOG("Full Jacobian: "<<m);

	int colrange = emcol->getNLocalVars() + colstart;
	matrix_range<compressed_matrix<double> > mr(m,range(0,m.size1()),range(colstart,colrange));
	assert(mr.size2()==emcol->getNLocalVars());
	assert(mr.size1()==this->getNLocalCons());
	LOG("submatrix -- ["<<this->getNLocalCons()<<"] x ["<<emcol->getNLocalVars()<<"]");
	block = mr;

	Stat::numConsJacLocalCall++;
	LOG("end cons_jacobs_distribute - emrow["<<this->name<<"] emcol["<<emcol->name<<"]");
}

uint ExpandedModel::nz_lag_hess_local(ExpandedModel* emcol)
{
	LOG("enter nz_cons_hess_local called -- this["<<this->name<<"] emcol["<<emcol->name<<"]");
	assert(this!=emcol || (this->model->level == emcol->model->level && this==emcol)); //the diagonal block
	BlockHV* emb = this->getHVBlockLocal(emcol);
	//get the nonlinear edges from all constraints in the block
	AutoDiff::EdgeSet edgeSet;

	BOOST_FOREACH(ExpandedModel* em, emb->block->ems)
	{
		BOOST_FOREACH(ConsComp* con , em->model->con_comps)
		{
			Con* conset = static_cast<Con*>(em->ctx->getCompValue(con));
			BOOST_FOREACH(ConSingle* consingle, conset->cons)
			{
				AutoDiff::Node* acon = consingle->con;
				assert(acon!=NULL);
				LOG("constraint expression  ---- ");
				LOG("-- "<<visit_tree(acon));
				nonlinearEdges(acon,edgeSet);
			}
		}
	}
	if(emb->obj!=NULL)
	{
		LOG("objective expression  ---- ");
		LOG("-- "<<visit_tree(emb->obj));
		nonlinearEdges(emb->obj,edgeSet);
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

void ExpandedModel::lag_hess_local(ExpandedModel* emcol,boost::numeric::ublas::compressed_matrix<double>& block)
{
	LOG("enter lag_hess_local - emrow["<<this->name<<"] emcol"<<emcol->name<<"]");
	assert(block.size1() == 0 && block.size2() == 0);
	assert(this!=emcol || (this->model->level == emcol->model->level && this==emcol)); //the diagonal block
	BlockHV* emb = this->getHVBlockLocal(emcol);

	uint colstart = 0;
	uint rowstart = 0;
	uint currvar = 0;
	std::vector<AutoDiff::Node*> vnodes;
	BOOST_FOREACH(ExpandedModel* em, emb->block->ems)
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

	boost::numeric::ublas::compressed_matrix<double> fullhess(vnodes.size(),vnodes.size());
	BOOST_FOREACH(ExpandedModel* em, emb->block->ems)
	{
		BOOST_FOREACH(ConsComp* con, em->model->con_comps)
		{
			Con* conset = static_cast<Con*>(em->ctx->getCompValue(con));
			assert(conset!=NULL);
			BOOST_FOREACH(ConSingle* consingle, conset->cons)
			{
				boost::numeric::ublas::compressed_matrix<double> m(vnodes.size(),vnodes.size());
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
					typedef boost::numeric::ublas::matrix_column<boost::numeric::ublas::compressed_matrix<double> > compressed_matrix_col;
					compressed_matrix_col chess(m,c);
					assert(consingle->con!=NULL);
					hess_reverse(consingle->con,vnodes,chess);
					c++;
				}
				fullhess = fullhess + consingle->y*m;
			}
		}
	}

	if(emb->obj!=NULL)
	{
		boost::numeric::ublas::compressed_matrix<double> m(vnodes.size(),vnodes.size());
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
			typedef boost::numeric::ublas::matrix_column<boost::numeric::ublas::compressed_matrix<double> > compressed_matrix_col;
			compressed_matrix_col chess(m,c);
			hess_reverse(emb->obj,vnodes,chess);
			c++;
		}
		fullhess = fullhess + m;
	}

//	LOG("Full hessian: "<<fullhess);
	typedef boost::numeric::ublas::matrix_range<boost::numeric::ublas::compressed_matrix<double> > compressed_matrix_range;
	uint colrange = colstart + emcol->numLocalVars;
	uint rowrange = rowstart + this->numLocalVars;
	compressed_matrix_range mr(fullhess,range(rowstart,rowrange),range(colstart,colrange));
	block = mr;
	assert(mr.size1()==this->getNLocalVars());
	assert(mr.size2()==emcol->getNLocalVars());
	LOG("submatrix -- ["<<this->getNLocalVars()<<"] x ["<<emcol->getNLocalVars()<<"]");
	block = mr;

	Stat::numConsHessLocalCall++;
	LOG("end lag_hess_local - emrow["<<this->name<<"] emcol"<<emcol->name<<"]");
}

uint ExpandedModel::nz_cons_hess_local(ExpandedModel *emcol)
{
	LOG("enter nz_cons_hess_local called -- this["<<this->name<<"] emcol["<<emcol->name<<"]");
	assert(this!=emcol || (this->model->level == emcol->model->level && this==emcol)); //the diagonal block
	BlockCons* emb = this->getConsBlockLocal(emcol);

	//get the nonlinear edges from all constraints in the block
	AutoDiff::EdgeSet edgeSet;
	BOOST_FOREACH(AutoDiff::Node* con, emb->cons)
	{
		assert(con!=NULL);
		LOG("constraint expression  ---- ");
		LOG("-- "<<visit_tree(con));
		nonlinearEdges(con,edgeSet);
		LOG("nonlinearEdges - now: -- "<<edgeSet.toString());
	}
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

void ExpandedModel::cons_hess_local(ExpandedModel* emcol, boost::numeric::ublas::compressed_matrix<double>& block)
{
	LOG("enter cons_hess_local - emrow["<<this->name<<"] emcol"<<emcol->name<<"]");
	assert(block.size1() == 0 && block.size2() == 0);
	assert(this!=emcol || (this->model->level == emcol->model->level && this==emcol)); //the diagonal block
	BlockCons* emb = this->getConsBlockLocal(emcol);

	std::vector<AutoDiff::Node*> vnodes;
	uint colstart = 0;
	uint rowstart = 0;
	uint currvar = 0;
	BOOST_FOREACH(ExpandedModel* em, emb->block->ems)
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

	boost::numeric::ublas::compressed_matrix<double> fullhess(vnodes.size(),vnodes.size());
	BOOST_FOREACH(AutoDiff::Node* con, emb->cons)
	{
		boost::numeric::ublas::compressed_matrix<double> m(vnodes.size(),vnodes.size());
		for(uint c = 0;c<vnodes.size();c++)
		{
			//initialize the weight for computing column c of Hessian m
			BOOST_FOREACH(AutoDiff::Node* v, vnodes)
			{
				static_cast<AutoDiff::VNode*>(v)->u = 0;
			}
			static_cast<AutoDiff::VNode*>(vnodes[c])->u = 1; //set weight = 1 for column c varible only, rest are 0
			//calculate the column c for Hessian m
			typedef boost::numeric::ublas::matrix_column<boost::numeric::ublas::compressed_matrix<double> > compressed_matrix_col;
			compressed_matrix_col chess(m,c);
			assert(con!=NULL);
			hess_reverse(con,vnodes,chess);
			c++;
		}
//		LOG("hessian curr"<<m);
		fullhess = fullhess + m;
//		LOG("full hess now"<<fullhess);
	}
//	LOG("Full hessian: "<<fullhess);

	typedef boost::numeric::ublas::matrix_range<boost::numeric::ublas::compressed_matrix<double> > compressed_matrix_range;
	uint colrange = colstart + emcol->numLocalVars;
	uint rowrange = rowstart + this->numLocalVars;
	compressed_matrix_range mr(fullhess,range(rowstart,rowrange),range(colstart,colrange));
	block = mr;
	assert(mr.size1()==this->getNLocalVars());
	assert(mr.size2()==emcol->getNLocalVars());
	LOG("submatrix -- ["<<this->getNLocalVars()<<"] x ["<<emcol->getNLocalVars()<<"]");
	block = mr;

	Stat::numConsHessLocalCall++;
	LOG("end cons_hess_local - emrow["<<this->name<<"] emcol"<<emcol->name<<"]");
}

double& ExpandedModel::obj_feval_local(double& oval)
{
	LOG("enter obj_feval_local - this["<<this->name<<"] emcol["<<this->name<<"]");

	if(this->model->obj_comp == NULL)
	{
		LOG("no objective declared at -- this["<<this->name<<"]");
		oval = NaN_D;
	}
	else
	{
		BlockObj* ob = this->getObjBlockLocal(this);
		oval = eval_function(ob->objective);
		LOG(""<<visit_tree(ob->objective));
		assert(!isnan(oval));
	}
	LOG("end obj_feval_local - this["<<this->name<<"] emcol["<<this->name<<"] - oval["<<oval<<"]");
	Stat::numObjEvalLocalCall++;
	return oval;
}


/* -------------------------------------------------------------------------
ExpandedModel::obj_grad_local
-------------------------------------------------------------------------- */
/** Return the gradient of the objective defined in this model.
 *
 *  @param[out] elts
 *              The objective gradient with respect to the local variables.
 */
void ExpandedModel::obj_grad_local(ExpandedModel* emcol, std::vector<double>& ograd)
{
	LOG("enter obj_grad_local - this["<<this->name<<"] x emcol["<<emcol->name<<"]");
	assert(this!=emcol || (this->model->level == emcol->model->level && this==emcol)); //the diagonal block

	if(this->model->obj_comp==NULL)
	{
		ograd.resize(emcol->getNLocalVars(),NaN_D);
	}
	else{
		BlockObj* ob = this->getObjBlockLocal(emcol);
		std::vector<AutoDiff::Node*> vnodes;
		uint colstart  = 0;
		uint currcol = 0;
		BOOST_FOREACH(ExpandedModel* em, ob->block->ems)
		{
			LOG("em["<<em->name<<"]");
			if(em == emcol) {
				colstart = currcol;
			}
			em->copyVariables(vnodes);
			currcol += em->numLocalVars;
			assert(vnodes.size() == currcol);
		}

		//full vector of the objective gradient
		std::vector<double> grad;
		assert(ob->objective!=NULL);
		grad_reverse(ob->objective,vnodes,grad);
		assert(vnodes.size()==grad.size());
		//take sub-vector of those variables declared in emcol
		uint colrange = colstart + emcol->getNLocalVars();
		ograd.assign(grad.begin()+colstart,grad.begin()+colrange);
	}
	assert(ograd.size() == emcol->getNLocalVars());
	Stat::numObjGradLocalCall++;
	LOG("end obj_grad_local - this["<<this->name<<"] x emcol["<<emcol->name<<"] -- ograd size["<<ograd.size()<<"]");
}

uint ExpandedModel::nz_obj_hess_local(ExpandedModel* emcol)
{
	LOG("enter nz_obj_hess_local - this["<<this->name<<"] x emcol["<<emcol->name<<"]");
	assert(this!=emcol || (this->model->level == emcol->model->level && this==emcol)); //the diagonal block

	uint nz = 0;
	if(this->model->obj_comp != NULL){
		BlockObj* ob = this->getObjBlockLocal(emcol);
		//get the nonlinear edges from all constraints in the block
		AutoDiff::EdgeSet edgeSet;
		uint nz = 0;
		if(ob->objective!=NULL)
		{
			LOG("constraint expression  ---- ");
			LOG("-- "<<visit_tree(ob->objective));
			nonlinearEdges(ob->objective,edgeSet);
			LOG("nonlinearEdges - now: -- "<<edgeSet.toString());
			//create variable map for emcol and emrow
			boost::unordered_set<Node*> colvSet;
			emcol->copyVariables(colvSet);
			boost::unordered_set<Node*> rowvSet;
			this->copyVariables(rowvSet);
			LOG("rowvSet size["<<rowvSet.size()<<"]  -- colvSet size["<<colvSet.size()<<"]");
			nz = nzHess(edgeSet,colvSet,rowvSet);
			LOG("nonlinearEdges - removed other edges - -"<<edgeSet.toString());
		}
	}
	LOG("end nz_obj_hess_local -- this["<<this->name<<"] emcol["<<emcol->name<<"]  - Num of Nonzero["<<nz<<"]");
	Stat::numNZObjHessLocalCall++;
	return nz;
}


void ExpandedModel::obj_hess_local(ExpandedModel* emcol, boost::numeric::ublas::compressed_matrix<double>& block)
{
	LOG("enter obj_hess_local - this["<<this->name<<"] x emcol["<<emcol->name<<"]");
	assert(this!=emcol || (this->model->level == emcol->model->level && this==emcol)); //the diagonal block
	assert(block.size1() == 0 && block.size2()==0);
	if(this->model->obj_comp == NULL){
		block.resize(this->numLocalVars,emcol->numLocalVars,false);
	}
	else{
		BlockObj* ob = this->getObjBlockLocal(emcol);
		std::vector<AutoDiff::Node*> vnodes;
		uint colstart = 0;
		uint rowstart = 0;
		uint currvar = 0;
		BOOST_FOREACH(ExpandedModel* em, ob->block->ems)
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

		//computating the Hessian for the full objective declared at this em
		boost::numeric::ublas::compressed_matrix<double> fullhess(vnodes.size(),vnodes.size());
		for(uint c = 0;c<vnodes.size();c++)
		{
			//initialize the weight for computing column c of Hessian m
			BOOST_FOREACH(AutoDiff::Node* v, vnodes)
			{
				static_cast<AutoDiff::VNode*>(v)->u = 0;
			}
			static_cast<AutoDiff::VNode*>(vnodes[c])->u = 1; //set weight = 1 for column c varible only, rest are 0
			//calculate the column c for Hessian m
			typedef boost::numeric::ublas::matrix_column<boost::numeric::ublas::compressed_matrix<double> > compressed_matrix_col;
			compressed_matrix_col chess(fullhess,c);
			assert(ob->objective!=NULL);
			hess_reverse(ob->objective,vnodes,chess);
			c++;
		}
		//	LOG("full hess now"<<fullhess);

		//take the submatrix of the Hessian declared by intersection of variables declared at this em and emcol.
		typedef boost::numeric::ublas::matrix_range<boost::numeric::ublas::compressed_matrix<double> > compressed_matrix_range;
		uint colrange = colstart + emcol->numLocalVars;
		uint rowrange = rowstart + this->numLocalVars;
		compressed_matrix_range mr(fullhess,range(rowstart,rowrange),range(colstart,colrange));
		block = mr;
		LOG("submatrix -- ["<<this->getNLocalVars()<<"] x ["<<emcol->getNLocalVars()<<"]");
	}
	assert(block.size1()==this->getNLocalVars());
	assert(block.size2()==emcol->getNLocalVars());
	Stat::numObjHessLocalCall++;
	LOG("end obj_hess_local - emrow["<<this->name<<"] emcol"<<emcol->name<<"]");
}

BlockHV* ExpandedModel::getHVBlockLocal(ExpandedModel* emcol)
{
	LOG("enter - getHVBlockLocal - this em["<<this->name<<"] x em["<<emcol->name<<"]");
	assert(this!=emcol || (this->model->level == emcol->model->level && this==emcol)); //the diagonal block
	boost::unordered_map<ExpandedModel*,BlockHV* >::iterator it=hvBlockMap_lo.find(emcol);
	BlockHV* emb = NULL;
	if(it!=hvBlockMap_lo.end()){
		LOG("createConsBlockLocal -- already in Map");
		emb = (*it).second;
	}
	else
	{
		Block* b = this->getBlockLocal();
		emb = new BlockHV(b);
		BOOST_FOREACH(ExpandedModel* em, emb->block->ems)
		{
			BOOST_FOREACH(ConsComp* con, em->model->con_comps)
			{
				if(em->ctx->getCompValue(con) == NULL)
				{
					//now build each constraint in autodiff format
					LOG(" -- con [ "<<con->name<<"]");
					Con* conset = new Con(con->name);
					SyntaxNode* attribute = con->attributes;
					SyntaxNode* indexing = con->indexing;
					if(indexing!=NULL)
					{
						LOG(" -- con index["<<indexing->print()<<"]");
						IndexSet* iset = indexing->createIndexSet(em->ctx);
						assert(iset->dummySetMap.size()==1); //so far support only 1-demonsianl set
						Set* set = iset->dummySetMap.begin()->second;
						string dummy = iset->dummySetMap.begin()->first;
						SetComp* setcomp = iset->dummyCompMap.begin()->second;
						std::vector<string>::iterator itset = set->setValues_data_order.begin();
						for(;itset!=set->setValues_data_order.end();itset++)
						{
							string value = *itset;
							em->ctx->addDummyCompValueMapTemp(dummy,setcomp,value);
							string conName = con->name + "_" + value;
							LOG(" constraint - ["<<conName<<"]");
							//now, build autodiff constraint
							SyntaxNode* assgin_expr = attribute->findChildNode(ASSIGN);
							Node* acon = assgin_expr->buildAutoDiffDAG(em,emcol);
							assert(acon!=NULL);
							double lower = NEG_INFINITY_D;
							double upper = INFINITY_D;
							con->attributes->calculateConsBounds(em->ctx,lower,upper);
							ConSingle* consingle = new ConSingle(acon, lower, upper);
							conset->cons.push_back(consingle);
							em->ctx->removeDummySetValueMapTemp(dummy);
						}
						delete iset;
					}
					else
					{
						LOG(" -- con index is NULL - constraint - ["<<con->name<<"]");
						//now, build autodiff constraint
						double lower = NEG_INFINITY_D;
						double upper = INFINITY_D;
						SyntaxNode* assgin_expr = attribute->findChildNode(ASSIGN);
						Node* acon = assgin_expr->buildAutoDiffDAG(em,emcol);
						assert(acon!=NULL);
						con->attributes->calculateConsBounds(ctx,lower,upper);
						ConSingle* consingle = new ConSingle(acon, lower, upper);
						conset->cons.push_back(consingle);
					}
					em->ctx->addCompValueMap(con,conset);
				}
				assert(em->ctx->getCompValue(con)!=NULL);
			}
		}
		//add the objective into the HVBlock
		//limitation objective function cannot has variables decalared in other level
		//objective declared at difference level will be summed as the total objective
		//this is currently a limitation in the implemenation.
		if(this->model->obj_comp!=NULL)
		{
			SyntaxNode* attribute = this->model->obj_comp->attributes;
			Node* acon = attribute->buildAutoDiffDAG(this,emcol);
			assert(acon!=NULL);
			emb->obj = acon;
		}
		if(GV(logBlock)){
			ostringstream oss;
			oss<<GV(logdir);
			this->getQaulifiedName(oss);
			oss<<"-";
			emcol->getQaulifiedName(oss);
			oss<<".lhvblk";
			ofstream fout(oss.str().c_str());
			emb->logBlock(this,emcol,fout);
		}
		hvBlockMap_lo.insert(pair<ExpandedModel*,BlockHV*>(emcol,emb));
	}
	LOG("exit - getHVBlockLocal - row["<<this->name<<"] X col["<<emcol->name<<"] nvar["<<emcol->numLocalVars<<"]");
	return emb;
}

//createConsBlockLocal
//emcol is passed for the purpose of sum{i in COMM} hack only
//eg. when this is declares the parent level of emcol, and the sum index can be the same of the model index of emcol.
//if this is the case, there is no need to construct the full summation computation graph, instead only the dummy index value
//of the emcol will be used for creating the computation graph.
BlockCons* ExpandedModel::getConsBlockLocal(ExpandedModel* emcol)
{
	LOG("enter - getConsBlockLocal - this em["<<this->name<<"] x em["<<emcol->name<<"]");
	assert(this!=emcol || (this->model->level == emcol->model->level && this==emcol)); //the diagonal block
	boost::unordered_map<ExpandedModel*,BlockCons* >::iterator it=cblockMap_lo.find(emcol);
	BlockCons* emb = NULL;
	if(it!=cblockMap_lo.end()){
		LOG("createConsBlockLocal -- already in Map");
		emb = (*it).second;
	}
	else{
		Block* b = this->getBlockLocal();
		emb = new BlockCons(b);
		BOOST_FOREACH(ConsComp* con, this->model->con_comps)
		{  //now build each constraint in autodiff format
			LOG(" -- con [ "<<con->name<<"]");
			SyntaxNode* attribute = con->attributes;
			SyntaxNode* indexing = con->indexing;
			if(indexing!=NULL)
			{
				LOG(" -- con index["<<indexing->print()<<"]");
				IndexSet* iset = indexing->createIndexSet(ctx);
				assert(iset->dummySetMap.size()==1); //so far support only 1-demonsianl set
				Set* set = iset->dummySetMap.begin()->second;
				string dummy = iset->dummySetMap.begin()->first;
				SetComp* setcomp = iset->dummyCompMap.begin()->second;
				std::vector<string>::iterator itset = set->setValues_data_order.begin();
				for(;itset!=set->setValues_data_order.end();itset++)
				{
					string value = *itset;
					this->ctx->addDummyCompValueMapTemp(dummy,setcomp,value);
					string conName = con->name + "_" + value;
					LOG(" constraint - ["<<conName<<"]");
					//now, build autodiff constraint
					SyntaxNode* assgin_expr = attribute->findChildNode(ASSIGN);
					Node* acon = assgin_expr->buildAutoDiffDAG(this,emcol);
					assert(acon!=NULL);
					emb->cons.push_back(acon);
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
				SyntaxNode* assgin_expr = attribute->findChildNode(ASSIGN);
				Node* acon = assgin_expr->buildAutoDiffDAG(this,emcol);
				assert(acon!=NULL);
				emb->cons.push_back(acon);
			}
		}

		if(GV(logBlock)){
			ostringstream oss;
			oss<<GV(logdir);
			this->getQaulifiedName(oss);
			oss<<"-";
			emcol->getQaulifiedName(oss);
			oss<<".lconsblk";
			ofstream fout(oss.str().c_str());
			emb->logBlock(this,emcol,fout);
		}
		cblockMap_lo.insert(pair<ExpandedModel*,BlockCons*>(emcol,emb));
	}
	LOG("exit - getConsBlockLocal - row["<<this->name<<"] X col["<<emcol->name<<"] -- ncon["<<emb->cons.size()<<"] nvar["<<emcol->numLocalVars<<"]");
	return emb;
}

BlockObj* ExpandedModel::getObjBlockLocal(ExpandedModel* emcol)
{
	LOG("enter - getObjBlockLocal - this em["<<this->name<<"] X em["<<emcol->name<<"]");
	ObjComp* objcomp = this->model->obj_comp;
	assert(objcomp!=NULL);
	assert(this!=emcol || (this->model->level == emcol->model->level && this==emcol)); //the diagonal block
	boost::unordered_map<ExpandedModel*,BlockObj* >::iterator it=oblockMap_lo.find(emcol);
	BlockObj* ob = NULL;
	if(it!=oblockMap_lo.end())
	{
		LOG("constructObjBlockLocal -- already in Map");
		ob = (*it).second;
	}
	else
	{
		Block* b = this->getBlockLocal();
		ob = new BlockObj(b);
		LOG(" -- objcomp [ "<<objcomp->name<<"]");
		SyntaxNode* attribute = objcomp->attributes;
		assert(objcomp->indexing == NULL); // no multi-objective supported yet!
		LOG(" -- con index is NULL");
		string conName = objcomp->name;
		LOG(" constraint - ["<<conName<<"]");
		//now, build autodiff constraint node for object constraint
		Node* obj = attribute->buildAutoDiffDAG(this,emcol);
		ob->objective = obj;

		if(GV(logBlock)){
			ostringstream oss;
			oss<<GV(logdir);
			this->getQaulifiedName(oss);
			oss<<"-";
			emcol->getQaulifiedName(oss);
			oss<<".lobjblk";
			ofstream fout(oss.str().c_str());
			ob->logBlock(this,emcol,fout);
		}
		oblockMap_lo.insert(pair<ExpandedModel*,BlockObj*>(emcol,ob));
	}
	return ob;
	LOG("exit - getObjBlockLocal - row["<<this->name<<"] X col["<<emcol->name<<"] -- nvar["<<emcol->numLocalVars<<"] --- with OBJ["<<this->model->obj_comp->name<<"]");
}

Block* ExpandedModel::getBlockLocal()
{
	LOG("getBlockLocal - em["<<this->name<<"] ");
	if(block_lo == NULL)
	{
		block_lo = new Block();
		std::vector<ExpandedModel*>& ems = block_lo->ems;
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
	assert(block_lo!=NULL);
	return block_lo;
}
/*
 * End of Local Interface methods
 */


/*
 * For both Local and Distribute Interface
 * Access information of this expaned model
 *
 */
void ExpandedModel::get_local_vars_names(std::vector<string>& names)
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
			this->getQaulifiedName(oss);
			oss<<"-"<<var->name<<"_"<<i->indicies;
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
void ExpandedModel::get_local_var_up_bounds(double *elts)
{
	int i = 0;
	std::vector<VarComp*>::iterator ivar=this->model->var_comps.begin();
	for(;ivar!=model->var_comps.end();ivar++)
	{
		Var* var = static_cast<Var*>(ctx->getCompValue(*ivar));
		var_multi_map_by_order& var_by_order = var->varMultiMap.get<0>();
		var_multi_map_by_order::iterator iv = var_by_order.begin();
		for(;iv!=var_by_order.end();iv++)
		{
//			const VarSingle& v = *;
			elts[i] = iv->ub;
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
void ExpandedModel::get_local_var_low_bounds(double *elts)
{
	int i = 0;
	std::vector<VarComp*>::iterator ivar=this->model->var_comps.begin();
	for(;ivar!=model->var_comps.end();ivar++)
	{
		Var* var = static_cast<Var*>(ctx->getCompValue(*ivar));
		var_multi_map_by_order& var_by_order = var->varMultiMap.get<0>();
		var_multi_map_by_order::iterator iv = var_by_order.begin();
		for(;iv!=var_by_order.end();iv++)
		{
			elts[i] = iv->lb;
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
void ExpandedModel::get_local_cons_bounds(double *lower, double *upper)
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
			assert(iset->dummyCompMap.size()==1); // support one dim indexing set only
			SetComp* setcomp = iset->dummyCompMap.begin()->second;
			Set* indset = iset->dummySetMap.begin()->second;
			string dummy = iset->dummySetMap.begin()->first;
			BOOST_FOREACH(string& val, indset->setValues_data_order)
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

void ExpandedModel::get_local_cons_names(std::vector<string>& names)
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
			assert(iset->dummyCompMap.size()==1); // support one dim indexing set only
			SetComp* setcomp = iset->dummyCompMap.begin()->second;
			Set* indset = iset->dummySetMap.begin()->second;
			string dummy = iset->dummySetMap.begin()->first;
			std::vector<string>::iterator itset = indset->setValues_data_order.begin();
			for(;itset!=indset->setValues_data_order.end();itset++)
			{
				ostringstream oss;
				this->getQaulifiedName(oss);
				oss<<"-"<<con->name<<"_"<<*itset;
				names.push_back(oss.str());
			}
			delete iset;
		}
		else
		{
			string conName = this->ctx->getContextId()+"-"+con->name + "_";
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
			static_cast<AutoDiff::VNode*>(i->adv)->val = elts[index];
			index++;
			LOG("update_primal_var_soln - ["<<i->toString()<<"]");
		}
	}
	assert(index==this->getNLocalVars());
}

void ExpandedModel::update_lag(double* elts)
{
	LOG("update_lag() -- ["<<this->name<<"] numOfLocalVar["<<numLocalVars<<"]");
	uint i = 0;
	BOOST_FOREACH(ConsComp* con, this->model->con_comps)
	{
		Con* conset = static_cast<Con*>(this->ctx->getCompValue(con));
		assert(conset!=NULL);
		BOOST_FOREACH(ConSingle* consingle, conset->cons)
		{
			consingle->y = elts[i];
			i++;
		}
	}
	assert(i = this->getNLocalCons());
}


uint ExpandedModel::getNLocalCons()
{
	return this->numLocalCons;
}
uint ExpandedModel::getNLocalVars()
{
	return this->numLocalVars;
}

/*
 * End of common interface methods for both Local and Distribute
 * **********************************************************************************************
 */



/* ***********************************************************************************************
 *  Helper Methods Section
 */

void ExpandedModel::addModelDummy(string dummyVar, ModelComp* comp, string value)
{
	assert(comp!=NULL);
	pair<boost::unordered_map<string,string>::iterator,bool> ret1;
	pair<boost::unordered_map<string,ModelComp*>::iterator,bool> ret2;
	ret1 = this->dummyValueMap.insert(pair<string,string>(dummyVar,value));
	ret2 = this->dummySetMap.insert(pair<string,ModelComp*>(dummyVar,comp));
	assert(ret1.second && ret2.second);
	this->name += "_"+value;
}

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

	std::vector<string> nameCons;
	this->get_local_cons_names(nameCons);
	for(std::vector<string>::iterator it = nameCons.begin();it!=nameCons.end();it++){
		out<<line<<"\t constraint:"<<(*it)<<endl;
	}


	std::vector<string> nameVars;
	this->get_local_vars_names(nameVars);
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

void ExpandedModel::convertToColSparseMatrix(boost::numeric::ublas::compressed_matrix<double>& m,ColSparseMatrix* sm)
{
	LOG("convertToColSparseMatrix - "<<m);
	assert(sm!=NULL);
	boost::numeric::ublas::compressed_matrix<double>::iterator1 it1;
	boost::numeric::ublas::compressed_matrix<double>::iterator2 it2;
//	uint index =0;
//	BOOST_FOREACH(uint i, m.index1_data())
//	{
//		LOG(" "<<i);
//		sm->rownos[index] = i;
//		index ++;
//	}
//	index =0;
//	BOOST_FOREACH(uint i, m.index2_data())
//	{
//		LOG(" "<<i);
//		sm->colstarts[index] = i;
//		index++;
//	}
//	index=0;
//	BOOST_FOREACH(double i, m.value_data())
//	{
//		LOG(" "<<i);
//		sm->values[index] = i;
//		index++;
//	}

	uint i = 0;
	int prev_col = -1;
	int curr_col = 0;
	for(it2=m.begin2();it2!=m.end2();it2++)
	{
		for(it1=it2.begin();it1!=it2.end();it1++)
		{
			LOG(it1.index1()<<","<<it2.index2()<<"="<<*it1);
			curr_col = it2.index2();
			sm->rownos[i] = it1.index1();
			sm->values[i] = *it1;
			if(prev_col==-1) //for leading zero columns
			{
				for(prev_col=0;prev_col<=curr_col;prev_col++)
				{
					sm->colstarts[prev_col] = 0;
					LOG("---- sm->colstarts[prev_col] "<<prev_col<<" - "<<sm->colstarts[prev_col]);
				}
			}
			else
			{
				if(prev_col != curr_col)
				{
					for(prev_col=prev_col+1;prev_col<=curr_col;prev_col++) //for intermediate zero columns
					{
						sm->colstarts[prev_col] = i;
						LOG("sm->colstarts[prev_col] "<<prev_col<<" - "<<sm->colstarts[prev_col]);
					}
				}
			}
			prev_col = curr_col;
			i++;
		}
	}

	for(curr_col=curr_col+1;curr_col<=m.size2();curr_col++) //for ending zero culumns
	{
		sm->colstarts[curr_col] = i;
	}
	assert(curr_col-1 == m.size2());

	for(int j=0;j<i;j++)
	{
		LOG("rowno ["<<sm->rownos[j] <<" value["<<sm->values[j]);
	}
	for(int j=0;j<m.size2()+1;j++){
		LOG("colstarts ["<<sm->colstarts[j]);
	}

	if(sm->collen!=NULL)
	{
		for(int j=0;j<m.size2();j++)
		{
			sm->collen[j] = sm->colstarts[j+1] - sm->colstarts[j];
		}
	}

	for(int j=0;j<m.size2();j++){
		LOG("collen ["<<sm->collen[j]);
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
			vSet.insert(i->adv);
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
			vSet.push_back(i->adv);
//			LOG("copyVariables - ["<<v.toString()<<"]");
		}
	}
}

void ExpandedModel::getQaulifiedName(ostringstream& oss)
{
	oss<<this->name;
	if(parent!=NULL){
		oss<<"_";
		parent->getQaulifiedName(oss);
	}
}

/*
 * Finding the sub-context from it's children problem, so that
 * the dummy varaibles value equals to dummyval
 */
ModelContext* ExpandedModel::locateCtx(AmplModel* model, string& dummyval)
{
	ModelContext* rval = NULL;
	BOOST_FOREACH(ExpandedModel* em, this->children)
	{
		if(em->model == model)
		{
			string dumval = em->dummyValueMap.begin()->second;
			if(dumval.compare(dummyval)==0) {
				rval = em->ctx;
				break;
			}
		}
	}
	//if not found
	if(rval == NULL){
		BOOST_FOREACH(ExpandedModel* em, this->children)
		{
			rval = em->locateCtx(model,dummyval);
			if(rval!=NULL) break;
		}
	}
	assert(rval != NULL);
	return rval;
}
/*
 * End Helper Methods Section
 * ******************************************************************************************
 */



/* * ******************************************************************************************
 * Distribute interface implemenation
 * 		createConsBlockDistributed
 *
 * 		cons_feval_distribute
 * 		cons_nz_jacobs_distribute
 * 		cons_nz_hessian_distribute
 * 		cons_jacobs_distribute
 * 		cons_hessian_distribute
 * 		obj_grad_distribute
 * 		obj_nz_hessian_distribute
 * 		obj_hessian_distribute
 *
*/

Block* ExpandedModel::getBlockDistributed(ExpandedModel* emcol)
{
	LOG("getBlockDistributed - this["<<this->name<<"] - emcol["<<emcol->name<<"]");
	Block* block = NULL;
	boost::unordered_map<ExpandedModel*,Block*>::iterator it = this->block_ds.find(emcol);
	if(it == this->block_ds.end())
	{
		block = new Block();
		//involve emcol's parent and ancestors
		emcol->getParentEM(block->ems);
		//involve emcol itself
		block->ems.push_back(emcol);

		std::vector<ExpandedModel*>& ems = block->ems;
		for(std::vector<ExpandedModel*>::iterator it = ems.begin();it != ems.end();it++) {
			(*it)->recursiveInitContext();
		}
		for(std::vector<ExpandedModel*>::iterator it = ems.begin();it != ems.end();it++) {
			(*it)->model->calculateModelCompRecursive((*it)->ctx);
		}
		for(std::vector<ExpandedModel*>::iterator it = ems.begin();it != ems.end();it++) {
			(*it)->model->calculateLocalVar((*it)->ctx);
		}
		this->block_ds.insert(pair<ExpandedModel*,Block*>(emcol,block));
	}
	else
	{
		LOG("getBlockDistributed -- already initilized ");
		block = it->second;
	}
	return block;
}

BlockCons* ExpandedModel::createConsBlockDistributed(ExpandedModel* emcol)
{
	LOG("createEMBlockDistributed - row["<<this->name<<"] col["<<emcol->name<<"]");
	Block* block_ds = this->getBlockDistributed(emcol);
	BlockCons* emb = new BlockCons(block_ds);

	int col_level = emcol->model->level;
	std::vector<ConsComp*>::iterator it = this->model->con_comps.begin();
	for(;it!=this->model->con_comps.end();it++)
	{  //now build each constraint in autodiff format
		ConsComp* con = *it;
		LOG(" -- con [ "<<con->name<<"]");
		boost::unordered_map<int,SyntaxNode*>::iterator pcon = con->partial.find(col_level);
		SyntaxNode* attribute = NULL;
		if(pcon!=con->partial.end())
		{
			attribute = (*pcon).second;
			LOG(" -- partial attribute ["<<attribute->print()<<"]");
		}

		SyntaxNode* indexing = con->indexing;
		if(indexing!=NULL)
		{
			LOG(" -- con index["<<indexing->print()<<"]");
			IndexSet* iset = indexing->createIndexSet(ctx);
			assert(iset->dummyCompMap.size()==1); //so far support only 1-demonsianl set
			ModelComp* setcomp = iset->dummyCompMap.begin()->second;
			Set* indset = iset->dummySetMap.begin()->second;
			string dummy = iset->dummyCompMap.begin()->first;
			std::vector<string>::iterator itset = indset->setValues_data_order.begin();
			for(;itset!=indset->setValues_data_order.end();itset++)
			{
				string value = *itset;
				this->ctx->addDummyCompValueMapTemp(dummy,setcomp,value);
				string conName = con->name + "_" + value;
				LOG(" constraint - ["<<conName<<"]");
				//now, build autodiff constraint
				AutoDiff::Node* acon = NULL;
				if(attribute!=NULL)
				{
					acon = attribute->buildAutoDiffDAG(this,emcol);
				}
				emb->cons.push_back(acon);
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
			AutoDiff::Node* acon = NULL;
			if(attribute!=NULL)
			{
				acon = attribute->buildAutoDiffDAG(this,emcol);
			}
			emb->cons.push_back(acon);
		}
	}

	assert(this->getNLocalCons()==emb->cons.size());

	if(GV(logBlock)){
		ostringstream oss;
		oss<<GV(logdir);
		this->getQaulifiedName(oss);
		emcol->getQaulifiedName(oss);
		oss<<".dblk";
		ofstream fout(oss.str().c_str());
		emb->logBlock(this,emcol,fout);
	}
	LOG("createEMBlockDistributed - row["<<this->name<<"] col["<<emcol->name<<"] -- ncon["<<emb->cons.size()<<"] nvar["<<emcol->numLocalVars<<"]");
	return emb;
}

void ExpandedModel::cons_jacobs_distribute(ExpandedModel *emcol, std::vector<boost::numeric::ublas::compressed_matrix<double> >& blocks)
{
	LOG("enter cons_jacobs_distribute - emrow["<<this->name<<"] emcol["<<emcol->name<<"]");
	//will need to check the current emrow/emcol level, more than one nz block will be returned

	boost::unordered_map<ExpandedModel*,BlockCons* >::iterator it=cblockMap_ds.find(emcol);
	BlockCons* emb = NULL;
	if(it==cblockMap_ds.end()){
		emb = this->createConsBlockDistributed(emcol);
		cblockMap_ds.insert(pair<ExpandedModel*,BlockCons*>(emcol,emb));
	}
	else{
		emb = (*it).second;
	}

	std::vector<AutoDiff::Node*> vnodes;
	for(std::vector<ExpandedModel*>::iterator it=emb->block->ems.begin();it!=emb->block->ems.end();it++)
	{
		LOG("em["<<(*it)->name);
		(*it)->copyVariables(vnodes);
	}
//	LOG("vnode size -"<<vnodes.size());
	compressed_matrix<double> m(emb->cons.size(),vnodes.size());
	int r = 0;
	for(std::vector<AutoDiff::Node*>::iterator it=emb->cons.begin();it!=emb->cons.end();it++){
		matrix_row<compressed_matrix<double> > rgrad (m,r);
		if(*it!=NULL){
			LOG("con - \n"<<visit_tree(*it));
			AutoDiff::grad_reverse(*it,vnodes,rgrad);
		}
		r++;
//		LOG("jacobian now"<<m);
	}
//	LOG("Full Jacobian: "<<m);

	int prev_c  = 0;
	for(std::vector<ExpandedModel*>::iterator it=emb->block->ems.begin();it!=emb->block->ems.end();it++)
	{
		int num_c = (*it)->getNLocalVars() + prev_c;
		matrix_range<compressed_matrix<double> > mr(m,range(0,m.size1()),range(prev_c,num_c));
		LOG("submatrix -- nvar["<<(*it)->getNLocalVars()<<"] - level["<<(*it)->model->level<<"] em["<<(*it)->name<<"]");
		compressed_matrix<double> subm(mr);
		blocks.push_back(subm);
		prev_c=num_c;
	}
	assert(emcol->model->level+1 == blocks.size());
	assert(blocks.size()==emb->block->ems.size());
	LOG("end cons_jacobs_distribute");
}

void ExpandedModel::cons_hessian_distribute(ExpandedModel* emcol, std::vector<boost::numeric::ublas::compressed_matrix<double> >& blocks)
{
	LOG("enter cons_hessian_distribute - emrow["<<this->name<<"] emcol"<<emcol->name<<"]");
	int nb = (emcol->model->level+2)*(emcol->model->level+1)/2;
	LOG("number of block - "<<nb);

	boost::unordered_map<ExpandedModel*,BlockCons* >::iterator it=cblockMap_ds.find(emcol);
	BlockCons* emb = NULL;
	if(it==cblockMap_ds.end()){
		emb = this->createConsBlockDistributed(emcol);
		cblockMap_ds.insert(pair<ExpandedModel*,BlockCons*>(emcol,emb));
	}
	else{
		emb = (*it).second;
	}

	std::vector<AutoDiff::Node*> vnodes;
	for(std::vector<ExpandedModel*>::iterator it=emb->block->ems.begin();it!=emb->block->ems.end();it++)
	{
		LOG("em["<<(*it)->name);
		(*it)->copyVariables(vnodes);
	}

	boost::numeric::ublas::compressed_matrix<double> fullhess(vnodes.size(),vnodes.size());
	for(std::vector<AutoDiff::Node*>::iterator cit=emb->cons.begin();cit!=emb->cons.end();cit++){
		boost::numeric::ublas::compressed_matrix<double> m(vnodes.size(),vnodes.size());
		for(std::vector<AutoDiff::Node*>::iterator nit=vnodes.begin();nit!=vnodes.end();nit++){
			int c = 0;
			boost::numeric::ublas::matrix_column<boost::numeric::ublas::compressed_matrix<double> > chess(m,c);
			if(*cit!=NULL){
				hess_reverse(*cit,vnodes,chess);
			}
			c++;
		}
//		LOG("hessian curr"<<m);
		fullhess = fullhess + m;
//		LOG("full hess now"<<fullhess);
	}
//	LOG("Full hessian: "<<fullhess);

	int pre_r = 0;
	for(uint i=0;i<emb->block->ems.size();i++)
	{
		int pre_c = pre_r;
		int num_r = pre_r + emb->block->ems.at(i)->getNLocalVars();
		for(uint j=i;j<emb->block->ems.size();j++)
		{
			int num_c = pre_c + emb->block->ems.at(j)->getNLocalVars();
			boost::numeric::ublas::matrix_range<boost::numeric::ublas::compressed_matrix<double> > mr(fullhess,range(pre_r,num_r),range(pre_c,num_c));
//			LOG("row block:"<<mr);
			boost::numeric::ublas::compressed_matrix<double> subm(mr);
			blocks.push_back(mr);
			pre_c = num_c;
		}
		pre_r = num_r;
	}
}

void ExpandedModel::cons_feval_distribute(ExpandedModel *emcol, std::vector<double>& fvals)
{
	LOG("enter cons_feval_distribute - this["<<this->name<<"] emcol["<<emcol->name<<"]");
	assert(fvals.empty());
	boost::unordered_map<ExpandedModel*,BlockCons* >::iterator it=cblockMap_ds.find(emcol);
	BlockCons* emb = NULL;
	if(it==cblockMap_ds.end()){
		emb = this->createConsBlockDistributed(emcol);
		cblockMap_ds.insert(pair<ExpandedModel*,BlockCons*>(emcol,emb));
	}
	else{
		emb = (*it).second;
	}

	int i=0;
	for(std::vector<AutoDiff::Node*>::iterator it=emb->cons.begin();it!=emb->cons.end();it++){
		if(*it==NULL){
			fvals.push_back(0);
		}
		else{
			fvals.push_back(eval_function(*it));
		}
		LOG("fvals["<<i<<"]="<<fvals.at(fvals.size()-1));
		i++;
	}
	LOG("end cons_feval_distribute - this["<<this->name<<"] emcol["<<emcol->name<<"]");
	assert(fvals.size()==this->getNLocalCons());
}

int ExpandedModel::nz_jacobs_distribute(ExpandedModel* emcol, std::vector<unsigned int>& nzs)
{
	LOG("enter nz_jacobs_distribute - emrow["<<this->name<<"] emcol"<<emcol->name<<"]");
	//will need to check the current emrow/emcol level, more than one nz block will be returned
	LOG("number of block - "<<emcol->model->level+1);

	boost::unordered_map<ExpandedModel*,BlockCons* >::iterator it=cblockMap_ds.find(emcol);
	BlockCons* emb = NULL;
	if(it==cblockMap_ds.end()){
		emb = this->createConsBlockDistributed(emcol);
		cblockMap_ds.insert(pair<ExpandedModel*,BlockCons*>(emcol,emb));
	}
	else{
		emb = (*it).second;
	}

	for(std::vector<ExpandedModel*>::iterator it=emb->block->ems.begin();it!=emb->block->ems.end();it++)
	{
		boost::unordered_set<Node*> vSet;
		(*it)->copyVariables(vSet);
		unsigned int nz=0;
		for(std::vector<AutoDiff::Node*>::iterator cit=emb->cons.begin();cit!=emb->cons.end();cit++){
			if(*cit!=NULL){
				nz+=nzGrad(*cit,vSet);
			}
		}
		LOG("Nz for block - "<<nz);
		nzs.push_back(nz);
	}
	assert(emcol->model->level+1 == nzs.size());
	LOG("end nz_jacobs_distribute");
	return nzs.size();
}

/* End of Distribute Interface Implemenation
 **********************************************************************************************
 * */


/* * ******************************************************************************************
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

	for(boost::unordered_map<string,ModelComp*>::iterator it=dummySetMap.begin();it!=dummySetMap.end();it++)
	{
		size_str += sizeof(pair<string,ModelComp*>);
		size_str += (*it).first.size() + 1;
	}
	for(boost::unordered_map<string,string>::iterator it=dummyValueMap.begin();it!=dummyValueMap.end();it++)
	{
		size_str += sizeof(pair<string,string>);
		size_str += (*it).first.size() + 1;
		size_str += (*it).second.size()+ 1;
	}

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
//void ExpandedModel::levelTraversal(std::vector<ExpandedModel*>& em2List,int level)
//{
//	if(level==0)
//	{
//		em2List.push_back(this);
//	}
//	else
//	{
//		for(std::vector<ExpandedModel*>::iterator it=this->children.begin();it!=this->children.end();it++)
//		{
//			ExpandedModel* childEm2 = (*it);
//			childEm2->levelTraversal(em2List,level-1);
//		}
//	}
//}
