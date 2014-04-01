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
#include "ModelComp.h"
#include "SyntaxNode.h"
#include "SyntaxNodeID.h"
#include "SyntaxNodeIDREF.h"
#include "changeitem.h"
#include "../context/ExpandedModel.h"
#include "../sml/GlobalVariables.h"
#include "../util/global_util_functions.h"
#include "../parser/sml.tab.h"
#include <cassert>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;

using namespace __gnu_cxx;

//list<changeitem> AmplModel::changes; // initialize to empty list

AmplModel *AmplModel::root = NULL; //initialize root to NULL
int AmplModel::MAX_LEVEL = 0;

/* ---------------------------------------------------------------------------
AmplModel::AmplModel()
---------------------------------------------------------------------------- */
/** Constructor */
AmplModel::AmplModel(const string& id, AmplModel *par, SyntaxNode* index) : ModelComp(id,TMODEL,index,NULL),
  parent(par),
  n_vars(0),
  n_cons(0),
  n_params(0),
  n_sets(0),
  n_objs(0),
  n_submodels(0),
  n_total(0),
  level(0),
  obj_comp(NULL)
{
	LOG("AmplModel -- create - name["<<id<<"] -- parent["<<(par==NULL?"NULL":par->name)<<"]");
}

/** Destructor */
AmplModel::~AmplModel()
{
	LOG("AmplModel destructor called...["<<this->name<<"]");
	for (vector<ModelComp*>::iterator it = this->all_comps.begin(); it != all_comps.end(); it++)
	{
		ModelComp* mc = *it;
		delete mc;
	}

	set_comps.clear();
	param_comps.clear();
	var_comps.clear();
	con_comps.clear();
	subm_comps.clear();
	obj_comp =  NULL;

//	changes.clear();

}

void AmplModel::settingUpLevels(int lev)
{
	LOG("settingUpLevels -- model["<<this->name<<"] level["<<lev<<"]");
	this->level = lev;
	AmplModel::MAX_LEVEL = AmplModel::MAX_LEVEL<lev?lev:AmplModel::MAX_LEVEL;
	vector<AmplModel*>::const_iterator it=this->subm_comps.begin();

	for(;it!=this->subm_comps.end();it++)
	{
		LOG("submodel comp["<<(*it)->name<<"]");
		(*it)->settingUpLevels(lev+1);
	}
}

void AmplModel::formulateConstraints()
{
	LOG("formulateConstraints --- model["<<this->name<<"] level["<<level<<"]");

	for(vector<ConsComp*>::iterator it = con_comps.begin();it!=con_comps.end();it++)
	{
		assert((*it)->type==TCON);

		//this method is not yet tested
		// to move the parameter to righ-hand-side and move variables to left-hand-side
		// so the RHS can be evaluated as a constant upper-lower bound for the constraint
		//(*it)->moveConsToLeft();

		//for distribute Interface call - we will need to compuate the partial constraint according to each level of intersection model
		//level -1 - indicate the constant only level;
//		(*it)->calculatePartialConstraints();   //TODO not yet tested !
	}

	if(obj_comp!=NULL)
	{
		assert(obj_comp->type == TOBJ);
		//for distribute Interface call - we will need to compuate the partial constraint according to each level of intersection model
		//level -1 - indicate the constant only level;
//		obj_comp->calculatePartialConstraints();   //TODO not yet tested !
	}

	for(vector<AmplModel*>::iterator it=subm_comps.begin();it!=subm_comps.end();it++)
	{
		assert((*it)->type == TMODEL);
		(*it)->formulateConstraints();
	}
}

