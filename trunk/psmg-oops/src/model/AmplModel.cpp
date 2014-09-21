/* (c) 2008,2009 Jonathan Hogg and Andreas Grothey, University of Edinburgh
 *
 * This file is part of SML.
 *
 * SML is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, using version 3 of the License.
 *
 * SML is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see http://www.gnu.org/licenses/.
 */

#include "AmplModel.h"
#include "ObjComp.h"
#include "ParamComp.h"
#include "VarComp.h"
#include "ConsComp.h"
#include "SetComp.h"
#include "SyntaxNode.h"
#include "SyntaxNodeID.h"
#include "SyntaxNodeIDREF.h"
#include "../context/ExpandedModel.h"
#include "../context/IndexSet.h"
#include "../context/Set.h"
#include "../context/Var.h"
#include "../st_model/StochModel.h"
#include "../parser/sml.tab.h"
#include "../util/util.h"
#include <typeinfo>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;

//using namespace __gnu_cxx;

//list<changeitem> AmplModel::changes; // initialize to empty list

AmplModel *AmplModel::root = NULL; //initialize root to NULL
int AmplModel::MAX_LEVEL = 0;

/* ---------------------------------------------------------------------------
AmplModel::AmplModel()
---------------------------------------------------------------------------- */
/** Constructor */
AmplModel::AmplModel(const string& id, SyntaxNode* index,AmplModel *par) : ModelComp(id,TMODEL,index,NULL,par),
  parent(par),
  n_vars(0),
  n_cons(0),
  n_params(0),
  n_sets(0),
  n_objs(0),
  n_submodels(0),
  n_total(0),
  level(0),
  obj_comp(NULL), split(false)
{
	level = par==NULL? 0:par->level+1;
	AmplModel::MAX_LEVEL = AmplModel::MAX_LEVEL<level?level:AmplModel::MAX_LEVEL;
	TRACE("AmplModel -- create - name["<<id<<"] -- level["<<level<<"] parent["<<(par==NULL?"NULL":par->name)<<"] MAX_LEVEL now["<<AmplModel::MAX_LEVEL<<"]");
}

/** Destructor */
AmplModel::~AmplModel()
{
	TRACE("AmplModel destructor called...["<<this->name<<"]");
	for(vector<SetComp*>::const_iterator p = set_comps.begin();p != set_comps.end();++p) {
		delete *p;
	}
	for (vector<ParamComp*>::const_iterator p = param_comps.begin();p != param_comps.end(); ++p) {
		delete *p;
	}
	for (vector<ConsComp*>::const_iterator p = con_comps.begin();p != con_comps.end(); ++p) {
		delete *p;
	}
	for (vector<VarComp*>::const_iterator p = var_comps.begin();p != var_comps.end(); ++p) {
		delete *p;
	}
	if( obj_comp!=NULL) {
		delete obj_comp;
	}

	for(vector<AmplModel*>::const_iterator p = subm_comps.begin();p != subm_comps.end();++p) {
		delete *p;
	}

	set_comps.clear();
	param_comps.clear();
	var_comps.clear();
	con_comps.clear();
	subm_comps.clear();
	obj_comp =  NULL;
}

void AmplModel::splitConstraints()
{
	TRACE("formulateConstraints --- model["<<this->name<<"] level["<<level<<"] - split["<<split<<"]");
	if(split) return;

	//we will need to compuate the partial constraint according to each level of intersection model
	//level -1 - indicate the constant only level;
	for(vector<ConsComp*>::iterator it = con_comps.begin();it!=con_comps.end();it++)
	{
		assert((*it)->type==TCON);
		(*it)->calculatePartialConstraints();
	}

	if(obj_comp!=NULL)
	{
		assert(obj_comp->type == TOBJ);
		obj_comp->calculatePartialConstraints();
	}
	split = true;
}

