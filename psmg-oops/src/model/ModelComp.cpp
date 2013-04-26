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

#include "ModelComp.h"
#include "AmplModel.h"
#include "../sml/GlobalVariables.h" //for GlobalVariables class
#include "SyntaxNode.h"
#include "SyntaxNodeIx.h"
#include "SyntaxNodeIDREF.h"
#include "../util/global_util_functions.h"
#include "../context/Param.h"
#include "../context/ParamValue.h"
#include "../parser/sml.tab.h"
#include "ValueNode.h"
#include <cassert>
#include <cstdlib>
#include <iostream>

using namespace std;

using namespace __gnu_cxx;

int ModelComp::tt_count = 0;  // initialise static class member

const string ModelComp::nameTypes[] = { "variable", "constraint", "parameter", "set", "objective min", "objective max", "submodel" };
const string ModelComp::compTypes[] = { "var", "subject to", "param", "set", "minimize", "maximize", "block" };

//extern void modified_write(ostream &fout, ModelComp *comp);

/* This should be an IDREF (or IDREFM) node that needs to be converted
 into its global name

 The node is a pointer to a ModelComp structure. Need to work out
 which (if any) blocks it belongs to and pre-pend the name of any block to
 the global name

 Also need to work out which dummy variables need to be put on the
 argument list

 IN: ModelComp *node          : the model comp of which the name should
 be obtained
 int        witharg        : 1 if argument list should be printed
 SyntaxNode     *opn           : the IDREF node that should be named
 AmplModel *current_model : the model in which this is referenced

 arguments opn, current_model are only needed if the argument list should
 be printed as well

 opn is the IDREF(M) node of the object that should be named. All the
 subscripts that are used for this in the model description are
 part of the 'opn' node. To get the complete global argument list, these
 subscripts need to be prefixed by the ones corresponding to the
 model from which this object is referenced
 */

/** Construct a model component given its name, id, indexing and attribute
 *  sections.
 *  Also analyses dependencies in indexing and attribute and set the 
 *  dependencies list
 *  @param id_
 *         Name of the component
 *  @param type_
 *         Type of the component
 *  @param indexing_
 *         Root node of the indexing expression
 *                     IDs should have been replaced by IDREFs 
 *  @param attrib
 *         Root node of the attribute expression
 *                     IDs should have been replaced by IDREFs 
 */
ModelComp::ModelComp(const string& id_, compType type_, SyntaxNode *indexing_, SyntaxNode *attrib, int uplevel) :
		type(type_), id(id_), attributes(attrib), model(NULL), setDim(0), setCard(0), isFromFile(false), count(ModelComp::tt_count++), moveUpLevel(uplevel) {

	this->indexing = dynamic_cast<SyntaxNodeIx*>(indexing_);
	if (indexing)
		(this->indexing)->splitExpression();
	LOG( "Creating model component (" << this->count << "): id="<<id<<" indexing="<<this->indexing<<" attribute="<<this->attributes<<" type"<<type);

	/* now set up the dependency list for the component */
	setUpDependencies();

	ostringstream oss;
	oss << this;
	this->hashKey = oss.str();

}

ModelComp::~ModelComp() {
	LOG("enter ModelComp destructor called...["<<this->id<<"]");
	if (attributes != NULL) {
		LOG(" --- delete attributes ");
		//delete attributes;
		attributes = NULL;
	}
	if (indexing != NULL) {
		LOG(" --- delete indexing ");
		//delete indexing;
		indexing = NULL;
	}
	LOG("exit ModelComp destructor called...["<<this->id<<"]");
}

void ModelComp::findDependencies(const SyntaxNode* nd) {
	list<ModelComp*> lmc;
	nd->findIDREF(lmc);
	list<ModelComp*>::const_iterator p, q;
	for(p = lmc.begin();p != lmc.end();++p) {
		// see if element already in the dependencies list
		bool found = false;
		for(q = dependencies.begin();q != dependencies.end();++q)
			if (*p == *q)
				found = true;
		if (!found) {
			dependencies.push_back(*p);
			LOG("  " + (*p)->id);
		}
	}
}