/* ---------------------------------------------------------------------------
AmplModel::createExpandedModel
---------------------------------------------------------------------------- */
ExpandedModel* AmplModel::createExpandedModel(string dummyVar,SetComp* comp,string value,ModelContext* parent)
{
	if(comp!=NULL){
		LOG("createExpandedModel- Model["<<this->name<<"]--level["<<this->level<<"] -- set["<<comp->name<<"] -- dummy["<<dummyVar<<"="<<value<<"]");
	}
	else
	{
		LOG("createExpandedModel- Model["<<this->name<<"]--level["<<this->level<<"] -- only one in this level");
	}
	ModelContext* currCtx = new ModelContext(parent);
	ExpandedModel *currEm2 = NULL;
	if(this==AmplModel::root){
		//for root node
		currEm2 = new ExpandedModel(AmplModel::root,currCtx);
		assert(comp==NULL);
	}
	else
	{
		currEm2 = new ExpandedModel(this,currCtx);
		if(comp!=NULL)
		{
			currEm2->addModelDummy(dummyVar,comp,value);
		}
	}

	for(vector<SetComp*>::iterator i=set_comps.begin();i!=set_comps.end();i++)
	{
		assert((*i)->type==TSET);
		(*i)->setSetDim();
		LOG("updateModelComp for set - id["<<(*i)->name<<"] dim["<<(*i)->setDim<<"] -- "<<*i);
	}
	for(vector<ParamComp*>::iterator i=param_comps.begin();i!=param_comps.end();i++)
	{
		assert((*i)->type==TPARAM);
		(*i)->setParamIndicies();
		LOG("updateModelComp for param - id["<<(*i)->name<<"] numParamIndicies["<<(*i)->getNumSetIndices()<<"] - isSym["<<(*i)->isSym<<"]"<<*i);
	}

	if(this==AmplModel::root)
	{
		parse_data(currCtx);
	}

	//calculating set values
	for(vector<SetComp*>::iterator i=set_comps.begin();i!=set_comps.end();i++)
	{
		SetComp *mp = *i;
		assert(mp->type==TSET);
		if(currCtx->getCompValue(mp) == NULL)
		{
			LOG("calculateSetModelComp   SET comp["<<mp->name<<"] -- "<<mp);
			mp->calculateSetModelComp(currCtx);
			LOG("end calculateSetModelComp   -- ["<<mp->name<<"] -- "<<mp);
			assert(currCtx->getCompValue(mp)!=NULL);
		}
	}

	//calculating param values
	for(vector<ParamComp*>::iterator i=param_comps.begin();i!=param_comps.end();i++)
	{
		ParamComp *mp = *i;
		assert(mp->type == TPARAM);
		if(currCtx->getCompValue(mp) == NULL)
		{
			LOG("caculateParamModelComp    PARAM comp["<<mp->name<<"]");
			mp->calculateParamModelComp(currCtx);
			LOG("end calculateParamModelComp   -- ["<<mp->name<<"]");
			assert(currCtx->getCompValue(mp)!=NULL);
		}
	}

	//calculating var list
	for(vector<VarComp*>::iterator it=var_comps.begin();it!=var_comps.end();it++)
	{
		VarComp* mc = *it;
		assert(mc->type == TVAR);
		mc->calculateVarDimCard(currCtx);
		currEm2->numLocalVars += mc->card;
		LOG("add vars -- EM["<<currEm2->name<<"] -- varcomp["<<mc->card<<"] numLocalVars["<<currEm2->numLocalVars<<"]");
	}

	//calculating con list
	for(vector<ConsComp*>::iterator it=con_comps.begin();it!=con_comps.end();it++)
	{
		ConsComp* mc = *it;
		assert(mc->type==TCON);
		mc->calculateLocalCon(currCtx);
		currEm2->numLocalCons += mc->card;
		LOG("add cons -- EM["<<currEm2->name<<"] -- varcomp["<<mc->card<<"] numLocalCons["<<currEm2->numLocalCons<<"]");
	}

	if(obj_comp!=NULL)
	{
		assert(obj_comp->type == TOBJ);
	}
	//create sub expanded models
	for(vector<AmplModel*>::iterator it = subm_comps.begin();it!=subm_comps.end();it++)
	{
		AmplModel* subm = *it;
		assert(subm->type == TMODEL);
		LOG(" START submodel["<<subm->name<<"] -----   OF  ["<<name<<"]");
		if(subm->indexing!=NULL)
		{
			LOG("Indexing over: ["<<subm->indexing->print()<<"]");
			IndexSet* iset = subm->indexing->createIndexSet(currCtx);
			assert(iset->dummyCompMap.size()==1 && iset->dummySetMap.size()==1); // support only one dummy index for submodel/block declaration for now.

			SetComp* setComp = iset->dummyCompMap.begin()->second;
			string dummy = iset->dummyCompMap.begin()->first;
			Set* set = iset->dummySetMap.begin()->second;
			assert(set->dim == 1); //only support one dimensional set for block index set for now!
			LOG("Name["<<name<<"]'s Comp["<<subm->name<<"] card["<<set->card<<"]");

			vector<string>::iterator it;
			LOG("creating child  ExpandedModel over the set:"<<set->toString());
			for(it=set->setValues_data_order.begin();it!=set->setValues_data_order.end();it++)
			{
				LOG("creating child EM - ["<<subm->name<<"] for - set["<<setComp->name<<"] -- dummy["<<dummy<<"="<<*it<<"]");
				ExpandedModel* subem2 = subm->createExpandedModel(dummy,setComp,*it,currCtx);
				currEm2->addChildren(subem2);
			}
			delete iset;
		}
		else
		{
			LOG("creating child EM["<<subm->name<<"]  -  only child");
			ExpandedModel* subem2 = subm->createExpandedModel("",NULL,"",currCtx);
			currEm2->addChildren(subem2);
		}
	}
	LOG("End createExpandedModel - model["<<this->name<<"]");
	return currEm2;
}