/* ---------------------------------------------------------------------------
AmplModel::createExpandedModel
---------------------------------------------------------------------------- */
ExpandedModel* AmplModel::createExpandedModel(string dummyVar,SetComp* comp,string value,ExpandedModel* par)
{
	if(comp!=NULL){
		TRACE("createExpandedModel- Model["<<this->name<<"]--level["<<this->level<<"] -- set["<<comp->name<<"] -- dummy["<<dummyVar<<"="<<value<<"]");
	}
	else
	{
		TRACE("createExpandedModel- Model["<<this->name<<"]--level["<<this->level<<"] -- only one in this level");
	}
	ExpandedModel *currEm = NULL;
	if(this==AmplModel::root){
		//for root node
		assert(comp==NULL && par == NULL);
		currEm = new ExpandedModel(AmplModel::root,par);
	}
	else
	{
		currEm = new ExpandedModel(this,par);
		if(comp!=NULL)
		{
			currEm->addModelDummy(dummyVar,comp,value);
		}
	}
	ModelContext& currCtx = currEm->ctx;

	//duplicate computation for creating sibling expanded model
	for(vector<SetComp*>::iterator i=set_comps.begin();i!=set_comps.end();i++)
	{
		assert((*i)->type==TSET);
		(*i)->setSetDim();
		TRACE("updateModelComp for set - id["<<(*i)->name<<"] dim["<<(*i)->setDim<<"] -- "<<*i);
	}
	for(vector<ParamComp*>::iterator i=param_comps.begin();i!=param_comps.end();i++)
	{
		assert((*i)->type==TPARAM);
		(*i)->setParamIndicies();
		TRACE("updateModelComp for param - id["<<(*i)->name<<"] numParamIndicies["<<(*i)->numIndicies<<"] - isSym["<<(*i)->isSym<<"]"<<*i);
	}

	if(this==AmplModel::root)
	{
		//comment: any unused character will be throw on stdout from lex
		parse_data(&currCtx);
	}
	//calculating set values
	for(vector<SetComp*>::iterator i=set_comps.begin();i!=set_comps.end();i++)
	{
		SetComp *mp = *i;
		assert(mp->type==TSET);
		if(currCtx.getCompValue(mp) == NULL)
		{
			TRACE("calculateSetModelComp   SET comp["<<mp->name<<"] -- "<<mp);
			mp->calculateSetModelComp(currCtx);
			TRACE("end calculateSetModelComp   -- ["<<mp->name<<"] -- "<<mp);
			assert(currCtx.getCompValue(mp)!=NULL);
		}
	}

	//calculating param values
	for(vector<ParamComp*>::iterator i=param_comps.begin();i!=param_comps.end();i++)
	{
		ParamComp *mp = *i;
		assert(mp->type == TPARAM);
		if(currCtx.getCompValue(mp) == NULL)
		{
			TRACE("caculateParamModelComp    PARAM comp["<<mp->name<<"]");
			mp->calculateParamModelComp(currCtx);
			TRACE("end calculateParamModelComp   -- ["<<mp->name<<"]");
			assert(currCtx.getCompValue(mp)!=NULL);
		}
	}

	//calculating var list
	for(vector<VarComp*>::iterator it=var_comps.begin();it!=var_comps.end();it++)
	{
		VarComp* mc = *it;
		assert(mc->type == TVAR);
		uint card;
		mc->calculateVarDimCard(currCtx, card);
		currEm->numLocalVars += card;
		TRACE("add vars -- EM["<<currEm->name<<"] -- card["<<card<<"] numLocalVars["<<currEm->numLocalVars<<"]");
	}

	//calculating con list
	for(vector<ConsComp*>::iterator it=con_comps.begin();it!=con_comps.end();it++)
	{
		ConsComp* mc = *it;
		assert(mc->type==TCON);
		uint card = 1;
		mc->calculateLocalConCard(currCtx,card);
		currEm->numLocalCons += card;
		TRACE("add cons -- EM["<<currEm->name<<"] -- varcomp["<<card<<"] numLocalCons["<<currEm->numLocalCons<<"]");
	}
//	assert(currEm->y==NULL); currEm->y = new double[currEm->numLocalCons];

	if(obj_comp!=NULL)
	{
		assert(obj_comp->type == TOBJ);
	}

	/*
	 * For Stochastic Model below this model
	 */
	//first convert any stochModel into an AmplModel
	StochModel* smodel = NULL;
	for(vector<AmplModel*>::iterator it=subm_comps.begin();it!=subm_comps.end();)
	{
		if(typeid(**it)==typeid(StochModel))
		{//need to conver to AmplModel first
			assert(smodel == NULL); //only support one stochmodel under root model for now!
			smodel = static_cast<StochModel*>(*it);
			//erease *it from this model and add newmodel to this
			it = subm_comps.erase(it);
			this->n_submodels--;
			this->n_total --;
		}
		else
		{
			it++;
		}
	}
	//now the AmplModel::root is all valid, invalid StochModel is pushed on toConvert vector
	//
	//the newly converted model is attached to the model tree as well to ensure at
	//anytime the AmplModel::root point to a valid up-to-date model tree
	if(smodel!=NULL) {
		AmplModel* stochroot = smodel->convertToAmplModel(currCtx);

		/*
		 * this rootset calculate is not need any more as the new algorithm using compound sets child instead of
		 * a the orignal Parent parameter.
		 */
		//calculate root node set
//		ModelComp* comp = static_cast<SyntaxNodeIDREF*>(stochroot->indexing->values[0]->values[0]->values[0]->values[1])->ref;
//		assert(comp->type==TSET);
//		SetComp* rootset = static_cast<SetComp*>(comp);
//		assert(currCtx.getCompValue(rootset) == NULL);
//		TRACE("stoch root set   SET comp["<<rootset->name<<"] -- "<<rootset);
//		rootset->setSetDim();
//		rootset->calculateSetModelComp(currCtx);
//		TRACE("end calculateSetModelComp   -- ["<<rootset->name<<"] -- "<<rootset);
//		assert(currCtx.getCompValue(rootset)!=NULL);
		/********************************************************************/
		delete smodel;
		smodel = NULL;
	}
	assert(smodel== NULL); //ensure the smodel is delete and not available anymore at this point.
	/*
	 * End Stochastic model pre-processing
	 */

	//Then, create sub expanded models
	for(vector<AmplModel*>::iterator it = subm_comps.begin();it!=subm_comps.end();it++)
	{
		AmplModel* subm = *it;
		subm->splitConstraints();
		assert(subm->type == TMODEL);
		TRACE(" START submodel["<<subm->name<<"] -----   in model["<<name<<"]");
		if(subm->indexing!=NULL)
		{
			TRACE("Indexing over: ["<<subm->indexing<<"]");
			IndexSet* iset = subm->indexing->createIndexSet(currCtx);
			assert(iset->tuples.size()==1); // support only one dummy index for submodel/block declaration for now.

			string dummy = iset->tuples.begin()->dummyVar;
			Set* set = iset->tuples.begin()->set;
			SetComp* setComp = iset->tuples.begin()->setcomp;
			assert(set->dim == 1); //only support one dimensional set for block index set for now!
			TRACE("Name["<<name<<"]'s Comp["<<subm->name<<"] card["<<set->card<<"]");

			vector<string>::iterator it;
			TRACE("creating child  ExpandedModel over the set:"<<set->toString());
			for(it=set->setValues_data_order.begin();it!=set->setValues_data_order.end();it++)
			{
				TRACE("creating child EM - ["<<subm->name<<"] for - set["<<setComp->name<<"] -- dummy["<<dummy<<"="<<*it<<"]");
				ExpandedModel* subem2 = subm->createExpandedModel(dummy,setComp,*it,currEm);
				currEm->addChildren(subem2);
			}
			delete iset;
		}
		else
		{
			TRACE("creating child EM["<<subm->name<<"]  -  only child");
			ExpandedModel* subem2 = subm->createExpandedModel("",NULL,"",currEm);
			currEm->addChildren(subem2);
		}
	}
	TRACE("End createExpandedModel - model["<<this->name<<"]");
	return currEm;
}