/** Set up the list of dependencies for this component */
void ModelComp::setUpDependencies() {
	LOG("-- In setUpDependencies -- id=" + id);
	dependencies.clear();

	if (indexing) {
		LOG(" dependencies in indexing: " + indexing->print());
		findDependencies(indexing);
	}
	if (attributes) {
		LOG(" dependencies in attributes: " + attributes->print());
		findDependencies(attributes);
	}
	LOG("-- end setUpDependencies --dependencies.size="<<dependencies.size());
}

/* --------------------------------------------------------------------------
 ModelComp::ModelComp()
 ---------------------------------------------------------------------------- */
/** Default constructor: just sets all fields to -1/NULL/false               */
ModelComp::ModelComp(const string& id_) :
		type(TNOTYPE), id(id_), attributes(NULL), indexing(NULL), model(NULL), other(NULL), count(-1), moveUpLevel(0) {
	ostringstream oss;
	oss << this;
	this->hashKey = oss.str();
}

/* ---------------------------------------------------------------------------
 ModelComp::dump(ostream &fout)
 ---------------------------------------------------------------------------- */
/** Print a detailed description of this model component and all its fields */
void ModelComp::dump(ostream& fout) const {
	fout << "MC: ----------------------------------------------------------\n";
	fout << "MC: ModelComp: " << id << " (" << (void *) this << ")\n";
	fout << "    type: " << ModelComp::nameTypes[type] << "\n";
	if (attributes) {
		fout << "    attr: " << attributes << '\n';
		// fout << "       ";
		// attributes->dump(fout);
	}
	if (indexing) {
		fout << "    indexing: " << indexing << "\n";
		indexing->splitExpression();
		indexing->printDiagnostic(fout);
		indexing->dump(fout);
	}
	fout << "    dependencies (" << dependencies.size() << "):\n";
	for(list<ModelComp*>::const_iterator p = dependencies.begin();p != dependencies.end();++p)
		fout << "       " << (*p)->model->name << "::" << (*p)->id << endl;
	fout << "    model: " << model->name << "\n";
	fout << "    count: " << count << "\n";
}

/* ---------------------------------------------------------------------------
 ModelComp::deep_copy()
 ---------------------------------------------------------------------------- */
/** Create a deep-copy of the ModelComp object.
 *
 *  The tree of attributes and indexing expressions is recreated using 
 *  entirely new objects.
 */
ModelComp *
ModelComp::deep_copy() const {
	LOG("start -- deep_copy -- ["<<this->id<<"]");
	ModelComp *newm = new ModelComp(id);

	newm->type = type;
	if (attributes)
		newm->attributes = attributes->deep_copy();
	if (indexing)
		newm->indexing = indexing->deep_copy();
	newm->dependencies = dependencies;
	newm->model = model;
	newm->other = other;
	newm->count = count;

	LOG("end -- deep_copy -- ["<<this->id<<"]");
	return newm;
}
/* ---------------------------------------------------------------------------
 ModelComp::clone()
 ---------------------------------------------------------------------------- */
/** Create a shallow copy of the object: only the top level object is 
 *  copied, pointers below are reused 
 */
ModelComp *
ModelComp::clone() const {

	ModelComp *newm = new ModelComp(id);

	newm->type = type;
	newm->attributes = attributes;
	newm->indexing = indexing;
	newm->dependencies = dependencies;
	newm->model = model;
	newm->other = other;
	newm->count = count;

	return newm;
}

/* --------------------------------------------------------------------------
 ModelComp::moveUp(int level)
 ---------------------------------------------------------------------------- */
/** Queue the ModelComp to be moved up by 'level' levels in the model tree:
 *  Just removing the component from the current model and adding it to a
 *  parent is dangerous, since ModelComp::moveUp is typically called from
 *  within a (nested) loop over all ModelComps (->comps) in the AmplModels
 *  removing/adding items to list<ModelComp*> comps while there is an
 *  iterator running over it will invalidate that iterator.
 *  => hence the request to move is scheduled to be executed by
 *     AmplModel::applyChanges() after the loop over all components
 *
 *  This method will also re-write the component for the new model
 *  I.e. all IDREFs to components below the new model will have their
 *  local indexing expression expanded
 */
