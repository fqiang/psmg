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
#include "SyntaxNodeIx.h"
#include "IDNode.h"
#include "OpNode.h"
#include "ListNode.h"
#include "SyntaxNodeIDREF.h"
#include "changeitem.h"
#include "../parser/sml.tab.h"
#include "../context/ExpandedModel2.h"
#include "../sml/GlobalVariables.h"
#include "../util/global_util_functions.h"
#include <cassert>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;

using namespace __gnu_cxx;

list<changeitem> AmplModel::changes; // initialize to empty list
AmplModel *AmplModel::root = NULL; //initialize root to NULL


/* ---------------------------------------------------------------------------
AmplModel::AmplModel()
---------------------------------------------------------------------------- */
/** Constructor */
AmplModel::AmplModel(const string& _name, AmplModel *par) :
  name(_name),
  n_vars(0),
  n_cons(0),
  n_params(0),
  n_sets(0),
  n_objs(0),
  n_submodels(0),
  n_total(0),
  level(0),
  node(NULL),
  parent(par)
{

	assert(par==NULL);
	LOG("AmplModel -- create - name["<<name<<"] ");
}

/** Destructor */
AmplModel::~AmplModel()
{
	LOG("AmplModel destructor called...["<<this->name<<"]");
	this->deleteModelCompVector(all_comps);

	set_comps.clear();
	param_comps.clear();
	var_comps.clear();
	con_comps.clear();
	subm_comps.clear();
	obj_comp =  NULL;

	changes.clear();

}

void AmplModel::deleteModelCompVector(vector<ModelComp*> comps)
{
	for (vector<ModelComp*>::iterator it = comps.begin(); it != comps.end(); it++)
	{
		ModelComp* mc = *it;
		if(mc->type==TMODEL)
		{
			delete mc->other;
			mc->other = NULL;
		}
		delete mc;
	}
}

void AmplModel::settingUpLevels(int lev)
{
	LOG("setting model["<<this->name<<"] level["<<lev<<"]");
	this->level = lev;
	vector<ModelComp*>::const_iterator it=this->subm_comps.begin();

	for(;it!=this->all_comps.end();it++)
	{
		LOG("submodel comp["<<(*it)->id<<"]");
		assert((*it)->type==TMODEL);
		(*it)->other->settingUpLevels(lev+1);
	}
}

void AmplModel::analyseConstraints()
{
	for(vector<ModelComp*>::iterator it = con_comps.begin();it!=con_comps.end();it++)
	{
		assert((*it)->type==TCON);
		(*it)->analyseVarDepLevelsInCons();
	}

	if(obj_comp!=NULL)
	{
		assert(obj_comp->type == TMAX || obj_comp->type == TMIN);
		obj_comp->analyseVarDepLevelsInCons();
	}

	for(vector<ModelComp*>::iterator it=subm_comps.begin();it!=subm_comps.end();it++)
	{
		assert((*it)->type == TMODEL);
		(*it)->other->analyseConstraints();
	}
}

