/*
 * ExpandedModel2.cpp
 *
 *  Created on: 17 Oct 2011
 *      Author: s0965328
 */

#include "ExpandedModel2.h"
#include "../model/SyntaxNodeIx.h"
#include "../model/SyntaxNodeIDREF.h"
#include "../model/IDNode.h"
#include "../util/global_util_functions.h"
#include "../metric/Statistics.h"
#include "../parser/sml.tab.h"
#include <cassert>
#include <iomanip>
#include <list>
#include <limits>

using namespace std;
using namespace __gnu_cxx;
using namespace AutoDiff;

ExpandedModel2* ExpandedModel2::root = NULL; //initialize root to NULL

ExpandedModel2::ExpandedModel2(AmplModel* mod,ModelContext* context, string dummyVar, ModelComp* comp, string value)
: model(mod),ctx(context),parent(NULL),numLocalVars(0),numLocalCons(0), localVarFilled(false),
  jcobReady(false),rhsReady(false),objComp(NULL),name(mod->name)
{
	if(comp!=NULL)
	{
		pair<__gnu_cxx::hash_map<string,string>::iterator,bool> ret1;
		pair<__gnu_cxx::hash_map<string,ModelComp*>::iterator,bool> ret2;
		ret1 = this->dummyValueMap.insert(pair<string,string>(dummyVar,value));
		ret2 = this->dummySetMap.insert(pair<string,ModelComp*>(dummyVar,comp));
		assert(ret1.second && ret2.second);
	}

	this->name = model->name;
	__gnu_cxx::hash_map<string,string>::iterator it = this->dummyValueMap.begin();
	while(it!=this->dummyValueMap.end())
	{
		this->name += "_"+it->second;
		it++;
	}

	ctx->em2 = this;

	LOG("ExpandedModel2 constructor for name["<<this->name<<"]");
}

ExpandedModel2::~ExpandedModel2() {
	LOG("ExpandedModel2 destructor called !!!");
	LOG("delete context recurisivly!");
	delete ctx;
	dummySetMap.clear();
	dummyValueMap.clear();
	for(int i=0;i<this->children.size();i++)
	{
		delete children.at(i);
	}
	this->children.clear();

	this->localVarComps.clear();
	for(vector<Var*>::iterator it=localVars.begin();it!=localVars.end();it++)
	{
		delete *(it);
	}
	localVars.clear();
	localConComps.clear();
	pvar.clear();
	dvar.clear();
	prow.clear();
	drow.clear();
	rowIndicies.clear();
	values.clear();
	rhss.clear();

}

void ExpandedModel2::addChildren(ExpandedModel2* em2)
{
	this->children.push_back(em2);
	em2->parent = this;
}

void ExpandedModel2::clearAllContextTreeKeepRoot()
{
	if(this!=ExpandedModel2::root)
	{
		delete this->ctx;
		this->ctx = NULL;
	}

	for(vector<ExpandedModel2*>::iterator it=this->children.begin();it!=this->children.end();it++)
	{
		(*it)->clearAllContextTreeKeepRoot();
	}
}

string ExpandedModel2::getName() const
{
	return this->name;
}

void ExpandedModel2::addLocalCon(ModelComp* conComp)
{
	this->localConComps.push_back(conComp);
	if(conComp->indexing==NULL){
		this->numLocalCons++;
	}
	else
	{
		assert(conComp->indexing->opCode == LBRACE);
		assert(conComp->indexing->values.size() == 1); //only one dimensional
		assert(conComp->indexing->values[0]->values[1]->opCode==IDREF);
		assert(conComp->indexing->values[0]->values[0]->opCode==ID);

		ModelComp* setComp = static_cast<SyntaxNodeIDREF*>(conComp->indexing->values[0]->values[1])->ref;
		assert(setComp->setDim==1);
		LOG("dummy ["<<static_cast<IDNode*>(conComp->indexing->values[0]->values[0])->id()<<"]");
		Set* set =static_cast<Set*>(ctx->getCompValue(setComp));
		assert(set->dim == setComp->setDim);
		this->numLocalCons+=set->getCard();
	}
	LOG("addLocalCon -- numLocalCons["<<this->numLocalCons<<"]");
}

