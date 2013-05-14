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

using namespace std;
using namespace __gnu_cxx;

ExpandedModel2::ExpandedModel2(AmplModel* _mod,ExpandedModel2* _parent,ModelContext* _context)
: model(_mod),context(_context),numLocalVars(0),numLocalCons(0),atRank(0),localVarFilled(0),
  jcobReady(false),rhsReady(false),objComp(NULL),name(_mod->name)
{
	parent = _parent;
	context->em = this;
	this->updateName();
	LOG("ExpandedModel2 constructor for name["<<this->name<<"]");
}

ExpandedModel2::~ExpandedModel2() {
	LOG("ExpandedModel2 destructor called !!!");
	LOG("delete context recurisivly!");
	delete context;
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
	localCons.clear();
	pvar.clear();
	dvar.clear();
	prow.clear();
	drow.clear();
	rowIndicies.clear();
	values.clear();
	rhss.clear();
}

void ExpandedModel2::updateName()
{
	this->name = model->name;
	__gnu_cxx::hash_map<string,string>::iterator it = this->context->dummyValueMap.begin();
	while(it!=this->context->dummyValueMap.end())
	{
		this->name += "_"+it->second;
		it++;
	}
	LOG("ExpandedModel2 - updateName -  name["<<this->name<<"]");
}

string ExpandedModel2::getName() const
{
	return this->name;
}

void ExpandedModel2::addLocalCon(ModelComp* conComp)
{
	this->localCons.push_back(conComp);
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
		Set* set =static_cast<Set*>(context->getCompValue(setComp));
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
	LOG("start fillLocalVar -- ");
	assert(this->context->isCompsCalculated == true);
	if(!this->localVarFilled)
	{
		vector<ModelComp*>::iterator it_varComp =this->localVarComps.begin();
		for(;it_varComp!=this->localVarComps.end();it_varComp++)
		{
			(*it_varComp)->fillLocalVar(this);
		}
		this->localVarFilled = true;
	}
	LOG("end fillLocalVar -- ");
}

ModelContext* ExpandedModel2::recursiveInitContext()
{
	if(this->context!=NULL)
	{
		//do nothing! - already root context
		LOG("ExpandedModel2 - recursiveInitContext  name["<<this->name<<"]");
		LOG("ExpandedModel2 - recursiveInitContext - contextId["<<this->context->getContextId()<<"]");
	}
	else if(this->context == NULL)
	{
		ExpandedModel2* parent_em2 = static_cast<ExpandedModel2*>(this->parent);
		ModelContext* parent_ctx = parent_em2->recursiveInitContext();
		this->context = new ModelContext(parent_ctx);
		this->context->em = this;
		LOG("ExpandedModel2 - recursiveInitContext - just create contextId["<<this->context->getContextId()<<"]");
	}
	return this->context;
}

