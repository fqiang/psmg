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
#include "SyntaxNodeIDREF.h"
#include "../util/global_util_functions.h"
#include "../context/Param.h"
#include "../context/ExpandedModel.h"
#include "../parser/sml.tab.h"
#include "SyntaxNodeValue.h"
#include <cassert>
#include <cstdlib>
#include <iostream>

using namespace std;

using namespace __gnu_cxx;

const string ModelComp::nameTypes[] = { "variable", "constraint", "parameter", "set", "objective", "submodel" };


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
ModelComp::ModelComp(const string& id, compType type, SyntaxNode* indexing, SyntaxNode *attribute)
	:name(id), type(type), indexing(indexing),attributes(attribute), model(NULL)
{

	LOG("ModelComp constructor  - id=["<<id<<"] indexing["<<this->indexing<<"] attribute["<<this->attributes<<"] type["<<type<<"]");

//	ostringstream oss;
//	oss << this;
//	this->hashKey = oss.str();
}

ModelComp::~ModelComp() {
	LOG("enter ModelComp destructor called...["<<this->name<<"]");
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
	LOG("exit ModelComp destructor called...["<<this->name<<"]");
}
//
//string& ModelComp::getHashKey() {
//	return this->hashKey;
//}

void ModelComp::calculateMemoryUsage(unsigned long& size) {
	LOG_MEM("ModelComp::calculateMemoryUsage -- comp["<<this->name<<"]");
	size += sizeof(ModelComp);
	if (this->attributes != NULL) {
		LOG_MEM(" --- attribute ["<<this->attributes->print()<<"]");
		this->attributes->calculateMemoryUsage(size);
	}
	if (this->indexing != NULL) {
		LOG_MEM(" --- indexing ["<<this->indexing->print()<<"]");
		this->indexing->calculateMemoryUsage(size);
	}
}





//legacy!!

/* --------------------------------------------------------------------------
 ModelComp::ModelComp()p
 ---------------------------------------------------------------------------- */
/** Default constructor: just sets all fields to -1/NULL/false               */
//ModelComp::ModelComp(string _id)
//	:type(TNOTYPE), id(_id), attributes(NULL), indexing(NULL), model(NULL)
//{
//	ostringstream oss;
//	oss << this;
//	this->hashKey = oss.str();
//}
///* ---------------------------------------------------------------------------
// ModelComp::deep_copy()
// ---------------------------------------------------------------------------- */
///** Create a deep-copy of the ModelComp object.
// *
// *  The tree of attributes and indexing expressions is recreated using
// *  entirely new objects.
// */
//ModelComp *
//ModelComp::deep_copy() const {
//	LOG("start -- deep_copy -- ["<<this->id<<"]");
////	ModelComp *newm = new ModelComp(id);
////
////	newm->type = type;
////	if (attributes)
////		newm->attributes = attributes->deep_copy();
////	if (indexing)
////		newm->indexing = indexing->deep_copy();
//////	newm->dependencies = dependencies;
////	newm->model = model;
////
////	LOG("end -- deep_copy -- ["<<this->id<<"]");
////	return newm;
//}
//
//
//
///* ---------------------------------------------------------------------------
// ModelComp::clone()
// ---------------------------------------------------------------------------- */
///** Create a shallow copy of the object: only the top level object is
// *  copied, pointers below are reused
// */
//ModelComp *ModelComp::clone() const {
//
////	ModelComp *newm = new ModelComp(id);
////
////	newm->type = type;
////	newm->attributes = attributes;
////	newm->indexing = indexing;
//////	newm->dependencies = dependencies;
////	newm->model = model;
////
////	return newm;
//}