void ExpandedModel2::addLocalVar(ModelComp* varComp)
{
	assert(varComp->type == TVAR);
	LOG("insert - varComp["<<(unsigned long)varComp<<"] - varComp["<<varComp->id<<"]");
	this->localVarComps.push_back(varComp);
	this->numLocalVars+=varComp->varCard;
	LOG("addLocalVar -- ["<<this->name<<"] --var size["<<this->localVarComps.size()<<"] numLocalVars["<<this->numLocalVars<<"]");
}

int ExpandedModel2::getNLocalCons() const
{
	return this->numLocalCons;
}
int ExpandedModel2::getNLocalVars() const
{
	return this->numLocalVars;
}

void ExpandedModel2::levelTraversal(vector<ExpandedModel2*>& em2List,int level)
{
	if(level==0)
	{
		em2List.push_back(this);
	}
	else
	{
		for(vector<ExpandedModel2*>::iterator it=this->children.begin();it!=this->children.end();it++)
		{
			ExpandedModel2* childEm2 = (*it);
			childEm2->levelTraversal(em2List,level-1);
		}
	}
}

void ExpandedModel2::fillLocalVar()
{
	LOG("start fillLocalVar -- expand model["<<this->name<<"]");
	assert(this->ctx->isCompsCalculated == true);
	vector<ModelComp*>::iterator it_varComp =this->localVarComps.begin();
	for(;it_varComp!=this->localVarComps.end();it_varComp++)
	{
		(*it_varComp)->fillLocalVar(this);
	}
	this->localVarFilled = true;
	LOG("end fillLocalVar -- ");
}

ModelContext* ExpandedModel2::recursiveInitContext()
{
	ExpandedModel2* parentEm2 = this->parent;
	ModelContext* parentCtx = parentEm2->ctx;

	if(parentCtx==NULL)
	{
		parentCtx = parentEm2->recursiveInitContext();
	}
	this->ctx = new ModelContext(parentCtx);
	this->ctx->em2 = this;
	LOG("recursiveInitContext for ["<<this->name<<"] - contextId["<<this->ctx->getContextId()<<"]");

	return this->ctx;
}

void ExpandedModel2::calcReqLevelsVariableforBlock(ExpandedModel2 *emcol,set<int>& reqLevels)
{
	vector<ModelComp*>::iterator it = this->localConComps.begin();
	for(;it!=this->localConComps.end();it++)
	{
		ModelComp* con = *it;
		assert(con->type == TCON);
		hash_map<int,set<int> >::iterator itVarLevels = con->varDeps.find(emcol->model->level);
		if(itVarLevels!=con->varDeps.end())
		{
			set<int> levels = itVarLevels->second;
			set<int>::iterator it2 = levels.begin();
			for(;it2!=levels.end();it2++)
			{
				if(reqLevels.find(*it2)==reqLevels.end())
				{
					reqLevels.insert(*it2);
				}
			}
		}
	}

	LOG(this->name<<"["<<this->model->level<<"] X "<<emcol->name<<"["<<emcol->model->level<<"]");
	for(set<int>::iterator it=reqLevels.begin();it!=reqLevels.end();it++)
	{
		LOG("   "<<*it);
	}
}