bool ExpandedModel2::isParent(ExpandedModel2* em2)
{
	ExpandedModel2* emParent = static_cast<ExpandedModel2*>(em2->parent);
	if(this==emParent)
	{
		return true;
	}
	if(em2->parent == NULL)
	{
		return false;
	}
	return isParent(emParent);
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
int ExpandedModel2::getNzJacobianOfIntersection(ExpandedModel2 *emcol_)
{
	LOG_SYS_MEM("beforeGetNz");
	LOG("enter getNzJacobianOfIntersection called -- this["<<this->name<<"] emcol["<<emcol_->getName()<<"]");
	assert(jcobReady==false);
	ExpandedModel2* emrow = static_cast<ExpandedModel2*>(this);
	emrow->context->recursiveMarkContextUsed();
	ExpandedModel2 *emcol = static_cast< ExpandedModel2* > (emcol_);
	emcol->context->recursiveMarkContextUsed();
	if(!emcol->context->isCompsCalculated)
	{
		emcol->model->calculateModelCompRecursive(emcol->context);
		emcol->context->isCompsCalculated = true;
	}
	if(!this->context->isCompsCalculated)
	{
		this->model->calculateModelCompRecursive(this->context);
		this->context->isCompsCalculated = true;
	}
	emcol->fillLocalVar();

	int rval = 0;
	int row = 0;
	vector<ModelComp*>::iterator it3 = this->localCons.begin();
	for(;it3!=this->localCons.end();it3++)
	{
		ModelComp* conComp = (*it3);
		LOG("handling constraint ["<<conComp->id<<"]");

		this->context->moveUpLevel=conComp->moveUpLevel;
		if(conComp->indexing)
		{
			ModelComp* setComp = conComp->indexing->sets_mc[0];
			assert(conComp->indexing->getDummyVarExpr(0)->opCode==258); //ID=258
			string dummyVar = static_cast<IDNode*>(conComp->indexing->getDummyVarExpr(0))->id();
			Set* aSet = static_cast<Set*>(this->context->getCompValue(setComp));
			vector<string>::iterator it4 = aSet->setValues_data_order.begin();
			LOG("constraints over Set - "<<aSet->toString());
			for(;it4!=aSet->setValues_data_order.end();it4++)
			{
				string dummyVal = (*it4);
				LOG("constraint dummy["<<dummyVar<<" --> "<<dummyVal<<"]");
				this->context->addDummySetValueMapCons(dummyVar,setComp,dummyVal);

				//vector operation version start
				vector<double> jcobs(emcol->numLocalVars,0.0); //vector operation
				conComp->attributes->evalDiff(this->context,emcol->context,jcobs);
				vector<Var*>::iterator it_var = emcol->localVars.begin();
				vector<string>::iterator it_varList = (*it_var)->indicies.begin();

				vector<double>::iterator it_jcobs = jcobs.begin();
				assert(jcobs.size()==emcol->numLocalVars);
				for(int col=0;it_jcobs!=jcobs.end();it_jcobs++,col++)
				{
					if((*it_jcobs)!=0)
					{
						rowIndicies[col].push_back(row);
						values[col].push_back(*it_jcobs);
						LOG("filling nonz -- ["<<col<<","<<row<<"]="<<*it_jcobs);
						rval++;
					}
					if(emcol->context->varNameReady == false)
					{
						string& varKey = (*it_varList);
						string varName = emcol->name+"_"+(*it_var)->name+"["+varKey+"]";
						emcol->context->localVarNames.push_back(varName);

						//increment.
						it_varList++;
						if(it_varList==(*it_var)->indicies.end())
						{
							it_var++;
							it_varList=it_var==emcol->localVars.end()?it_varList:(*it_var)->indicies.begin();
						}
					}
				}
				assert(((emcol->context->varNameReady==false && (it_var==emcol->localVars.end()))
					||(emcol->context->varNameReady==true && (it_var==emcol->localVars.begin()))));
				emcol->context->varNameReady = true;
				//vector operation version end

				//for each constraints
				//1. evaluate the right hand side
				//2. calculate a constraint name
				if(rhsReady == false)
				{
					double rhs = conComp->attributes->evalRhs(this->context);
					LOG("indexing contraints rhss = ["<<rhs<<"]");
					rhss.push_back(rhs);
				}
				if(this->context->conNameReady == false)
				{
					string conName = "";
					conName = this->name+"_"+conComp->id+"["+dummyVal+"]";
					this->context->localConNames.push_back(conName);
				}

				this->context->removeDummySetValueMapCons(dummyVar);
				row++;
			}
		}
		else
		{
			//start vector version of autodiff
			vector<double> jcobs(emcol->numLocalVars,0.0); //vector operation
			assert(jcobs.size()==emcol->numLocalVars);
			conComp->attributes->evalDiff(this->context,emcol->context,jcobs);
			vector<Var*>::iterator it_var = emcol->localVars.begin();
			vector<string>::iterator it_varList = (*it_var)->indicies.begin();
			vector<double>::iterator it_jcobs = jcobs.begin();
			assert(jcobs.size()==emcol->numLocalVars);
			for(int i=0;it_jcobs!=jcobs.end();it_jcobs++,i++)
			{
				if((*it_jcobs)!=0)
				{
					rowIndicies[i].push_back(row);
					values[i].push_back(*it_jcobs);
					LOG("filling nonz - ["<<i<<","<<row<<"]="<<*it_jcobs);
					rval++;
				}
				if(emcol->context->varNameReady == false)
				{
					//string varKey = it_vmap->second->getVarKey();
					string& varKey = (*it_varList);
					string varName = emcol->name+"_"+(*it_var)->name+"["+varKey+"]";
					emcol->context->localVarNames.push_back(varName);

					//increment.
					it_varList++;
					if(it_varList==(*it_var)->indicies.end())
					{
						it_var++;
						it_varList=it_var==emcol->localVars.end()?it_varList:(*it_var)->indicies.begin();
					}
				}
			}
			assert(((emcol->context->varNameReady==false && (it_var==emcol->localVars.end()))
					||(emcol->context->varNameReady==true && (it_var==emcol->localVars.begin()))));
			emcol->context->varNameReady = true;
			//end vector version of autodiff

			//for each constraints
			//1. evaluate the right hand side
			//2. calculate a constraint name
			if(rhsReady == false)
			{
				double rhs = conComp->attributes->evalRhs(this->context);
				LOG("no indexing constraint - rhss = ["<<rhs<<"]");
				rhss.push_back(rhs);
			}
			if(this->context->conNameReady == false)
			{
				string conName = "";
				conName = this->name+"_"+conComp->id;
				this->context->localConNames.push_back(conName);
			}
			row++;
		}
	}
	assert(row == this->numLocalCons);

	LOG("end getNzJacobianOfIntersection -- this["<<this->name<<"] emcol["<<emcol_->getName()<<"]  - rval["<<rval<<"]");
	jcobReady = true;
	rhsReady = true;
	this->context->conNameReady = true;


	Statistics::numGetNzJacCall++;
	LOG_SYS_MEM("AfterGetNz");

	return rval;
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
	LOG_SYS_MEM("beforeGetJac");
	LOG("enter getJacobianOfIntersection called");
	assert(jcobReady==true);
	ExpandedModel2 *emcol = static_cast< ExpandedModel2* > (emcol_);
	LOG("block demension - row: "<<this->numLocalCons<<"  col: "<<emcol->numLocalVars);

	int nextCol = 0;
	int currCol = 0;
	int totalValue = 0;

	hash_map<int,vector<double> >::iterator colVal_it = values.begin();
	hash_map<int,vector<int> >::iterator colRowIndex_it = rowIndicies.begin();
	assert(values.size()==rowIndicies.size());

	LOG("values size : "<<values.size());
	LOG("rowIndicies size : "<<rowIndicies.size());
	for(;colVal_it!=values.end();colVal_it++,colRowIndex_it++)
	{

		nextCol = colVal_it->first;
		for(int i=currCol;i<nextCol;i++)
		{
			collen[i] = 0;
			colbeg[i] = totalValue;
			LOG("I collen["<<i<<"]=0");
			LOG("I colbeg["<<i<<"]="<<totalValue);
		}
		collen[nextCol] = colVal_it->second.size();
		colbeg[nextCol] = totalValue;
		LOG("R collen["<<nextCol<<"]="<<colVal_it->second.size());
		LOG("R colbeg["<<nextCol<<"]="<<totalValue);
		vector<double> colValues = colVal_it->second;
		vector<int> rowIndx = colRowIndex_it->second;
		vector<double>::iterator it3 = colValues.begin();
		vector<int>::iterator it4 = rowIndx.begin();
		for (; it3 != colValues.end(); it3++, it4++)
		{
			el[totalValue] = *it3;
			rownbs[totalValue] = *it4;
			LOG("el["<<totalValue<<"]="<<*it3);
			LOG("rownbs["<<totalValue<<"]="<<*it4);
			totalValue++;
		}
		assert( it3==colValues.end() );
		assert( it4==rowIndx.end() );
		currCol = nextCol+1;
	}

	for(nextCol=currCol;nextCol<emcol->numLocalVars;nextCol++)
	{
		collen[nextCol] = 0;
		colbeg[nextCol] = totalValue;
		LOG("P collen["<<nextCol<<"]=0");
		LOG("P colbeg["<<nextCol<<"]="<<totalValue);
	}
	assert(nextCol==emcol->numLocalVars);
	colbeg[nextCol] = totalValue;
	LOG("E colbeg["<<nextCol<<"]="<<totalValue);

	LOG("currCol: "<<nextCol);
	LOG("totalValue: "<<totalValue);

	assert(totalValue == colbeg[emcol->numLocalVars-1] + collen[emcol->numLocalVars-1]);

	rowIndicies.clear();
	values.clear();
	jcobReady = false;


	//checking up code begin
//	ostringstream oss;
//	for(int n=0;n<emcol->numLocalVars;n++)
//	{
//		oss<<collen[n]<<",";
//	}
//	LOG("checking... collen["<<oss.str());
//	oss.str("");
//	for(int n=0;n<=emcol->numLocalVars;n++)
//	{
//		oss << colbeg[n] << ",";
//	}
//	LOG("checking... colbeg["<<oss.str());
//	oss.str("");
//	for(int nz=0;nz<totalValue;nz++)
//	{
//		oss <<rownbs[nz] <<",";
//	}
//	LOG("checking... rownbs["<<oss.str());
//	oss.str("");
//	for(int nz=0;nz<totalValue;nz++)
//	{
//		oss <<el[nz] <<",";
//	}
//	LOG("checking... el["<<oss.str());
//	oss.str("");
	//checking up code done
	Statistics::numGetJacCall++;
	LOG("end getJacobianOfIntersection");
	LOG_SYS_MEM("afterGetJac");
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
		this->objComp->attributes->evalDiff(context,context,jcobs);
		assert(jcobs.size()==this->numLocalVars);
		vector<double>::iterator it_jcobs = jcobs.begin();
		for(int i=0;it_jcobs!=jcobs.end();i++,it_jcobs++)
		{
			elts[i] = sign*(*it_jcobs);
			LOG("elts["<<i<<"]="<<elts[i]);
		}

//		int i = 0;
//		vector<Var*>::iterator it = this->localVars.begin();
//		vector<ModelComp*>::iterator it_1 = this->localVarComps.begin();
//		while(it!=this->localVars.end())
//		{
//			Var* aVar = *it;
//			hash_map<string,VarValue*>::iterator it2=aVar->varMap.begin();
//			while(it2!=aVar->varMap.end())
//			{
//				string varKey= it2->second->getVarKey();
//				ModelComp* varComp = *it_1;
//				elts[i] = sign*this->objComp->attributes->evalDiff(context,context,varComp,varKey);
//				LOG("elts["<<i<<"]="<<elts[i]);
//				i++;
//				it2++;
//			}
//			it++;
//			it_1++;
//		}
//		assert(i==this->numLocalVars);
	}
	LOG("end getObjGradient - ");
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
	LOG("["<<this->getName()<<"]");
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
	assert(this->context->localConNames.size() == this->numLocalCons);
	assert(this->context->localVarNames.size() == this->numLocalVars);

	string ind(indent, ' ');
	vector<double>::iterator it_pvar=pvar.begin();
	vector<double>::iterator it_dvar=dvar.begin();
	vector<double>::iterator it_prow=prow.begin();
	vector<double>::iterator it_drow=drow.begin();

	list<string>::iterator it_varName =this->context->localVarNames.begin();
	list<string>::iterator it_conName =this->context->localConNames.begin();

	while(it_varName!=this->context->localVarNames.end())
	{
		out<<ind<<left<<setw(20)<<*it_varName<<" Value= "<<setw(15)<<*it_pvar;
		out<<" Reduced cost= "<<*it_dvar;
		out<<endl;

		it_pvar++;
		it_dvar++;
		it_varName++;
	}

	while(it_conName!=this->context->localConNames.end())
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
	return this->context->localVarNames;
}

const list<string>& ExpandedModel2::getLocalConNames()const
{
	return this->context->localConNames;
}

void ExpandedModel2::calculateMemoryUsage(unsigned long& size_str,unsigned long& size_data)
{
	LOG_MEM("ExpandedModel2::calculateMemoryUsage  -- name["<<name<<"]");
	unsigned long pre = size_data;
	if(context!=NULL && context->isUsed==true)
	{
		context->calculateMemoryUsage(size_data);
		context->isUsed = false;
		//cout<<"  "<<context->getContextId()<<endl;
		LOG_MEM(" --- context memory usage ["<<size_data-pre<<"]");
		for(vector<Var*>::iterator it=localVars.begin();it!=localVars.end();it++)
		{
			size_data += sizeof(Var*);
			(*it)->calculateMemoryUsage(size_data);
		}
		size_data += sizeof(ModelComp*)*localVarComps.size();
		size_data += sizeof(ModelComp*)*localCons.size();
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