void ModelComp::moveUp(int level) {
	LOG( "enter moveUp --["<<level<<"]-- id["<<this->id<<"] indexing["<<this->indexing->print()<<"] attribute["<<this->attributes->print()<<"]");
	AmplModel *current = model;
	int i, posm;

	// -------------------- Expand local indexing expression -----------------
	/* This ModelComp is written for the 'current' model and is now re-assigned
	 to a different model. In order for that to work the indexing expressions
	 in all IDREFs in its attribute/indexing section have got to be
	 rewritten.

	 Indexing expressions applicable to a IDREF are divided into local and
	 block indexing. 'local' is directly associated with the IDREF (as
	 arguments in ->values[]. 'block' originate from the indexing expressions
	 of the blocks up to the current model in the model tree. Both indexing
	 expression together need to combine to get the correct global indexing.

	 When moving a ModelComp up in the tree, we therefore need to do the
	 following to have correct global indexing:
	 - all IDREFs to ModelComp's in models below the new model
	 (current.parent(level)) need to have the block indexing expressions
	 between their own model and current.parent(level) added
	 to their local indexing
	 */

	// get list of models from current model to root
	vector<AmplModel*> mlist;
	int nlevels = 0;
	for(AmplModel *tmp = current;tmp->parent != NULL;tmp = tmp->parent) {
		LOG("adding model["<<tmp->name<<"] to list");
		mlist.push_back(tmp);
		nlevels++;
	}
	// it's possible to move the model up by at most as many levels as there
	// are from here to the root
	assert(nlevels - level > 0);

	// get list of all IDREF nodes in dependencies
	list<SyntaxNode*> idrefnodes;
	if (indexing)
		indexing->findIDREF(&idrefnodes);
	if (attributes)
		attributes->findIDREF(&idrefnodes);

//	//--- All the depended ModelComp also should be moved up --Feng (two parts fix)
//	vector<AmplModel*> upModelList;
//	int upLevel = level;
//	for(AmplModel* tmp=current;tmp->parent!=NULL;tmp=tmp->parent)
//	{
//		if(upLevel == 0)
//		{
//			LOG("adding model["<<tmp->name<<"] to upModelList");
//			upModelList.push_back(tmp);
//		}
//		else
//		{
//			upLevel--;
//		}
//	}
//	//end fix -- part 1

	// loop over all IDREF nodes
	list<SyntaxNode*>::const_iterator p;
	for(p = idrefnodes.begin();p != idrefnodes.end();++p) {
		SyntaxNodeIDREF *currSN = dynamic_cast<SyntaxNodeIDREF*>(*p);
		ModelComp *currMC = currSN->ref;
		AmplModel *currAM = currMC->model;
		LOG( "-- Moveup -- at currSN["<<currSN->print()<<"] currMC["<<currMC->id<<"] currAM["<<currAM->name<<"]");

		// need to check if this model is below the new assigned model
		bool found = false;
		for(posm = 0;posm < level;posm++) {
			if (mlist[posm] == currAM) {
				found = true;
				break;
			}
		}
		if (found) {
			LOG( "found currMC["<<currMC->id<<"] currAM["<<currAM->name<<"] syntaxNode["<<currSN->print()<<"] at posm["<<posm<<"]");
			// this is a model between the old and new model for ModelComp *this
			// posm gives the position: 0 is the old model, level is the new
			// one
			// => need to add indexing expressions between posm and level-1
			// starting with level-1
			for(i = posm;i < level;++i) {
				SyntaxNodeIx *mix = mlist[i]->ix;
				if (mix->getNComp() != 1) {
					cerr << "ModelComp::moveUp() does not support intermediate models with !=1 dummy Var" << endl;
					exit(1);
				}
				currSN->push_front(mix->getDummyVarExpr(0));
				/* indexing dummy var of mlist[level-1-i]*/
				LOG("["<<i<<"] - now syntaxNode["<<currSN->print()<<"]");
			}
		}

//		bool isDefined = false;
//		for(vector<AmplModel*>::iterator it=upModelList.begin();it!=upModelList.end();it++)
//		{
//			//hack!
//			if(currAM->name.compare("root")==0)
//			{
//				LOG("currMC["<<currMC->id<<"] is in root! ");
//				isDefined = true;
//				break;
//			}
//			if((*it)==currAM)
//			{
//				LOG("currMC["<<currMC->id<<"] is defined ");
//				isDefined = true;
//				break;
//			}
//		}
//		if(!isDefined)
//		{
//			LOG("currMC["<<currMC->id<<"] is not defined! ");
//			changeitem rem = {currMC,currAM,CHANGE_REM};
//			changeitem add = {currMC,mlist[level],CHANGE_ADD};
//			AmplModel::changes.push_back(rem);
//			AmplModel::changes.push_back(add);
//		}
	}
	this->moveUpLevel = level;
	LOG("setting for ModelComp["<<this->id<<"] moveUpLevel["<<this->moveUpLevel<<"]");
	// and queue this item to be moved up by AmplModel::applyChanges
	changeitem rem = { this, model, CHANGE_REM };
	AmplModel::changes.push_back(rem); // Q for removal
	model = mlist[level];
	changeitem add = { this, model, CHANGE_ADD };
	AmplModel::changes.push_back(add);
	LOG("exit moveUp --["<<level<<"]-- id["<<this->id<<"] indexing["<<this->indexing->print()<<"] attribute["<<this->attributes->print()<<"]");
}