EMBlock* ExpandedModel2::constructEMBlock(ExpandedModel2 * emcol)
{
	LOG("enter constructEMBlock -- ["<<this->ctx->getContextId()<<"] X ["<<emcol->ctx->getContextId()<<"]");

	set<int> reqLevels;
	this->calcReqLevelsVariableforBlock(emcol,reqLevels);
	/* if the reqLevels has level id > emcol->model.level
	 *    it means the intersection block hessian/jacobian is depend on the lower block
	 *    of emcol.
	 *    we will need to expanded all the lower expanded model's local variables in order
	 *    to compute the hessian/jacobian block by assuming semi-separable of siblings.
	 *    Question: can we assume semi-separable of siblings?
	 *    We can use modelling technique to enforce the only one level of connection in this linking constraints
	 *    	by defining variables in it's children model = it's children's children(or even more lower level) model's varaibles.
	 */
	int colLevel = emcol->model->level;
	LOG("emcol ["<<emcol->name<<"] level ["<<colLevel<<"]");

	//first initialize the localvar names for all the depended block
	EMBlock* emb = new EMBlock();
	for(set<int>::iterator it=reqLevels.begin();it!=reqLevels.end();it++)
	{
		int currLevel = *it;
		int level = currLevel - colLevel;
		LOG("get relative level["<<level<<"] to list");
		emcol->initRecursive(level,emb->em2s);
	}

	for(vector<ExpandedModel2*>::iterator it=emb->em2s.begin();it!=emb->em2s.end();it++)
	{
		ExpandedModel2* em = *it;
		em->initAutoDiffVariables(emb);
		assert(em->varToIndMap.size()==em->variables.size());
	}
	LOG("total var in this EMBlock - ["<<emb->totalVars<<"]");
	//now build constraints for this EMBlock
	vector<ModelComp*>::iterator it=this->localConComps.begin();
	for(;it!=this->localConComps.end();it++)
	{
		ModelComp* con = *it;
		this->ctx->moveUpLevel=con->moveUpLevel;
		if(con->indexing!=NULL)
		{
			//assume only one-dim index for constraints declaration
			LOG("handling constraints  -- "<<con->id<<" -- "<<con->indexing->print()<<" -- "<<con->attributes->print());
			assert(con->indexing->opCode == LBRACE);
			assert(con->indexing->values.size()==1);
			assert(con->indexing->values[0]->opCode == IN && con->indexing->values[0]->values.size()==2);
			assert(con->indexing->values[0]->values[1]->opCode == IDREF);
			assert(con->indexing->values[0]->values[0]->opCode == ID);
			SyntaxNodeIDREF* refn = static_cast<SyntaxNodeIDREF*>(con->indexing->values[0]->values[1]);
			IDNode* idn = static_cast<IDNode*>(con->indexing->values[0]->values[0]);
			ModelComp* setComp = refn->ref;
			string dummyVar = idn->id();
			Set* aSet = static_cast<Set*>(this->ctx->getCompValue(setComp));
			vector<string>::iterator it1 = aSet->setValues_data_order.begin();
			LOG("creating constraints node over Set - "<<aSet->toString());
			for(;it1!=aSet->setValues_data_order.end();it1++)
			{
				string dummyVal = (*it1);
				LOG("constraint dummy["<<dummyVar<<" --> "<<dummyVal<<"]");
				this->ctx->addDummySetValueMapCons(dummyVar,setComp,dummyVal);
				Node* node =con->constructAutoDiffCons(this->ctx,emb,emcol);
				LOG(" add constraints -- ["<<con->id<<"] -- "<<con->attributes->print());
				if(node==NULL)
				{
					LOG("      NULL ----------------");
				}
				emb->constraints.push_back(node);

				//constraint name and rhs value
				if(this->rhsReady == false)
				{
					double rhs = con->attributes->evalRhs(this->ctx);
					rhss.push_back(rhs);
					string conName = "";
					conName = this->name+"_"+con->id+"["+dummyVal+"]";
					this->ctx->localConNames.push_back(conName);
					LOG("Create constraint name["<<conName<<"]- with RHS=["<<rhs<<"]");
				}
				//remove dummy set value
				this->ctx->removeDummySetValueMapCons(dummyVar);
			}
		}
		else
		{//no indexing constraint declaration
			LOG("handling constraints  -- "<<con->id<<" -- "<<con->attributes->print());
			Node* node = con->constructAutoDiffCons(this->ctx,emb,emcol);
			LOG(" add constraints -- ["<<con->id<<"]");
			if(node==NULL)
			{
				LOG("      NULL ----------------");
			}
			emb->constraints.push_back(node);

			//constraint name and rhs value
			if(this->rhsReady == false)
			{
				double rhs = con->attributes->evalRhs(this->ctx);
				rhss.push_back(rhs);
				string conName = "";
				conName = this->name+"_"+con->id;
				this->ctx->localConNames.push_back(conName);
				LOG("Create NOINDEXING constraint name["<<conName<<"]- with RHS=["<<rhs<<"]");
			}
		}
	}
	this->rhsReady = true;
	LOG("total constraints in this EMBlock - ["<<emb->constraints.size()<<"]");

	this->emBlocksMap.insert(pair<ExpandedModel2*,EMBlock*>(emcol,emb));
	LOG("exit constructEMBlock --- ["<<this->ctx->getContextId()<<"] X ["<<emcol->ctx->getContextId()<<"]  ---  var["<<emb->totalVars<<"] X cons["<<emb->constraints.size()<<"]");
	return emb;
}