void AmplModel::addComp(ModelComp *comp)
{
	TRACE("-- In addComp  -- this model:["<<this->name<<"] level["<<level<<"] add type[" << comp->type<<"] ["<<comp->name<<"]");
	assert(comp->model == this);
	switch (comp->type)
	{
		case TVAR:
			n_vars++;
			var_comps.push_back(static_cast<VarComp*>(comp));
			break;
		case TCON:
			n_cons++;
			con_comps.push_back(static_cast<ConsComp*>(comp));
			break;
		case TSET:
			n_sets++;
			set_comps.push_back(static_cast<SetComp*>(comp));
			break;
		case TPARAM:
			n_params++;
			param_comps.push_back(static_cast<ParamComp*>(comp));
			break;
		case TOBJ:
			assert(obj_comp == NULL); //support only one objective per model
			obj_comp = static_cast<ObjComp*>(comp);
			n_objs++;
			assert(n_objs == 1);
			break;
		case TMODEL:
			n_submodels++;
			subm_comps.push_back(static_cast<AmplModel*>(comp));
			break;
		default:
			cerr << "addComp: type " << comp->type << "unrecognised\n";
			exit(1);
	}
	n_total++;

	TRACE("-- End AddComp [current:"<<this->name<<"]  n_total["<<n_total<<"]");
}

/*
 * Recursively-up findModelComp that has the name matches id
 */