/* ---------------------------------------------------------------------------
AmplModel::createExpandedModel2
---------------------------------------------------------------------------- */
ExpandedModel2* AmplModel::createExpandedModel2(string dummyVar,ModelComp* comp,string value,ModelContext* parent)
{
	LOG("createExpandedModel2- Model["<<this->name<<"] -- level["<<this->level<<"]");

	for(vector<ModelComp*>::iterator i=set_comps.begin();i!=set_comps.end();i++)
	{
		assert((*i)->type==TSET);
		(*i)->setSetDim();
		LOG("updateModelComp for set - id["<<(*i)->id<<"] dim["<<(*i)->setDim<<"] -- "<<*i);
	}
	for(vector<ModelComp*>::iterator i=param_comps.begin();i!=param_comps.end();i++)
	{
		assert((*i)->type==TPARAM);
		(*i)->setParamIndicies();
		LOG("updateModelComp for param - id["<<(*i)->id<<"] numParamIndicies["<<(*i)->getNumParamIndicies()<<"] -- "<<*i);
	}

	ModelContext* currCtx = new ModelContext(parent);
	ExpandedModel2 *currEm2 = NULL;
	if(this==AmplModel::root){
		//for root node
		currEm2 = new ExpandedModel2(AmplModel::root,currCtx,"",NULL,"");
		parse_data(currCtx);
	}
	else
	{
		currEm2 = new ExpandedModel2(this,currCtx,dummyVar,comp,value);
	}


	//calculating set values
	for(vector<ModelComp*>::iterator i=set_comps.begin();i!=set_comps.end();i++)
	{
		ModelComp *mp = *i;
		assert(mp->type==TSET);
		if(currCtx->getCompValue(mp) == NULL)
		{
			LOG("calculateSetModelComp   SET comp["<<mp->id<<"] -- "<<mp);
			mp->calculateSetModelComp(currCtx);
			LOG("end calculateSetModelComp   -- ["<<mp->id<<"] -- "<<mp);
			assert(currCtx->getCompValue(mp)!=NULL);
		}
	}

	//calculating param values
	for(vector<ModelComp*>::iterator i=param_comps.begin();i!=param_comps.end();i++)
	{
		ModelComp *mp = *i;
		assert(mp->type == TPARAM);
		if(currCtx->getCompValue(mp) == NULL)
		{
			LOG("caculateParamModelComp    PARAM comp["<<mp->id<<"]");
			mp->calculateParamModelComp(currCtx);
			LOG("end calculateParamModelComp   -- ["<<mp->id<<"]");
			assert(currCtx->getCompValue(mp)!=NULL);
		}
	}

	//calculating var list
	for(vector<ModelComp*>::iterator it=var_comps.begin();it!=var_comps.end();it++)
	{
		ModelComp* mc = *it;
		assert(mc->type == TVAR);
		LOG("   model-name ["<<name<<"] Comp["<<mc->id<<"] is TVAR type");
		mc->calculateLocalVar(currCtx);
		currEm2->addLocalVar(mc);
	}

	//calculating con list
	for(vector<ModelComp*>::iterator it=con_comps.begin();it!=con_comps.end();it++)
	{
		ModelComp* mc = *it;
		assert(mc->type==TCON);
		LOG("   model-name ["<<name<<"] Comp["<<mc->id<<"] is TCON type");
		currEm2->addLocalCon(mc);
	}

	if(obj_comp!=NULL)
	{
		assert(obj_comp->type == TMIN || obj_comp->type== TMAX);
		currEm2->objComp = obj_comp;
	}
	//create sub expanded models
	for(vector<ModelComp*>::iterator it = subm_comps.begin();it!=subm_comps.end();it++)
	{
		ModelComp* mc = *it;
		assert(mc->type == TMODEL);
		LOG("For Sub-Model ["<<name<<"]'s Comp["<<mc->id<<"] ");
		if(mc->indexing!=NULL)
		{
			LOG("Indexing over: ["<<mc->indexing->print()<<"]");
			assert(mc->indexing->sets_mc.size()==1);
			assert(mc->indexing->opCode==LBRACE);
			assert(mc->indexing->values.size()==1);
			assert(mc->indexing->values[0]->opCode == IN);

			ModelComp* setComp = static_cast<SyntaxNodeIDREF*>(mc->indexing->values[0]->values[1])->ref;
			string dummy = static_cast<IDNode*>(mc->indexing->values[0]->values[0])->id();
			Set* set = static_cast<Set*>(currCtx->getCompValue(setComp));
			assert(set->dim == 1); //only support one dimensional set for block index set for now!
			LOG("Name["<<name<<"]'s Comp["<<mc->id<<"] card["<<set->card<<"]");

			vector<string>::iterator it;
			LOG("creating child  ExpandedModel2 over the set:"<<set->toString());
			for(it=set->setValues_data_order.begin();it!=set->setValues_data_order.end();it++)
			{
				LOG("creating sub-em2 ["<<mc->id<<"] for - set["<<setComp->id<<"] ["<<this->name<<"] "<<dummy<<"=["<<*it<<"]");
				AmplModel* submod = mc->other;
				ExpandedModel2* subem2 = submod->createExpandedModel2(dummy,setComp,*it,currCtx);
				currEm2->addChildren(subem2);
				LOG("add child em2 ["<<subem2->getName()<<"] for - ["<<currEm2->getName()<<"]");
			}
		}
		else
		{
			LOG("Model's indexing over is NULL");
			AmplModel* submod = mc->other;
			ExpandedModel2* subem2 = submod->createExpandedModel2("",NULL,"",currCtx);
			currEm2->addChildren(subem2);
			LOG("child expandedModel2 ["<<subem2->getName()<<"]");
			//assert(childContext == NULL);
		}
	}

	LOG("End createExpandedModel2 - model["<<this->name<<"]");
	return currEm2;
}