void ExpandedModel2::initAutoDiffVariables(Block* emb)
{
	if(this->variables.size() == 0)
	{
		for(vector<Var*>::iterator it=this->localVars.begin();it!=this->localVars.end();it++)
		{
			vector<string>::iterator it2 = (*it)->indicies.begin();
			for(;it2!=(*it)->indicies.end();it2++)
			{
				Node* v =  create_var_node(numeric_limits<int>::quiet_NaN());
				this->variables.push_back(v);
				string key = (*it)->name+"_"+(*it2);
				LOG("create auto diff var ["<<key<<"] --->             to ["<<this->name<<"]");
				this->varToIndMap.insert(pair<string, Node* >(key,v));
			}
		}
	}
	emb->totalVars += this->variables.size();
	assert(this->variables.size()==this->numLocalVars);
}

void ExpandedModel2::initRecursive(int level, vector<ExpandedModel2*>& em2s)
{
	if(level > 0)
	{
		for(vector<ExpandedModel2*>::iterator it = this->children.begin();it!=this->children.end();it++)
		{
			(*it)->initRecursive(level-1, em2s);
		}
	}
	else if(level < 0)
	{
		this->parent->initRecursive(level+1, em2s);
	}
	else
	{
		LOG("copy this em["<<this->name<<"] to list");
		if(this->ctx->isCompsCalculated==false)
		{
			this->model->calculateModelCompRecursive(this->ctx);
		}
		assert(this->ctx->isCompsCalculated==true);

		if(this->localVarFilled == false)
		{
			this->fillLocalVar();
		}
		assert(this->localVarFilled == true);
		em2s.push_back(this);
	}
}