ModelComp* AmplModel::findModelComp(string& id)
{
	ModelComp* rval = NULL;
	//first finding anything in this current model
	for(vector<SetComp*>::const_iterator p = set_comps.begin();p != set_comps.end();++p) {
		if((*p)->name.compare(id)==0)
		{
			rval = *p;
			goto found;
		}
	}
	for (vector<ParamComp*>::const_iterator p = param_comps.begin();p != param_comps.end(); ++p) {
		if((*p)->name.compare(id)==0)
		{
			rval = *p;
			goto found;
		}
	}
	for (vector<VarComp*>::const_iterator p = var_comps.begin();p != var_comps.end(); ++p) {
		if((*p)->name.compare(id)==0)
		{
			rval = *p;
			goto found;
		}
	}
	for(vector<AmplModel*>::const_iterator p = subm_comps.begin();p!=subm_comps.end();++p)
	{
		if((*p)->name.compare(id)==0)
		{
			rval =*p;
			goto found;
		}
	}
	// if first failed , now find in parent
	if(rval == NULL)
	{
		rval = parent==NULL? NULL:parent->findModelComp(id);
	}

	found:
	TRACE("findModelComp -- model["<<this->name<<"] -- id["<<id<<"] - rval["<<rval<<"]");
	return rval;
}

ParamComp* AmplModel::findParamComp(string& id)
{
	ParamComp* rval = NULL;
	for(vector<ParamComp*>::iterator i=param_comps.begin();i!=param_comps.end();i++)
	{
		//TRACE("findModelComp -- model["<<this->name<<"] -- search_for["<<id<<"] at "<<(*i)->id);
		if((*i)->name.compare(id)==0)
		{
			rval = *i;
			break;
		}
	}

	if(rval == NULL)
	{
		rval = parent==NULL? NULL:parent->findParamComp(id);

	}
	TRACE("findParamComp -- model["<<this->name<<"] -- id["<<id<<"] - rval["<<rval<<"]");
	return rval;
}

SetComp* AmplModel::findSetComp(string& id)
{
	SetComp* rval = NULL;
	for(vector<SetComp*>::iterator i=set_comps.begin();i!=set_comps.end();i++)
	{
		//TRACE("findModelComp -- model["<<this->name<<"] -- search_for["<<id<<"] at "<<(*i)->id);
		if((*i)->name.compare(id)==0)
		{
			rval = *i;
			break;
		}
	}

	if(rval == NULL)
	{
		rval = parent==NULL? NULL:parent->findSetComp(id);

	}
	TRACE("findSetComp -- model["<<this->name<<"] -- id["<<id<<"] - rval["<<rval<<"]");
	return rval;
}

//TODO -- currently calculate set first then calculate param,
//	      there will be a problem if a calculated sym param required for creating a set.
void AmplModel::calculateModelComp(ModelContext& context)
{
	TRACE("calculateModelComp - context["<<context.em->name<<"] -- on Model["<<this->name<<"] - ");
	for(vector<SetComp*>::iterator i=set_comps.begin();i!=set_comps.end();i++)
	{
		SetComp *comp = *i;
		assert(comp->type==TSET);
		if(context.getCompValue(comp) == NULL)
		{
			TRACE("calculateSetModelComp   SET comp["<<comp->name<<"] -- "<<comp);
			comp->calculateSetModelComp(context);
			TRACE("end calculateSetModelComp   -- ["<<comp->name<<"] -- "<<comp);
			assert(context.getCompValue(comp)!=NULL);
		}
	}
	for(vector<ParamComp*>::iterator i=param_comps.begin();i!=param_comps.end();i++)
	{
		ParamComp *comp = *i;
		assert(comp->type == TPARAM);
		if( context.getCompValue(comp) == NULL)
		{
			TRACE("caculateParamModelComp    PARAM comp["<<comp->name<<"]");
			comp->calculateParamModelComp(context);
			TRACE("end calculateParamModelComp   -- ["<<comp->name<<"]");
			assert(context.getCompValue(comp)!=NULL);
		}
	}
	TRACE("calculateModelComp - context"<<context.em->name<<"] -- on Model["<<this->name<<"] --DONE");
}

void AmplModel::calculateModelCompRecursive(ModelContext& ctx)
{
	if(this->parent!=NULL)
	{
		this->parent->calculateModelCompRecursive(*(ctx.parent));
	}
	this->calculateModelComp(ctx);
	ctx.used = true;
}