void AmplModel::addComp(ModelComp *comp)
{
	LOG("-- In addComp  -- this model:["<<this->name<<"] level["<<level<<"] add type[" << comp->type<<"] ["<<comp<<"]");
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
	comp->model = this;  //set parent AmplModel
	all_comps.push_back(comp);

	LOG("-- End AddComp [current:"<<this->name<<"]  n_total["<<n_total<<"]");
}

////Feng- my methods starts here  //////////////////////////////////////////////////

ModelComp* AmplModel::findModelComp(string& id)
{
	ModelComp* rval = NULL;
	for(vector<ModelComp*>::iterator i=all_comps.begin();i!=all_comps.end();i++)
	{
		//LOG("findModelComp -- model["<<this->name<<"] -- search_for["<<id<<"] at "<<(*i)->id);
		if((*i)->name.compare(id)==0)
		{
			rval = *i;
			break;
		}
	}

	if(rval == NULL)
	{
		rval = parent==NULL? NULL:parent->findModelComp(id);

	}
	LOG("findModelComp -- model["<<this->name<<"] -- id["<<id<<"] - rval["<<rval<<"]");
	return rval;
}

ParamComp* AmplModel::findParamComp(string& id)
{
	ParamComp* rval = NULL;
	for(vector<ParamComp*>::iterator i=param_comps.begin();i!=param_comps.end();i++)
	{
		//LOG("findModelComp -- model["<<this->name<<"] -- search_for["<<id<<"] at "<<(*i)->id);
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
	LOG("findParamComp -- model["<<this->name<<"] -- id["<<id<<"] - rval["<<rval<<"]");
	return rval;
}

SetComp* AmplModel::findSetComp(string& id)
{
	SetComp* rval = NULL;
	for(vector<SetComp*>::iterator i=set_comps.begin();i!=set_comps.end();i++)
	{
		//LOG("findModelComp -- model["<<this->name<<"] -- search_for["<<id<<"] at "<<(*i)->id);
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
	LOG("findSetComp -- model["<<this->name<<"] -- id["<<id<<"] - rval["<<rval<<"]");
	return rval;
}

//TODO -- currently calculate set first then calculate param,
//	      there will be a problem if a calculated sym param required for creating a set.
void AmplModel::calculateModelComp(ModelContext* context)
{
	LOG("calculateModelComp - context["<<context->getContextId()<<"] -- on Model["<<this->name<<"] - ");
	for(vector<SetComp*>::iterator i=set_comps.begin();i!=set_comps.end();i++)
	{
		SetComp *comp = *i;
		assert(comp->type==TSET);
		if(context->getCompValue(comp) == NULL)
		{
			LOG("calculateSetModelComp   SET comp["<<comp->name<<"] -- "<<comp);
			comp->calculateSetModelComp(context);
			LOG("end calculateSetModelComp   -- ["<<comp->name<<"] -- "<<comp);
			assert(context->getCompValue(comp)!=NULL);
		}
	}
	for(vector<ParamComp*>::iterator i=param_comps.begin();i!=param_comps.end();i++)
	{
		ParamComp *comp = *i;
		assert(comp->type == TPARAM);
		if( context->getCompValue(comp) == NULL)
		{
			LOG("caculateParamModelComp    PARAM comp["<<comp->name<<"]");
			comp->calculateParamModelComp(context);
			LOG("end calculateParamModelComp   -- ["<<comp->name<<"]");
			assert(context->getCompValue(comp)!=NULL);
		}
	}
	LOG("calculateModelComp - context"<<context->getContextId()<<"] -- on Model["<<this->name<<"] --DONE");
}

void AmplModel::calculateModelCompRecursive(ModelContext* context)
{
	if(this->parent!=NULL)
	{
		this->parent->calculateModelCompRecursive(context->parent);
	}
	this->calculateModelComp(context);
}



void AmplModel::calculateLocalVar(ModelContext* ctx)
{
	LOG("calculateLocalVar - context["<<ctx->getContextId()<<"] -- on Model["<<this->name<<"] - ");
	vector<VarComp*>::iterator it_varComp =this->var_comps.begin();
	for(;it_varComp!=this->var_comps.end();it_varComp++)
	{
		VarComp* comp = *it_varComp;
		assert(comp->type==TVAR);
		if(ctx->getCompValue(comp)==NULL)
		{
			LOG("calculateLocalVar    VAR comp["<<comp->name<<"]");
			comp->calculateVarComp(ctx);
			LOG("calculateLocalVar    VAR comp["<<comp->name<<"]");
			assert(ctx->getCompValue(comp)!=NULL);
		}
	}
	LOG("calculateLocalVar - context"<<ctx->getContextId()<<"] -- on Model["<<this->name<<"] --DONE");
}

void AmplModel::calculateMemoryUsage(unsigned long& size)
{
	LOG("AmplModel::calculateMemoryUsage -- model["<<this->name<<"]");
	size += sizeof(AmplModel);
	size += this->name.size() + 1;
	for(vector<ModelComp*>::iterator it=all_comps.begin();it!=all_comps.end();it++)
	{
		size += sizeof(ModelComp*);
		(*it)->calculateMemoryUsage(size);
		if((*it)->type==TMODEL)
		{
			static_cast<AmplModel*>(*it)->calculateMemoryUsage(size);
		}
	}
}

/* ---------------------------------------------------------------------------
AmplModel::dump(const char *filename)
---------------------------------------------------------------------------- */
void AmplModel::logModel(const char *filename){
  ofstream fout(filename);
  this->dump(fout, 0);
}

/* ---------------------------------------------------------------------------
AmplModel::dump(ostream &fout)
---------------------------------------------------------------------------- */
void AmplModel::dump(ostream& fout, int counter){
	fout << "AM: ----------------------------------------------------------\n";
	fout << "AM: This is AmplModel (" << (void *) this << "): " << name << "\n";
	fout << "AM: level: " << level << "\n";
	fout << "AM: parent: " << (parent ? parent->name : "NULL") << "\n";
	fout << "AM: Nb submodels  : " << n_submodels << "\n";
	fout << "AM: Nb sets       : " << n_sets << "\n";
	fout << "AM: Nb parameters : " << n_params << "\n";
	fout << "AM: Nb objectives : " << n_objs << "\n";
	fout << "AM: Nb variables  : " << n_vars << "\n";
	fout << "AM: Nb constraints: " << n_cons << "\n";
	fout << "AM: List components:\n";

	for(vector<ModelComp*>::const_iterator p = all_comps.begin();p != all_comps.end();++p) {
		(*p)->dump(fout, counter);
		++counter;
	}

	if (n_submodels > 0)
		fout << "AM: now list the submodels:\n";
	for(vector<AmplModel*>::const_iterator p = subm_comps.begin();p != subm_comps.end();++p) {
		AmplModel *mc = *p;
		mc->dump(fout,counter);
	}
}

















//legacy !!!

/* --------------------------------------------------------------------------
//AmplModel::removeComp()
//---------------------------------------------------------------------------- */
//void AmplModel::removeComp(ModelComp *comp)
//{
//	LOG("enter removeComp - name["<<this->id<<"] comp["<<comp->id<<"]");
//	bool found = false;
//	for(vector<ModelComp*>::iterator p = all_comps.begin();p!=all_comps.end();p++)
//	{
//		if ((*p)->id.compare(comp->id)==0)
//		{
//			all_comps.erase(p); // this invalidates the iterator => break from loop
//			n_total--;
//			found = true;
//			break;
//		}
//	}
//	if(found == false)
//	{
//		LOG("try to remove comp"[<<comp->id<<"] not in this model["<<this->id<<"]");
//		assert(false);
//	}
//
//	switch(comp->type){
//	case TVAR:
//		this->removeComp(var_comps,comp);
//		n_vars--;
//		break;
//	case TCON:
//		this->removeComp(con_comps,comp);
//		n_cons--;
//		break;
//	case TPARAM:
//		this->removeComp(param_comps,comp);
//		n_params--;
//	  	break;
//	case TSET:
//		this->removeComp(set_comps,comp);
//		n_sets--;
//		break;
//	case TOBJ:
//		obj_comp = NULL;
//		n_objs--;
//		break;
//	case TMODEL:
//		this->removeComp(subm_comps,comp);
//		n_submodels--;
//		break;
//	default:
//	  cerr << "removeComp: type " << comp->type << "unrecognised\n";
//	  exit(1);
//	}
//}
//
///* --------------------------------------------------------------------------
//AmplModel::removeComp()
//---------------------------------------------------------------------------- */
//template<typename T>
//void AmplModel::removeComp(vector<T*> comps, ModelComp* comp)
//{
//	for(vector<ModelComp*>::iterator it=comps.begin();it!=comps.end();it++)
//	{
//		if((*it)->id.compare(comp->id))
//		{
//			comps.erase(it);
//			break;
//		}
//	}
//}
//
//
//
///* --------------------------------------------------------------------------
//AmplModel::applyChanges()
//---------------------------------------------------------------------------- */
//void
//AmplModel::applyChanges()
//{
//  for (list<changeitem>::iterator p = changes.begin();
//       p != changes.end(); ++p) {
//    changeitem& ch = *p;
//    if (ch.action == CHANGE_REM)
//      ch.model->removeComp(ch.comp);
//    if (ch.action == CHANGE_ADD)
//      ch.model->addComp(ch.comp);
//  }
//  changes.clear();
//}
//
///* --------------------------------------------------------------------------
//AmplModel::reassignDependencies()
//---------------------------------------------------------------------------- */
///** Recursively recalculate dependency list and re-resolve IDREF nodes.
// *
// *  In the process of building the AmplModel tree from the StochModelTree
// *  some of the IDREF dependency nodes still point to the StochModelComp
// *  nodes from the StochModel tree (or the intermediate tree)
// *
// *  This routine goes through all components and makes sure that IDREF
// *  nodes are resolved with respect to the correct ModelComp and that
// *  the dependency lists are in order.
// *  Recursively follows down submodels.
// */
//void
//AmplModel::reassignDependencies()
//{
//	//1. reassigned depenencies for it's ModelComp
////	LOG("reassignDependencies -- at Model["<<this->name<<"]");
////	for (vector<ModelComp*>::iterator p = all_comps.begin(); p != all_comps.end(); p++)
////	{
////		if ((*p)->type == TMODEL)
////		{
////			AmplModel *submodel = (*p)->other;
////			submodel->reassignDependencies();
////		}
////		else
////		{
////			(*p)->reassignDependencies();
////		}
////	}
//}
//
//void AmplModel::reassignModelIndexDependencies()
//{
//	LOG("reassignModelIndexDependencies -- at Model["<<id<<"]");
//	//2. then reassign dependencies for this model's index to point to its own modelComp
////	if (this->node->indexing)
////	{
////		assert(this->parent!=NULL);
////		LOG("reassignDependencies in indexing -- "<<this->node->indexing->print());
////		list<SyntaxNode*> refns;
////		this->node->indexing->findIDREF(&refns);
////		for(list<SyntaxNode*>::iterator it=refns.begin();it!=refns.end();it++)
////		{
////			SyntaxNodeIDREF* refn = static_cast<SyntaxNodeIDREF*>(*it);
////			string id = refn->ref->id;
////			for(vector<ModelComp*>::iterator it2=this->parent->all_comps.begin();it2!=this->parent->all_comps.end();it2++)
////			{
////				LOG("ModelComp["<<(*it2)->id);
////				if((*it2)->id.compare(id)==0)
////				{
////					LOG("reassignDependencies for TMODEL["<<this->name<<"] indexing["<<this->node->indexing->print()<<"]");
////					refn->ref = *it2;
////					break;
////				}
////			}
////		}
////		this->node->indexing->resetSplitExpression();
////		this->node->indexing->splitExpression();
////	}
////	else
////	{
////		LOG("no indexing for this Model ["<<name<<"]");
////	}
////
////	for (vector<ModelComp*>::iterator it = all_comps.begin(); it != all_comps.end(); it++)
////	{
////		if((*it)->type == TMODEL)
////		{
////			(*it)->other->reassignModelIndexDependencies();
////		}
////	}
//}
//
//
//SyntaxNodeIDREF* AmplModel::createIdrefNode(SyntaxNodeID *ref)
//{
//	for(vector<ModelComp*>::iterator p=all_comps.begin(); p!=all_comps.end(); ++p){
//		ModelComp *thismc = *p;
//		if (ref->id() == thismc->id) { //matched
//			LOG("Found Match: " << ref->id() << " refers to ");
//			LOG("             "<<ModelComp::nameTypes[thismc->type]);
//			LOG("             "<< thismc->id);
//			if (thismc->indexing)
//				LOG("             "<<*(thismc->indexing));
//			if (thismc->attributes)
//				LOG("             "<< *(thismc->attributes));
//
//			SyntaxNodeIDREF *ret;
//			if(thismc->type==TMODEL) {
//				ret = new SyntaxNodeIDREF(IDREFM, thismc);
//			}
//			else {
//				ret = new SyntaxNodeIDREF(IDREF, thismc);
//			}
//			return ret;
//		}
//	}
//
//	/* need also to look through parent model */
//	if (parent) return parent->createIdrefNode(ref);
//
//	/* need also to look through list of local variables */
//
//	cerr << "ERROR: Could not find ref '" << ref->id() << "' in context\n";
//	exit(1);
//}


/*
 * calculating variable depended levels for each constraint modelcomp in this amplmodel
 */
//void AmplModel::analyseConstraints()
//{
//	LOG("analyseConstraints --- model["<<this->name<<"] level["<<level<<"]");
//	for(vector<ModelComp*>::iterator it = con_comps.begin();it!=con_comps.end();it++)
//	{
//		assert((*it)->type==TCON);
//		(*it)->analyseVarDepLevelsInCons();
//	}
//
//	if(obj_comp!=NULL)
//	{
//		assert(obj_comp->type == TMAX || obj_comp->type == TMIN);
//		obj_comp->analyseVarDepLevelsInCons();
//	}
//
//	for(vector<ModelComp*>::iterator it=subm_comps.begin();it!=subm_comps.end();it++)
//	{
//		assert((*it)->type == TMODEL);
//		(*it)->other->analyseConstraints();
//	}
//}