/* ----------------------------------------------------------------------------
ExpandedModel::getNzJacobianOfIntersection
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
int ExpandedModel2::getNzJacobianOfIntersection(ExpandedModel2 *emcol)
{
	LOG("enter getNzJacobianOfIntersection called -- this["<<this->name<<"] emcol["<<emcol->getName()<<"]");
	assert(jcobReady==false);

	if(emcol->ctx == NULL)
	{
		emcol->recursiveInitContext();
		assert(emcol->ctx!=NULL);
	}
	if(this->ctx == NULL)
	{
		this->recursiveInitContext();
		assert(this->ctx != NULL);
	}

	EMBlock* emb = NULL;
	hash_map<ExpandedModel2*,EMBlock*>::iterator it=this->emBlocksMap.find(emcol);
	if(it==this->emBlocksMap.end()){
		emb = this->constructEMBlock(emcol);
	}
	else
	{
		emb = (*it).second;
	}
	assert(emb!=NULL);
	LOG("EMBlock  num_var["<<emb->totalVars<<"] X num_cons["<<emb->constraints.size()<<"]");

	assert(emcol->ctx->isCompsCalculated == true);
	assert(this->ctx->isCompsCalculated == true);

	//1. ordering variables and assigning id
	emb->settingVariablesId();
	//2. reverse gradient method
	LOG("Gradient evaulation .... for .. this["<<this->name<<"] X emcol["<<emcol->getName()<<"]");
	int nz = 0;
	int row_index = 0;
	vector<Node*>::iterator con_it = emb->constraints.begin();
	for(;con_it!=emb->constraints.end();con_it++)
	{

		Node* con = (*con_it);
		LOG("con"<<row_index <<"----------------");
		if(con != NULL)
		{
			int level = 0;
			con->inorder_visit(level);
			//reset adjoint for each variables
			emb->resetAdj();
			grad_reverse(con);
			emb->outputVariables();

			vector<Node*>::iterator var = emcol->variables.begin();
			int i = 0;
			for(;var!=emcol->variables.end();var++,i++)
			{
				VNode* v = static_cast<VNode*>(*var);
				if(!isnan(v->adj) && v->adj!=0)
				{
					this->rowIndicies[i].push_back(row_index);
					this->values[i].push_back(v->adj);
					LOG("col["<<i<<"] row["<<row_index<<"] grad["<<v->adj<<"]");
					nz++;
				}
			}
		}
		else
		{
			LOG("con"<<row_index <<" is NULL -------");
		}
		row_index++;
	}
	assert(row_index==this->numLocalCons);

	this->jcobReady = true;
	LOG("end getNzJacobianOfIntersection -- this["<<this->name<<"] emcol["<<emcol->getName()<<"]  - Num of Nonzero["<<nz<<"]");
	Statistics::numGetNzJacCall++;

	return nz;
}

void ExpandedModel2::convertToColSparseMatrix(ColSparseMatrix* sm)
{
	assert(jcobReady == true);
	assert(values.size()==rowIndicies.size());
	hash_map<int,vector<double> >::iterator colVal_it = values.begin();
	hash_map<int,vector<int> >::iterator colRowIndex_it = rowIndicies.begin();

	int currCol = 0;
	int preCol = -1;
	int valueIndex = 0;
	for(;colVal_it!=values.end();colVal_it++,colRowIndex_it++)
	{
		currCol =  (*colVal_it).first;
		vector<double> col_values = (*colVal_it).second;
		vector<int> row_indicies = (*colRowIndex_it).second;
		assert(col_values.size()==row_indicies.size());

		if(preCol == -1)
		{
			for(int i=0;i<=currCol;i++)
			{
				sm->colstarts[i] = valueIndex;
				LOG("col_starts["<<i<<"] = "<<valueIndex);
			}
		}
		else
		{
			for(int i=preCol+1;i<=currCol;i++)
			{
				sm->colstarts[i] = valueIndex;
				LOG("col_starts["<<i<<"] = "<<valueIndex);
			}
		}

		vector<double>::iterator col_val_it = col_values.begin();
		vector<int>::iterator row_indicies_it = row_indicies.begin();
		for(;col_val_it!=col_values.end();col_val_it++,row_indicies_it++)
		{
			sm->values[valueIndex] = *col_val_it;
			sm->rownos[valueIndex] = *row_indicies_it;
			LOG(valueIndex<<"col["<<currCol<<"] row["<<sm->rownos[valueIndex]<<"] = "<<sm->values[valueIndex]);

			valueIndex++;
		}

		if(sm->collen != NULL)
		{
			for(int i=preCol+1;i<=currCol;i++)
			{
				if(i==currCol)
				{
					sm->collen[i] = col_values.size();
				}
				else
				{
					sm->collen[i] =0;
				}
				LOG("                           collen["<<i<<"] --- "<<sm->collen[i]);
			}
		}
		preCol = currCol;
	}

	LOG("------ "<< valueIndex<< "-----------");

	rowIndicies.clear();
	values.clear();
	jcobReady = false;
}

/* ----------------------------------------------------------------------------
ExpandedModel::getJacobianOfIntersection
---------------------------------------------------------------------------- */
/** Return the Jacobian of a section of the model in sparse matrix format.
 *
 *  The matrix is defined by the intersection of the local constraints in
 *  this model with the local variables of this or another model.
 *
 *  @param[in] emcol_
 *             The model w.r.t. whose local variables the Jacobian should be
 *             evaluated. This parameter can be NULL, in which case the method
 *             works on the intersection of the local constraints with the
 *             local variables (a "diagonal" block).
 *  @param[out] colbeg
 *             Column starts of the Jacobian.
 *  @param[out] collen
 *             Column lengths of the Jacobian (not returned if NULL on call).
 *  @param[out] rownbs
 *             Row indices of nonzeros entries.
 *  @param[out] el
 *             Values of the nonzero entries.
 *
 *  @note Parameters colbeg, collen, rownbs, el are assumes to be of
 *  appropriate dimensions before the method is called, namely
 *  colbeg[n+1], collen[n], rownbs[nz], el[nz] (n=number of columns,
 *  nz=number nonzeros). The number of nonzeros in this section of the
 *  Jacobian can be obtained from a call to getNzJacobianOfIntersection().
 */
void ExpandedModel2::getJacobianOfIntersection(ExpandedModel2 *emcol_, int *colbeg,
					 int *collen, int *rownbs, double *el)
{
	LOG("enter getJacobianOfIntersection called");
	assert(jcobReady==true);
	ExpandedModel2 *emcol = static_cast< ExpandedModel2* > (emcol_);
	LOG("block demension - row: "<<this->numLocalCons<<"  col: "<<emcol->numLocalVars);


	ColSparseMatrix sm;
	sm.collen = collen;
	sm.rownos = rownbs;
	sm.values = el;
	sm.colstarts = colbeg;

	convertToColSparseMatrix(&sm);

	Statistics::numGetJacCall++;
	LOG("end getJacobianOfIntersection");
}