void AmplModel::calculateLocalVar(ModelContext& ctx)
{
	TRACE("calculateLocalVar - context["<<ctx.em->name<<"] -- on Model["<<this->name<<"] - ");
	uint idx = ctx.em->colbeg;
	vector<VarComp*>::iterator it_varComp =this->var_comps.begin();
	for(;it_varComp!=this->var_comps.end();it_varComp++)
	{
		VarComp* comp = *it_varComp;
		assert(comp->type==TVAR);
		Var* var = static_cast<Var*>(ctx.getCompValue(comp));
		if(var==NULL)
		{
			TRACE("calculateLocalVar    VAR comp["<<comp->name<<"]");
			comp->calculateVarComp(idx,ctx);
			TRACE("calculateLocalVar    VAR comp["<<comp->name<<"]");
			assert(ctx.getCompValue(comp)!=NULL);
		}
		else{
			idx += var->varMultiMap.size();
		}
	}
	assert(idx == ctx.em->colbeg + ctx.em->numLocalVars);
	TRACE("calculateLocalVar - context"<<ctx.em->name<<"] -- on Model["<<this->name<<"] --DONE");
}

/* ---------------------------------------------------------------------------
AmplModel::dump(const char *filename)
---------------------------------------------------------------------------- */
void AmplModel::logModel(const char *filename){
  ofstream fout(filename);
  this->dump(fout,0);
}

/* ---------------------------------------------------------------------------
AmplModel::dump(ostream &fout)
---------------------------------------------------------------------------- */
void AmplModel::dump(ostream& fout, int count){
	fout << "AM: ----------------------------------------------------------\n";
	fout << "AM: This is AmplModel (" << (void *) this << "): " << name << "\n";
	fout << "AM: indexing: "<<indexing<<"\n";
	fout << "AM: level: " << level << "\n";
	fout << "AM: parent: " << (parent ? parent->name : "NULL") << "\n";
	fout << "AM: Nb sets       : " << n_sets << "\n";
	fout << "AM: Nb parameters : " << n_params << "\n";
	fout << "AM: Nb variables  : " << n_vars << "\n";
	fout << "AM: Nb constraints: " << n_cons << "\n";
	fout << "AM: Nb objectives : " << n_objs << "\n";
	fout << "AM: Nb submodels  : " << n_submodels << "\n";
	fout << "AM: List components:\n";

	count = 0; //ignor the count passed in and reset counter to 0
	for(vector<SetComp*>::const_iterator p = set_comps.begin();p != set_comps.end();++p) {
		++count;
		(*p)->dump(fout,count);
	}
	for (vector<ParamComp*>::const_iterator p = param_comps.begin();p != param_comps.end(); ++p) {
		++count;
		(*p)->dump(fout, count);
	}
	for (vector<ConsComp*>::const_iterator p = con_comps.begin();p != con_comps.end(); ++p) {
		++count;
		(*p)->dump(fout, count);
	}
	for (vector<VarComp*>::const_iterator p = var_comps.begin();p != var_comps.end(); ++p) {
		++count;
		(*p)->dump(fout, count);
	}
	if( obj_comp!=NULL) {
		++count;
		obj_comp->dump(fout, count);
	}

	for(vector<AmplModel*>::const_iterator p = subm_comps.begin();p != subm_comps.end();++p) {
		++count;
		AmplModel *mc = *p;
		mc->dump(fout,count);
	}
	assert(count == this->n_total);
}



void AmplModel::calculateMemoryUsage(unsigned long& size)
{
	TRACE("AmplModel::calculateMemoryUsage -- model["<<this->name<<"]");
	ModelComp::calculateMemoryUsage(size);
	size += sizeof(AmplModel);

	for(vector<SetComp*>::const_iterator p = set_comps.begin();p != set_comps.end();++p) {
		size += sizeof(SetComp*);
		(*p)->calculateMemoryUsage(size);;
	}
	for (vector<ParamComp*>::const_iterator p = param_comps.begin();p != param_comps.end(); ++p) {
		size += sizeof(ParamComp*);
		(*p)->calculateMemoryUsage(size);
	}
	for (vector<ConsComp*>::const_iterator p = con_comps.begin();p != con_comps.end(); ++p) {
		size += sizeof(ConsComp*);
		(*p)->calculateMemoryUsage(size);
	}
	for (vector<VarComp*>::const_iterator p = var_comps.begin();p != var_comps.end(); ++p) {
		size += sizeof(VarComp*);
		(*p)->calculateMemoryUsage(size);
	}
	if( obj_comp!=NULL) {
		obj_comp->calculateMemoryUsage(size);
	}

	for(vector<AmplModel*>::const_iterator p = subm_comps.begin();p != subm_comps.end();++p) {
		size += sizeof(AmplModel*);
		(*p)->calculateMemoryUsage(size);
	}
}