/* ---------------------------------------------------------------------------
AmplModel::print
---------------------------------------------------------------------------- */
void 
AmplModel::print() const {
  dump(cout);
}

/* ---------------------------------------------------------------------------
AmplModel::dump(const char *filename)
---------------------------------------------------------------------------- */
void 
AmplModel::dump(const char *filename) const {
  ofstream fout(filename);
  dump(fout);
}

/* ---------------------------------------------------------------------------
AmplModel::dump(ostream &fout)
---------------------------------------------------------------------------- */
void 
AmplModel::dump(ostream& fout) const {
  fout << "AM: ----------------------------------------------------------\n";
  fout << "AM: This is AmplModel (" << (void *) this << "): " << name << "\n";
  fout << "AM: level: " << level << "\n";
  fout << "AM: parent: " << (parent?parent->name:"NULL") << "\n";
  fout << "AM: Nb submodels  : " <<  n_submodels << "\n";
  fout << "AM: Nb sets       : " <<  n_sets << "\n";
  fout << "AM: Nb parameters : " <<  n_params << "\n";
  fout << "AM: Nb objectives : " <<  n_objs << "\n";
  fout << "AM: Nb variables  : " <<  n_vars << "\n";
  fout << "AM: Nb constraints: " <<  n_cons << "\n";
  fout << "AM: List components:\n";
  vector<ModelComp*>::const_iterator p;

  int counter=0;
  for (p = all_comps.begin(); p != all_comps.end(); ++p,++counter)
  {
	  (*p)->dump(fout,counter);
  }


  if (n_submodels>0)
    fout << "AM: now list the submodels:\n";
  for (p = all_comps.begin(); p != all_comps.end(); ++p) {
    ModelComp *mc = *p;
    if (mc->type == TMODEL){
      AmplModel *am = mc->other;
      am->dump(fout);
    }
  }
}