/* -------------------------------------------------------------------------
ExpandedModel::getObjGradient
-------------------------------------------------------------------------- */
/** Return the gradient of the objective defined in this model.
 *
 *  @param[out] elts
 *              The objective gradient with respect to the local variables.
 */
void ExpandedModel2::getObjGradient(double *elts)
{
	LOG("enter getObjGradient - this["<<this->name<<"]");
	for(int i=0;i<numLocalVars;i++) elts[i] = 0.;

	if(this->objComp != NULL)
	{
		double sign = objComp->type==TMIN? 1.0: -1.0;
		LOG("objComp is defined: name["<<this->objComp->id<<"] type["<<objComp->type<<"]");

		vector<double> jcobs(this->numLocalVars,0.0);
		this->objComp->attributes->evalDiff(ctx,ctx,jcobs);
		assert(jcobs.size()==this->numLocalVars);
		vector<double>::iterator it_jcobs = jcobs.begin();
		for(int i=0;it_jcobs!=jcobs.end();i++,it_jcobs++)
		{
			elts[i] = sign*(*it_jcobs);
			LOG("elts["<<i<<"]="<<elts[i]);
		}
	}
	LOG("end getObjGradient - ");
}

CBlock* ExpandedModel2::getCBlock(ExpandedModel2* emcol)
{
	CBlock* cb = NULL;
	hash_map<ExpandedModel2*, CBlock*>::iterator it = this->cBlockMap.find(emcol);
	if(it==this->cBlockMap.end())
	{//construct cBlock object
		cb = constructCBlock(emcol);
	}
	else
	{
		cb = (*it).second;
	}
	return cb;
}

CBlock* ExpandedModel2::constructCBlock(ExpandedModel2* emcol)
{
	LOG("enter constructCBlock -- ["<<this->ctx->getContextId()<<"] X ["<<emcol->ctx->getContextId()<<"]");

	if(this->objComp == NULL)
	{//case 1: the objective is not defined in this expanded model
		return NULL;
	}

	assert(this->objComp != NULL);
	hash_map<int,set<int> >::iterator itVarLevels = this->objComp->varDeps.find(emcol->model->level);
	if(itVarLevels == this->objComp->varDeps.end())
	{//case 2: no variables declared in emcol are used in objective
		return NULL;
	}

	set<int> levels = (*itVarLevels).second;
	assert(levels.size() != 0); //if emcol is in the objective the dependent level at least be emcol itself.

	CBlock* cb = new CBlock();
	int colLevel = emcol->model->level;
	for(set<int>::iterator it=levels.begin();it!=levels.end();it++)
	{
		int currLevel = *it;
		int level = currLevel - colLevel;
		LOG("get relative level["<<level<<"] to list");
		emcol->initRecursive(level,cb->em2s);
	}
	for(vector<ExpandedModel2*>::iterator it=cb->em2s.begin();it!=cb->em2s.end();it++)
	{
		ExpandedModel2* em = *it;
		em->initAutoDiffVariables(cb);
		assert(em->varToIndMap.size()==em->variables.size());
	}
	LOG("total var in this CBlock - ["<<cb->totalVars<<"]");

	LOG("handling objective  -- "<<this->objComp->id<<" -- "<<this->objComp->attributes->print());
	Node* node = this->objComp->constructAutoDiffCons(this->ctx,cb,emcol);

	if(node==NULL)
	{
		LOG("      NULL ----------------");
		assert(false); //for objective either the emcol doesn't
	}
	cb->objective = node;

	LOG("exit constructCBlock --variable in CBlock ["<<cb->totalVars<<"] ["<<this->ctx->getContextId()<<"] X ["<<emcol->ctx->getContextId()<<"]");
	return cb;
}

