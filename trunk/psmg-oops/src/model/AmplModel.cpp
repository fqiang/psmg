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
  parent(par),
  isCompsUpdated(false)
{

  LOG("AmplModel -- create - name["<<name<<"] ");

}

/** Destructor */
AmplModel::~AmplModel()
{
	LOG("AmplModel destructor called...["<<this->name<<"]");
	for (list<ModelComp*>::iterator it = comps.begin(); it != comps.end(); it++)
	{
		ModelComp* mc = *it;
		if(mc->type==TMODEL)
		{
			delete mc->other;
			mc->other = NULL;
		}
		delete mc;
	}
	comps.clear();
	changes.clear();

}

/* ---------------------------------------------------------------------------
AmplModel::createExpandedModel2
---------------------------------------------------------------------------- */
ExpandedModelAbstract* AmplModel::createExpandedModel2(ModelContext* context,ExpandedModelAbstract* parent)
{
	LOG("createExpandedModel2- Model["<<this->name<<"]");

	//step 1; must update comps
	if(!this->isCompsUpdated)
	{
		this->updateCurrLevelModelComp();
		this->isCompsUpdated = true;
	}
	//for root model loading datas from file
	if(parent == NULL)
	{
		LOG("**** parsing data file [" << GlobalVariables::datafilename <<"]");
		parse_data(context);
		context->isCompsCalculated = true; //root context model comps value is set by loading data from file
		LOG("**** parsing data file End ");
	}
	//then calculate the set and parameters
	//this->calculateCurrLevelModelComp(context);
	//set dim is calculated in step 1.
	ExpandedModelAbstract *em = new ExpandedModel2(this,parent,context);
	ExpandedModel2* em2 = static_cast<ExpandedModel2*>(em);
	this->calculateCurrLevelSetModelComp(context);

	for(list<ModelComp*>::iterator it = comps.begin();it!=comps.end();it++)
	{
		ModelComp *mc = *it;
		if (mc->type == TVAR)
		{
			LOG("   model-name ["<<name<<"] Comp["<<mc->id<<"] is TVAR type");
			mc->calculateLocalVar(context);
			em2->addLocalVar(mc);
			LOG("   local var:"<<endl<<mc->id);
		}
		else if(mc->type==TCON)
		{
			LOG("   model-name ["<<name<<"] Comp["<<mc->id<<"] is TCON type");
			em2->addLocalCon(mc);
		}
		else if(mc->type==TMIN || mc->type==TMAX)
		{
			LOG("   model-name ["<<name<<"] Comp["<<mc->id<<"] is TMIN or TMAX type");
			em2->objComp = mc;
		}
		else if (mc->type == TMODEL)
		{
			LOG("Name["<<name<<"]'s Comp["<<mc->id<<"] is TMODEL type");
			LOG("Indexing over: ["<<mc->indexing<<"]");
			if(mc->indexing!=NULL)
			{
				assert(mc->indexing->sets_mc.size()==1);
				ModelComp* setComp = mc->indexing->sets_mc[0];
				Set* set = static_cast<Set*>(context->getCompValue(setComp));
				assert(set->dim == 1);
				int card = set->card;
				LOG("Name["<<name<<"]'s Comp["<<mc->id<<"] card["<<card<<"]");


				SyntaxNode* ind = static_cast<SyntaxNode*>(mc->indexing);
				hash_map<string,ModelComp*> dummys;
				vector<string> ds;
				vector<ModelComp*> cs;
				ind->calculateParamIndicies(ds,cs,dummys);//feng - actually should named calculate model indicies
				vector<string>::iterator it;
				LOG("creating child  ExpandedModel2 over the set:"<<set->toString());
				for(it=set->setValues_data_order.begin();it!=set->setValues_data_order.end();it++)
				{
					ModelContext* childContext = new ModelContext(context);
					assert(set->dim==1);  //not work for composite set yet!
					hash_map<string,ModelComp*>::iterator it2;
					assert(dummys.size()==1); //only 1 index for model for now!
					for(it2=dummys.begin();it2!=dummys.end();it2++)
					{
						string dummy = it2->first;
						LOG("creating child["<<mc->id<<"] for ["<<this->name<<"] "<<dummy<<"=["<<dummy<<"]");
						ModelComp* comp = it2->second;
						AmplModel* submod = mc->other;
						childContext->addDummySetValueMap(dummy,comp,*it);
						ExpandedModelAbstract* subem = submod->createExpandedModel2(childContext,em2);
						ExpandedModel2* subem2 = static_cast<ExpandedModel2*>(subem);
						assert(childContext->em==subem2);
						em2->children.push_back(subem);
						LOG("child expandedModel2 ["<<subem2->getName());
					}
				}
			}
			else
			{
				LOG("Model's indexing over is NULL");
				ModelContext* childContext = new ModelContext(context);
				AmplModel* submod = mc->other;
				ExpandedModelAbstract* subem2 = submod->createExpandedModel2(childContext,em2);
				em2->children.push_back(subem2);
				LOG("child expandedModel2 ["<<subem2->getName()<<"]");
				//assert(childContext == NULL);
			}
		}
		else
		{
			LOG("Name["<<name<<"]'s Comp["<<mc->id<<"] is OTHER type");
		}
	}
	this->clearCalculateCurrLevelSetModelComp(context);
	LOG("End createExpandedModel2 - model["<<this->name<<"]");
	return em2;
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
  fout << "AM: Nb objectives : " <<  n_submodels << "\n";
  fout << "AM: List components:\n";
  list<ModelComp*>::const_iterator p;
  for (p = comps.begin(); p != comps.end(); ++p)
    (*p)->dump(fout);

  if (n_submodels>0)
    fout << "AM: now list the submodels:\n";
  for (p = comps.begin(); p != comps.end(); ++p) {
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
  for(list<ModelComp*>::iterator p = comps.begin();p!=comps.end();p++){
    if ((*p)->id == comp->id) {
      comps.erase(p); // this invalidates the iterator => break from loop
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
	LOG("-- In addComp  -- [current:"<<this->name<<"] type[" << comp->type<<"]");
  AmplModel *subm;
  //ModelComp *lastinmodel = model->last;
  switch(comp->type)
  {
    case TVAR:
      n_vars++;
      break;
    case TCON:
      n_cons++;
      break;
    case TSET:
      n_sets++;
      break;
    case TPARAM:
      n_params++;
      break;
    case TMIN:
    case TMAX:
      n_objs++;
      break;
    case TMODEL:  //feng a bad design style. subm is only used to setup the parant.
    			  //	the parent should be set while creating.
      n_submodels++;
      subm = comp->other;
      subm->parent = this;
      break;
    default:
      cerr << "addComp: type " << comp->type << "unrecognised\n";
      exit(1);
    }
  n_total++;
  comp->model = this;  //set parent AmplModel
  comps.push_back(comp);

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
	for (list<ModelComp*>::iterator p = comps.begin(); p != comps.end(); p++)
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
			for(list<ModelComp*>::iterator it2=this->parent->comps.begin();it2!=this->parent->comps.end();it2++)
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

	for (list<ModelComp*>::iterator it = comps.begin(); it != comps.end(); it++)
	{
		if((*it)->type == TMODEL)
		{
			(*it)->other->reassignModelIndexDependencies();
		}
	}
}


SyntaxNodeIDREF* AmplModel::createIdrefNode(IDNode *ref)
{
	for(list<ModelComp*>::iterator p=comps.begin(); p!=comps.end(); ++p){
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
	for(list<ModelComp*>::iterator i=comps.begin();i!=comps.end();i++)
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

void AmplModel::updateCurrLevelModelComp()
{
	for(list<ModelComp*>::iterator i=comps.begin();i!=comps.end();i++)
	{
		if((*i)->type==TSET)
		{
			(*i)->setSetDim();
			LOG("updateModelComp for set - id["<<(*i)->id<<"] dim["<<(*i)->getSetDim()<<"] -- "<<*i);
		}
		else if((*i)->type==TPARAM)
		{
			(*i)->setParamIndicies();
			LOG("updateModelComp for param - id["<<(*i)->id<<"] numParamIndicies["<<(*i)->getNumParamIndicies()<<"] -- "<<*i);
		}
	}
}

void AmplModel::calculateCurrLevelModelComp(ModelContext* context)
{
	this->calculateCurrLevelSetModelComp(context);
	this->calculateCurrLevelParamModelComp(context);
}

void AmplModel::calculateModelCompRecursive(ModelContext* context)
{
	if(this->parent!=NULL)
	{
		this->parent->calculateModelCompRecursive(context->parent);
	}
	this->calculateCurrLevelModelComp(context);
}

void AmplModel::calculateCurrLevelSetModelComp(ModelContext* context)
{
	for(list<ModelComp*>::iterator i=comps.begin();i!=comps.end();i++)
	{
		ModelComp *comp = *i;
		if(comp->type==TSET && context->getCompValue(comp) == NULL)
		{
			LOG("calculateSetModelComp   SET comp["<<comp->id<<"] -- "<<comp);
			comp->calculateSetModelComp(context);
			LOG("end calculateSetModelComp   -- ["<<comp->id<<"] -- "<<comp);
			assert(context->getCompValue(comp)!=NULL);
		}
	}
}

void AmplModel::clearCalculateCurrLevelSetModelComp(ModelContext* context)
{
	for(list<ModelComp*>::iterator i=comps.begin();i!=comps.end();i++)
	{
		ModelComp *comp = *i;
		if(comp->type==TSET && comp->isFromFile == false)
		{
			LOG("removeCompValueMap   SET comp["<<comp->id<<"] -- "<<comp);
			context->removeCurrLevelCompValueMap(comp);
			LOG("end removeCompValueMap   -- ["<<comp->id<<"] -- "<<comp);
			assert(context->getCompValue(comp)==NULL);
		}
	}
}

void AmplModel::calculateCurrLevelParamModelComp(ModelContext* context)
{
	for(list<ModelComp*>::iterator i=comps.begin();i!=comps.end();i++)
	{
		ModelComp *comp = *i;
		if(comp->type == TPARAM && context->getCompValue(comp) == NULL)
		{
			LOG("caculateParamModelComp    PARAM comp["<<comp->id<<"]");
			comp->calculateParamModelComp(context);
			LOG("end calculateParamModelComp   -- ["<<comp->id<<"]");
			assert(context->getCompValue(comp)!=NULL);
		}
	}
}

void AmplModel::calculateMemoryUsage(unsigned long& size)
{
	LOG("AmplModel::calculateMemoryUsage -- model["<<this->name<<"]");
	size += sizeof(AmplModel);
	size += this->name.size() + 1;
	for(list<ModelComp*>::iterator it=comps.begin();it!=comps.end();it++)
	{
		size += sizeof(ModelComp*);
		(*it)->calculateMemoryUsage(size);
		if((*it)->type==TMODEL)
		{
			(*it)->other->calculateMemoryUsage(size);
		}
	}
}