/* --------------------------------------------------------------------------
AmplModel::removeComp()
---------------------------------------------------------------------------- */
void
AmplModel::removeComp(const ModelComp *comp) {
  // FIXME: check of comp is indeed on the list
  
  bool found = false;
  for(vector<ModelComp*>::iterator p = all_comps.begin();p!=all_comps.end();p++)
  {
    if ((*p)->id == comp->id)
    {
    	all_comps.erase(p); // this invalidates the iterator => break from loop
    	found = true;
    	break;
    }
  }
  if (!found){
    cerr << "ERROR: Attempting to remove component " << comp->id
         << " from model " << name << ":\nComponent is not in model.\n";
    exit(1);
  }
  n_total--;
  switch(comp->type){
    case TVAR: 
      n_vars--;
      break;
    case TCON: 
      n_cons--;
      break;
    case TPARAM: 
      n_params--;
      break;
    case TSET: 
      n_sets--;
      break;
    case TMIN: 
    case TMAX: 
      n_objs--;
      break;
    case TMODEL: 
      n_submodels--;
      break;
    default:
      cerr << "removeComp: type " << comp->type << "unrecognised\n";
      exit(1);
  }
}
/* --------------------------------------------------------------------------
AmplModel::removeComp()
---------------------------------------------------------------------------- */
void
AmplModel::addComp(ModelComp *comp)
{
	LOG("-- In addComp  -- this model:["<<this->name<<"] level["<<level<<"] add type[" << comp->type<<"]");
  switch(comp->type)
  {
    case TVAR:
      n_vars++;
      var_comps.push_back(comp);
      break;
    case TCON:
      n_cons++;
      con_comps.push_back(comp);
      break;
    case TSET:
      n_sets++;
      set_comps.push_back(comp);
      break;
    case TPARAM:
      n_params++;
      param_comps.push_back(comp);
      break;
    case TMIN:
    case TMAX:
      obj_comp = comp;
      n_objs++;
      assert(n_objs==1);
      break;
    case TMODEL:
      n_submodels++;
      comp->other->parent = this;
      comp->other->level = this->level + 1;
      subm_comps.push_back(comp);
      break;
    default:
      cerr << "addComp: type " << comp->type << "unrecognised\n";
      exit(1);
    }
  n_total++;
  comp->model = this;  //set parent AmplModel
  all_comps.push_back(comp);

  comp->setUpDependencies();
  LOG("-- End AddComp [current:"<<this->name<<"]  n_total["<<n_total<<"]");
}

/* --------------------------------------------------------------------------
AmplModel::applyChanges()
---------------------------------------------------------------------------- */
void
AmplModel::applyChanges()
{
  for (list<changeitem>::iterator p = changes.begin();
       p != changes.end(); ++p) {
    changeitem& ch = *p;
    if (ch.action == CHANGE_REM)
      ch.model->removeComp(ch.comp);
    if (ch.action == CHANGE_ADD)
      ch.model->addComp(ch.comp);
  }
  changes.clear();
}

/* --------------------------------------------------------------------------
AmplModel::reassignDependencies()
---------------------------------------------------------------------------- */
/** Recursively recalculate dependency list and re-resolve IDREF nodes.
 *
 *  In the process of building the AmplModel tree from the StochModelTree
 *  some of the IDREF dependency nodes still point to the StochModelComp
 *  nodes from the StochModel tree (or the intermediate tree)
 *
 *  This routine goes through all components and makes sure that IDREF
 *  nodes are resolved with respect to the correct ModelComp and that
 *  the dependency lists are in order.
 *  Recursively follows down submodels.
 */
void
AmplModel::reassignDependencies()
{
	//1. reassigned depenencies for it's ModelComp
	LOG("reassignDependencies -- at Model["<<this->name<<"]");
	for (vector<ModelComp*>::iterator p = all_comps.begin(); p != all_comps.end(); p++)
	{
		if ((*p)->type == TMODEL)
		{
			AmplModel *submodel = (*p)->other;
			submodel->reassignDependencies();
		}
		else
		{
			(*p)->reassignDependencies();
		}
	}
}

void AmplModel::reassignModelIndexDependencies()
{
	LOG("reassignModelIndexDependencies -- at Model["<<name<<"]");
	//2. then reassign dependencies for this model's index to point to its own modelComp
	if (this->node->indexing)
	{
		assert(this->parent!=NULL);
		LOG("reassignDependencies in indexing -- "<<this->node->indexing->print());
		list<SyntaxNode*> refns;
		this->node->indexing->findIDREF(&refns);
		for(list<SyntaxNode*>::iterator it=refns.begin();it!=refns.end();it++)
		{
			SyntaxNodeIDREF* refn = static_cast<SyntaxNodeIDREF*>(*it);
			string id = refn->ref->id;
			for(vector<ModelComp*>::iterator it2=this->parent->all_comps.begin();it2!=this->parent->all_comps.end();it2++)
			{
				LOG("ModelComp["<<(*it2)->id);
				if((*it2)->id.compare(id)==0)
				{
					LOG("reassignDependencies for TMODEL["<<this->name<<"] indexing["<<this->node->indexing->print()<<"]");
					refn->ref = *it2;
					break;
				}
			}
		}
		this->node->indexing->resetSplitExpression();
		this->node->indexing->splitExpression();
	}
	else
	{
		LOG("no indexing for this Model ["<<name<<"]");
	}

	for (vector<ModelComp*>::iterator it = all_comps.begin(); it != all_comps.end(); it++)
	{
		if((*it)->type == TMODEL)
		{
			(*it)->other->reassignModelIndexDependencies();
		}
	}
}