void ExpandedModel2::getObjGradient(ExpandedModel2* emcol, CBlock* cb, double* elts)
{
	LOG("enter getObjGradient - this["<<this->name<<"] x emcol["<<emcol->name<<"] variables in CBlock["<<cb->totalVars<<"]");

	if(cb==NULL)
	{
		for(int i=0;i<emcol->numLocalVars;i++) elts[i] = 0;
	}
	else
	{
		assert(this->objComp != NULL);
		cb->settingVariablesId();
		cb->resetAdj();
		int level = 0;
		cb->objective->inorder_visit(level);
		grad_reverse(cb->objective);
		cb->outputVariables();

		vector<Node*>::iterator var = emcol->variables.begin();
		for(int i=0;var!=emcol->variables.end();var++,i++)
		{
			VNode* v = static_cast<VNode*>(*var);
			if(!isnan(v->adj) && v->adj!=0)
			{
				LOG("var_id["<<v->id<<"] var_value["<<v->val<<"] grad["<<v->adj<<"]");
				elts[i] = 0;
			}
			assert(i<emcol->variables.size());
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
void ExpandedModel2::getColUpBounds(double *elts)
{
	int i = 0;
	vector<Var*>::iterator it = this->localVars.begin();
	for(;it!=this->localVars.end();it++)
	{
		Var* aVar = *it;
		vector<string>::iterator it2 = aVar->indicies.begin();
		assert(aVar->indicies.size()==aVar->card);
		for(;it2!=aVar->indicies.end();it2++)
		{
			elts[i]=aVar->ub;
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
void ExpandedModel2::getColLowBounds(double *elts)
{
	int i = 0;
	vector<Var*>::iterator it = this->localVars.begin();
	for(;it!=this->localVars.end();it++)
	{

		Var* aVar = *it;
		vector<string>::iterator it2 = aVar->indicies.begin();
		assert(aVar->indicies.size()==aVar->card);
		for(;it2!=aVar->indicies.end();it2++)
		{
			elts[i]=aVar->lb;
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
 *  The method is simply a wrapper around NlFile::getRowBoundsAMPL().
 */
void ExpandedModel2::getRowBounds(double *lower, double *upper)
{
	LOG("getRowBounds -- ["<<this->getName()<<"]");
	assert(rhsReady == true);
	assert(rhss.size() == this->numLocalCons);
	vector<double>::iterator it = this->rhss.begin();

	for(int i=0;it!=this->rhss.end();it++,i++)
	{
		lower[i]=*it;
		upper[i]=*it;
		LOG(i<<"th row lower=["<<lower[i]<<"] upper=["<<upper[i]<<"]");
	}
	rhss.clear();
	rhsReady == false;
}

/* -------------------------------------------------------------------------
ExpandedModel::setPrimalSolColumns
-------------------------------------------------------------------------- */
/** Upload the local variable solutions */
void ExpandedModel2::setPrimalSolColumns(const double *elts)
{
	LOG("setPrimalSolColumns -- ["<<this->getName()<<"] numOfLocalVar["<<numLocalVars<<"]");

	for (int i=0;i<numLocalVars;i++)
	{
//		LOG(i<<"th var =["<<elts[i]<<"]");
		pvar.push_back(elts[i]);
	}
}

/* -------------------------------------------------------------------------
ExpandedModel::setPrimalSolRows
-------------------------------------------------------------------------- */
/** Upload the local constraints slacks */
void ExpandedModel2::setPrimalSolRows(const double *elts)
{
	LOG("setPrimalSolRows -- ["<<this->getName()<<"] numLocalCons["<<numLocalCons<<"]");

	for (int i = 0; i < numLocalCons; ++i)
	{
		prow.push_back(elts[i]);
	}
}

/* -------------------------------------------------------------------------
ExpandedModel::setDualSolColumns
-------------------------------------------------------------------------- */
/** Upload the local variable duals (multipliers on bounds) */
void ExpandedModel2::setDualSolColumns(const double *elts)
{
	LOG("setDualSolColumns -- ["<<this->getName()<<"] numLocalVars["<<numLocalVars<<"]");
	for (int i = 0; i < numLocalVars; ++i)
	{
		dvar.push_back(elts[i]);
	}
}

/* -------------------------------------------------------------------------
ExpandedModel::setDualSolRows
-------------------------------------------------------------------------- */
/** Upload the local constraints duals (multipliers on constraints) */
void ExpandedModel2::setDualSolRows(const double *elts)
{
	LOG("setDualSolRows -- ["<<this->getName()<<"] numLocalCons["<<numLocalCons<<"]");

	for (int i = 0; i < numLocalCons; ++i)
	{
		drow.push_back(elts[i]);
	}
}

/* -------------------------------------------------------------------------
ExpandedModel::outputSolution
-------------------------------------------------------------------------- */
/** Output the solution to the supplied stream with the given indent */
void ExpandedModel2::outputSolution(ostream &out,int indent)
{
	LOG("outputSolution --- ["<<this->getName()<<"] numOfChildren["<<this->children.size()<<"]");
	assert(this->ctx->localConNames.size() == this->numLocalCons);
	assert(this->ctx->localVarNames.size() == this->numLocalVars);

	string ind(indent, ' ');
	vector<double>::iterator it_pvar=pvar.begin();
	vector<double>::iterator it_dvar=dvar.begin();
	vector<double>::iterator it_prow=prow.begin();
	vector<double>::iterator it_drow=drow.begin();

	list<string>::iterator it_varName =this->ctx->localVarNames.begin();
	list<string>::iterator it_conName =this->ctx->localConNames.begin();

	while(it_varName!=this->ctx->localVarNames.end())
	{
		out<<ind<<left<<setw(20)<<*it_varName<<" Value= "<<setw(15)<<*it_pvar;
		out<<" Reduced cost= "<<*it_dvar;
		out<<endl;

		it_pvar++;
		it_dvar++;
		it_varName++;
	}

	while(it_conName!=this->ctx->localConNames.end())
	{
		out<<ind<<left<<setw(20)<<*it_conName<<" Slack= "<<setw(15)<<*it_prow;
		out<<" Dual= "<<*it_drow;
		out<<endl;

		it_prow++;
		it_drow++;
		it_conName++;
	}

	for(vector<ExpandedModel2*>::iterator i=children.begin();i!=children.end();i++)
	{
		(*i)->outputSolution(out, indent+2);
	}
}

const list<string>& ExpandedModel2::getLocalVarNames() const
{
	return this->ctx->localVarNames;
}

const list<string>& ExpandedModel2::getLocalConNames()const
{
	return this->ctx->localConNames;
}

void ExpandedModel2::calculateMemoryUsage(unsigned long& size_str,unsigned long& size_data)
{
	LOG_MEM("ExpandedModel2::calculateMemoryUsage  -- name["<<name<<"]");
	unsigned long pre = size_data;
	if(ctx!=NULL)
	{
		ctx->calculateMemoryUsage(size_data);
		//cout<<"  "<<context->getContextId()<<endl;
		LOG_MEM(" --- context memory usage ["<<size_data-pre<<"]");
		for(vector<Var*>::iterator it=localVars.begin();it!=localVars.end();it++)
		{
			size_data += sizeof(Var*);
			(*it)->calculateMemoryUsage(size_data);
		}
		size_data += sizeof(ModelComp*)*localVarComps.size();
		size_data += sizeof(ModelComp*)*localConComps.size();
	}

	for(hash_map<string,ModelComp*>::iterator it=dummySetMap.begin();it!=dummySetMap.end();it++)
	{
		size_str += sizeof(pair<string,ModelComp*>);
		size_str += (*it).first.size() + 1;
	}
	for(hash_map<string,string>::iterator it=dummyValueMap.begin();it!=dummyValueMap.end();it++)
	{
		size_str += sizeof(pair<string,string>);
		size_str += (*it).first.size() + 1;
		size_str += (*it).second.size()+ 1;
	}

	size_str += sizeof(ExpandedModel2);
	size_str += name.size() + 1;
	pre = size_str;
	size_str += sizeof(double)*pvar.size();
	size_str += sizeof(double)*dvar.size();
	size_str += sizeof(double)*prow.size();
	size_str += sizeof(double)*drow.size();
	LOG_MEM(" --- solution vector usage ["<<size_str-pre<<"]");

	pre = size_str;
	for(hash_map<int,vector<int> >::iterator it=rowIndicies.begin();it!=rowIndicies.end();it++)
	{
		size_str += sizeof(pair<int,vector<int> >);
		size_str += (*it).second.size()*sizeof(int);
	}
	for(hash_map<int,vector<double> >::iterator it=values.begin();it!=values.end();it++)
	{
		size_str += sizeof(pair<int,vector<double> >);
		size_str += sizeof(double)*(*it).second.size();
	}
	size_str += sizeof(double)*rhss.size();
	LOG_MEM(" --- holder usage ["<<size_str-pre<<"]");

	for(vector<ExpandedModel2*>::iterator it=children.begin();it!=children.end();it++)
	{
		size_str += sizeof(ExpandedModel2*);
		(*it)->calculateMemoryUsage(size_str,size_data);
	}
}