/* --------------------------------------------------------------------------
 ModelComp::reassignDependencies()
 ---------------------------------------------------------------------------- */
/** Recalculate dependency list and re-resolve IDREF nodes.
 *
 *  In the process of building the AmplModel tree from the StochModelTree
 *  some of the IDREF dependency nodes still point to the StochModelComp
 *  nodes from the StochModel tree (or the intermediate tree).
 *
 *  This routine makes sure that IDREF nodes are resolved with respect to the 
 *  correct ModelComp and rebuilds the dependency lists.
 */
void ModelComp::reassignDependencies() {
	list<SyntaxNode*> idrefnodes;
	list<ModelComp*> newdep;

	if (indexing)
		indexing->findIDREF(&idrefnodes);
	if (attributes)
		attributes->findIDREF(&idrefnodes);

	list<SyntaxNode*>::const_iterator p;
	for(p = idrefnodes.begin();p != idrefnodes.end();++p) {
		SyntaxNodeIDREF *onidr = dynamic_cast<SyntaxNodeIDREF*>(*p);
		ModelComp *mc = onidr->ref;
		AmplModel *am = mc->model;

		//check that this ModelComp belongs to this model
		bool found = false;
		for(list<ModelComp*>::iterator q = am->comps.begin();q != am->comps.end();q++) {
			if ((*q)->id == mc->id) {
				found = true;
				if ((*q) != mc) {
					LOG( "Model component " << mc->id << " referenced in "<< this->id << " is reassigned.");
					onidr->ref = (*q);
				}
				newdep.push_back(*q);
			}
		}
		if (!found) {
			cerr << "ERROR: Model component " << mc->id << " referenced in " << this->id << " not found.\n";
			exit(1);
		}
	}
	dependencies = newdep;
	if (indexing) {
		indexing->resetSplitExpression();
		indexing->splitExpression();
	}
}

/***************************************************************************************
 Set related operation evaluation
 Feng
 ***************************************************************************************/
void ModelComp::setSetDim() {
	assert(this->type == TSET);
	assert(this->indexing==NULL);

	this->setDim = 0;
	if (this->attributes != NULL) {
		SyntaxNode* attr = this->attributes;
		setDim = attr->calculateSetDim();
	}
	else {
		setDim = 1;
	}
}

int ModelComp::getSetDim() {
	assert(this->type==TSET);
	return setDim;
}

void ModelComp::setSetCard(int card) {
	assert(this->type==TSET);
	setCard = card;
}

int ModelComp::getSetCard() {
	assert(this->type==TSET);
	return setCard;
}

void ModelComp::setNumVarIndicies(int num) {
	assert(this->type==TVAR);
	this->numVarIndicies = num;
}