SyntaxNodeIDREF* AmplModel::createIdrefNode(IDNode *ref)
{
	for(vector<ModelComp*>::iterator p=all_comps.begin(); p!=all_comps.end(); ++p){
		ModelComp *thismc = *p;
		if (ref->id() == thismc->id) { //matched
			LOG("Found Match: " << ref->id() << " refers to ");
			LOG("             "<<ModelComp::nameTypes[thismc->type]);
			LOG("             "<< thismc->id);
			if (thismc->indexing)
				LOG("             "<<*(thismc->indexing));
			if (thismc->attributes)
				LOG("             "<< *(thismc->attributes));

			SyntaxNodeIDREF *ret;
			if(thismc->type==TMODEL) {
				ret = new SyntaxNodeIDREF(IDREFM, thismc);
			}
			else {
				ret = new SyntaxNodeIDREF(IDREF, thismc);
			}
			return ret;
		}
	}

	/* need also to look through parent model */
	if (parent) return parent->createIdrefNode(ref);

	/* need also to look through list of local variables */

	cerr << "ERROR: Could not find ref '" << ref->id() << "' in context\n";
	exit(1);
}

////Feng- my methods starts here  //////////////////////////////////////////////////

ModelComp* AmplModel::findModelComp(string id,compType forType)
{
	ModelComp* rval = NULL;
	for(vector<ModelComp*>::iterator i=all_comps.begin();i!=all_comps.end();i++)
	{
		if((*i)->type==forType && (*i)->id.compare(id)==0)
		{
			rval = *i;
			break;
		}
		else if((*i)->type==TMODEL)
		{
			rval = (*i)->model->findModelComp(id,forType);
			break;
		}
	}
	LOG("findModelComp - id["<<id<<"] type["<<forType<<"] - rval["<<rval<<"]");
	return rval;
}

void AmplModel::calculateCurrLevelModelComp(ModelContext* context)
{
	if(context->isCompsCalculated == false);
	{
		for(vector<ModelComp*>::iterator i=set_comps.begin();i!=set_comps.end();i++)
		{
			ModelComp *comp = *i;
			assert(comp->type==TSET);
			if(context->getCompValue(comp) == NULL)
			{
				LOG("calculateSetModelComp   SET comp["<<comp->id<<"] -- "<<comp);
				comp->calculateSetModelComp(context);
				LOG("end calculateSetModelComp   -- ["<<comp->id<<"] -- "<<comp);
				assert(context->getCompValue(comp)!=NULL);
			}
		}
		for(vector<ModelComp*>::iterator i=param_comps.begin();i!=param_comps.end();i++)
		{
			ModelComp *comp = *i;
			assert(comp->type == TPARAM);
			if( context->getCompValue(comp) == NULL)
			{
				LOG("caculateParamModelComp    PARAM comp["<<comp->id<<"]");
				comp->calculateParamModelComp(context);
				LOG("end calculateParamModelComp   -- ["<<comp->id<<"]");
				assert(context->getCompValue(comp)!=NULL);
			}
		}
		context->isCompsCalculated = true;
	}
	assert(context->isCompsCalculated == true);
}

void AmplModel::calculateModelCompRecursive(ModelContext* context)
{
	if(this->parent!=NULL)
	{
		this->parent->calculateModelCompRecursive(context->parent);
	}
	this->calculateCurrLevelModelComp(context);
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
			(*it)->other->calculateMemoryUsage(size);
		}
	}
}