/*create varDeps map , each entry in the map is the pair
	(level -> levels) : level is the column level for a intersection block
						levels are the set of int that corresponding to the list of levels required for evaluate
						the derivatives of a intersection block.
*/
//void ModelComp::analyseVarDepLevelsInCons()
//{
//	assert(this->type == TCON || this->type == TMAX || this->type==TMIN);
//	LOG("ModelComp::analyseConstraint --id["<<id<< "] attr["<<this->attributes->print()<<"] declared level["<<this->model->level<<"] ["<<this<<"]");
//	set<int> levels;
//	this->attributes->calcVarDefinedLevels(levels);
//
//
//	for(set<int>::iterator it=levels.begin();it!=levels.end();it++)
//	{
//		int level = *it;
//		set<int> deps;
//		this->attributes->calcSeparability(level,deps);
//		this->varDeps.insert(pair<int,set<int> >(level,deps));
//
//		LOG("Variable Declare Level: "<<level);
//		for(set<int>::iterator it2=deps.begin();it2!=deps.end();it2++)
//		{
//			LOG("	   Depend Level: "<<*it2);
//		}
//
//	}
//}

/*convert this modelcomp constraint to an expression tree in autodiff library.
return the root node of the expression tree.
*/
//Node* ModelComp::constructAutoDiffCons(ModelContext* ctx, Block* emb,ExpandedModel* emcol)
//{
//	assert(this->type == TCON);
//	LOG("constructAutoDiffCons - modelcomp["<<this->id<<"] ctx["<<ctx->getContextId()<<"]");
//	Node* node = this->attributes->constructAutoDiffNode(ctx,emb,emcol);
//	return node;
//}


//void ModelComp::findDependencies(const SyntaxNode* nd) {
//	list<ModelComp*> lmc;
//	nd->findIDREF(lmc);
//	list<ModelComp*>::const_iterator p, q;
//	for(p = lmc.begin();p != lmc.end();++p) {
//		// see if element already in the dependencies list
//		bool found = false;
//		for(q = dependencies.begin();q != dependencies.end();++q)
//			if (*p == *q)
//				found = true;
//		if (!found) {
//			dependencies.push_back(*p);
//			LOG("  " + (*p)->id);
//		}
//	}
//}
//
///** Set up the list of dependencies for this component */
//void ModelComp::setUpDependencies() {
//	LOG("-- In setUpDependencies -- id=" + id);
//	dependencies.clear();
//
//	if (indexing) {
//		LOG(" dependencies in indexing: " + indexing->print());
//		findDependencies(indexing);
//	}
//	if (attributes) {
//		LOG(" dependencies in attributes: " + attributes->print());
//		findDependencies(attributes);
//	}
//	LOG("-- end setUpDependencies --dependencies.size="<<dependencies.size());
//}


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
//void ModelComp::reassignDependencies() {
//	list<SyntaxNode*> idrefnodes;
//	list<ModelComp*> newdep;
//
//	if (indexing)
//		indexing->findIDREF(&idrefnodes);
//	if (attributes)
//		attributes->findIDREF(&idrefnodes);
//
//	list<SyntaxNode*>::const_iterator p;
//	for(p = idrefnodes.begin();p != idrefnodes.end();++p) {
//		SyntaxNodeIDREF *onidr = dynamic_cast<SyntaxNodeIDREF*>(*p);
//		ModelComp *mc = onidr->ref;
//		AmplModel *am = mc->model;
//
//		//check that this ModelComp belongs to this model
//		bool found = false;
//		for(vector<ModelComp*>::iterator q = am->all_comps.begin();q != am->all_comps.end();q++) {
//			if ((*q)->id == mc->id) {
//				found = true;
//				if ((*q) != mc) {
//					LOG( "Model component " << mc->id << " referenced in "<< this->id << " is reassigned.");
//					onidr->ref = (*q);
//				}
//				newdep.push_back(*q);
//			}
//		}
//		if (!found) {
//			cerr << "ERROR: Model component " << mc->id << " referenced in " << this->id << " not found.\n";
//			exit(1);
//		}
//	}
//	dependencies = newdep;
//	if (indexing) {
//		indexing->resetSplitExpression();
//		indexing->splitExpression();
//	}
//}

//void ModelComp::setSetCard(int card) {
//	assert(this->type==TSET);
//	setCard = card;
//}
//
//int ModelComp::getSetCard() {
//	assert(this->type==TSET);
//	return setCard;
//}
//
//void ModelComp::setNumVarIndicies(int num) {
//	assert(this->type==TVAR);
//	this->numVarIndicies = num;
//}
//
//int ModelComp::getNumVarIndicies() {
//	assert(this->type==TVAR);
//	return numVarIndicies;
//}