int ModelComp::getNumVarIndicies() {
	assert(this->type==TVAR);
	return numVarIndicies;
}

// param related operations
void ModelComp::setParamIndicies() {
	assert(this->type==TPARAM);
	if (this->indexing != NULL) {
		SyntaxNodeIx* ind = this->indexing;
		ind->calculateParamIndicies(this->paramIndiciesDummy, this->paramIndiciesComp, this->paramIndiciesMap);
		assert(this->paramIndiciesDummy.size()==this->paramIndiciesComp.size());
	}
	else {
		assert(this->paramIndiciesDummy.size()==0);
		//this->paramIndicies = NULL;
	}
}

int ModelComp::getNumParamIndicies() {
	assert(this->type==TPARAM);
	assert(this->paramIndiciesDummy.size()==this->paramIndiciesComp.size());
	return this->paramIndiciesDummy.size();
}

void ModelComp::calculateSetModelComp(ModelContext* context) {
	assert(this->type==TSET);
	assert(this->indexing == NULL);
	//set define may not have a indexing
	assert(this->attributes != NULL);

	Set* theSet = this->attributes->calculateSetValue(context);
	LOG( "the calculated set is   -- ["<<theSet->name<<"]  -- "<<theSet->toString());
	this->setCard = theSet->card;
	assert(theSet->dim==this->setDim);
	theSet->name = this->id;
	theSet->dim = this->setDim;
	context->addCompValueMap(this, theSet);
}

void ModelComp::calculateParamModelComp(ModelContext* context) {
	assert(this->type==TPARAM);
	//for every indexing = evaluate the attribute value
	Param* paramValue = new Param(this->getNumParamIndicies(), this->id);
	int total = 1;
	vector<ModelComp*>::iterator i;
	for(i = this->paramIndiciesComp.begin();i != this->paramIndiciesComp.end();i++) {
		ModelComp* comp = (*i);
		assert(comp->type==TSET);
		Set* aSet = static_cast<Set*>(context->getCompValue(comp));
		total = total * aSet->card;
	}

	vector<string>::iterator i2;
	for(int j = 0;j < total;j++) {
		int curr = j;
		hash_map<string, string> dummyValueMap;
		vector<string> indicies;

		for(i = this->paramIndiciesComp.begin(), i2 = this->paramIndiciesDummy.begin()
		;i != this->paramIndiciesComp.end();i++, i2++) {
			ModelComp* comp = *i;
			Set* aSet = static_cast<Set*>(context->getCompValue(comp));
			LOG(" at set -- "<<aSet->name);
			int index = curr % aSet->card;
			curr = curr / aSet->card;
			string dummy = (*i2);
			string key = aSet->setKeys.find(index + 1)->second;
			LOG("add dummay map ["<<dummy<<"] ---> ["<<key<<"]");
			dummyValueMap.insert(pair<string, string>(dummy, key));
			indicies.push_back(key);
		}
		//now calculate the double value
		double dval = this->attributes->calculateParamValue(dummyValueMap, paramIndiciesMap, context);
		paramValue->addParamValue(new ParamValue(indicies, dval));
	}
	context->addCompValueMap(this, paramValue);
}

void ModelComp::calculateLocalVar(ModelContext* context, Var* aVar) {
	LOG( "calculateVarCard -- in model["<<this->model->name<<"] modelcomp["<<this->id<<"]aVar["<<aVar->name<<"] card["<<aVar->card<<"]");
	assert(this->type==TVAR);
	int card = 1;
	int numIndices = 0;
	vector<string> ind;
	if (context != NULL) {
		context->fillDummyValue(ind);
	}
	numIndices = ind.size();
	if (this->indexing == NULL) {
		//do nothing.. card = 1,numIndicies = num of model's indices
	}
	else if (this->indexing->sets_mc.size() > 0) {
		for(vector<ModelComp*>::iterator it = this->indexing->sets_mc.begin();it != this->indexing->sets_mc.end();it++) {
			Set* aSet = static_cast<Set*>(context->getCompValue(*it));
			card = aSet->card * card;
			numIndices = numIndices + aSet->dim;
		}
	}
	aVar->card = card;
	aVar->numIndicies = numIndices;
	LOG( "calculateVarCard -- in model["<<this->model->name<<"] modelcomp["<<this->id<<"]aVar["<<aVar->name<<"] card["<<aVar->card<<"] numIndicies["<<aVar->numIndicies<<"]");
}

void ModelComp::fillLocalVar(ModelContext* context, Var* aVar) {
	LOG( "fillLocalVar -- in model["<<this->model->name<<"] modelcomp["<<this->id<<"]aVar["<<aVar->name<<"] card["<<aVar->card<<"]");
	assert(this->type==TVAR);
	double lower = -dINF;
	double upper = dINF;
	if (this->attributes != NULL) {
		LOG( "setting variable bounds -- attribute opCode["<<this->attributes->opCode<<"]  ["<<this->attributes->print()<<"]");
		assert(this->attributes->opCode == COMMA);
		SyntaxNode* commaNode = this->attributes;
		if (commaNode->values[0]->opCode == GE) {
			SyntaxNode* geNode = commaNode->values[0];
			assert(geNode->values[0]->opCode==-99);
			ValueNode* vNode = static_cast<ValueNode*>(geNode->values[0]);
			lower = vNode->value;
		}
		else {
			LOG("other bounds not yet implemented");
			assert(false);
		}
	}
	if (this->indexing == NULL) {
		LOG("this["<<this->id<<"] has no indexing over");
		vector<string> ind;
		context->fillDummyValue(ind);
		//fix -- shouldn't fill the id, id is capture in modelComp
		//ind.push_back(this->id);
		VarValue* val = new VarValue(ind, lower, upper);
		aVar->addVarValue(val);
	}
	else if (this->indexing->sets_mc.size() > 0) {
		LOG("Indexing -- ["<<this->indexing->print()<<"]");
		LOG("sets_mc["<<this->indexing->sets_mc.size()<<"]");
		vector<string> ind;
		if (context != NULL) {
			context->fillDummyValue(ind);
		}
		vector<ModelComp*>::iterator curr = this->indexing->sets_mc.begin();
		this->fillLocalVarRecurive(context, aVar, curr, ind, lower, upper);
	}
	LOG( "end fillLocalVar -- in ["<<this->id<<"]aVar["<<aVar->name<<"] - card["<<aVar->card<<"]");
}

void ModelComp::fillLocalVarRecurive(ModelContext* context, Var* aVar, vector<ModelComp*>::iterator curr, vector<string>& ind, double lower, double upper) {
	if (curr == this->indexing->sets_mc.end()) {
		VarValue* val = new VarValue(ind, lower, upper);
		aVar->addVarValue(val);
	}
	else {
		LOG("-- fillLocalVar Recursive -- curr ModelComp["<<(*curr)->id<<"]");
		Set* aSet = static_cast<Set*>(context->getCompValue(*curr));
		for(vector<SetValue*>::iterator it_setval = aSet->setValues_data_order.begin();it_setval != aSet->setValues_data_order.end();it_setval++) {
			int size_before = ind.size();
			(*it_setval)->fillValueList(ind);

			if (curr != this->indexing->sets_mc.end()) {
				this->fillLocalVarRecurive(context, aVar, curr + 1, ind, lower, upper);
			}

			ind.resize(size_before);
		}
	}
}

void ModelComp::calculateMemoryUsage(unsigned long& size) {
	LOG_MEM("ModelComp::calculateMemoryUsage -- comp["<<this->id<<"]");
	size += sizeof(ModelComp);
	if (this->attributes != NULL) {
		LOG_MEM(" --- attribute ["<<this->attributes->print()<<"]");
		this->attributes->calculateMemoryUsage(size);
	}
	if (this->indexing != NULL) {
		LOG_MEM(" --- indexing ["<<this->indexing->print()<<"]");
		this->indexing->calculateMemoryUsage(size);
	}

	for(list<ModelComp*>::iterator it = dependencies.begin();it != dependencies.end();it++) {
		size += sizeof(ModelComp*);
	}
}

string& ModelComp::getHashKey() {
	return this->hashKey;
}

