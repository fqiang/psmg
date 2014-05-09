/*
 * Context.cpp
 *
 *  Created on: 10 Oct 2011
 *      Author: s0965328
 */

#include <vector>
#include <sstream>

#include "ModelContext.h"
#include "ExpandedModel.h"
#include "SetSimple.h"
#include "IndexSet.h"

#include "../model/SyntaxNode.h"
#include "../model/ModelComp.h"
#include "../util/util.h"


using namespace std;

static string CONTEXT_ID_NOT_INIT="NOT_INITIALIZED";

ModelContext::ModelContext(ModelContext* par):
		parent(par),em(NULL)
{
	LOG("ModelContext - constructor - ");
	//assert(_parent!=NULL);
}

ModelContext::~ModelContext()
{
	LOG("Delete ModelContext["<<em->name<<"]");

	//must delete IndexSet before CompDescr* map
	//because IndexSet only delete a Set if its name == "TMP_" , once the CompDescr* deleted
	//it is not possible for IndexSet to test name=="TMP_" for the set already deleted
	for(boost::unordered_map<SyntaxNode*,IndexSet*>::iterator it=tempISetMap.begin();it!=tempISetMap.end();it++)
	{
		LOG("Deleting -------  IndexSet in Context "<<this->getContextId()<<" -- ["<<(*it).first->print()<<"]");
		delete it->second;
	}
	tempISetMap.clear();

	//delete CompDescr* map after IndexSet map
	for(boost::unordered_map<ModelComp*,CompDescr*>::iterator it=compValueMap.begin();it!=compValueMap.end();it++)
	{
		delete it->second;
	}
	compValueMap.clear();
	dummyTempMap.clear();
	dummyEmcolTempMap.clear();
//	dummyCompMapTemp.clear();
//	dummyValueMapTemp.clear();
}

string ModelContext::getContextId()
{
	return this->em==NULL? CONTEXT_ID_NOT_INIT:em->name;
}

void ModelContext::addDummyEmcolTempMap(string& dummyVar, string& value)
{
	pair<boost::unordered_map<string,string>::iterator,bool> ret1;
	ret1 = this->dummyEmcolTempMap.insert(pair<string,string>(dummyVar,value));
	assert(ret1.second);
}

void ModelContext::removeDummyEmcolTempMap(string& dummyVar)
{
	boost::unordered_map<string,string>::iterator ret1;
	ret1 = this->dummyEmcolTempMap.find(dummyVar);
	assert(ret1!=this->dummyEmcolTempMap.end()); //has to be exist

	this->dummyEmcolTempMap.erase(dummyVar);
}
void ModelContext::addDummyCompValueMapTemp(string& dummyVar,ModelComp* comp,string& value)
{
	pair<boost::unordered_map<string,dummy_tmp_t>::iterator,bool> ret1;
	dummy_tmp_t a_dummy_tmp = std::make_pair<ModelComp*,string>(comp,value);
	ret1 = this->dummyTempMap.insert(pair<string,dummy_tmp_t>(dummyVar,a_dummy_tmp));
	assert(ret1.second);
//	pair<boost::unordered_map<string,string>::iterator,bool> ret1;
//	pair<boost::unordered_map<string,ModelComp*>::iterator,bool> ret2;
//	ret1 = this->dummyValueMapTemp.insert(pair<string,string>(dummyVar,value));
//	ret2 = this->dummyCompMapTemp.insert(pair<string,ModelComp*>(dummyVar,comp));
//	assert(ret1.second && ret2.second );
}

void ModelContext::removeDummySetValueMapTemp(string& dummyVar)
{
	boost::unordered_map<string,dummy_tmp_t>::iterator ret1;
	ret1 = this->dummyTempMap.find(dummyVar);
	assert(ret1!=this->dummyTempMap.end()); //has to be exist

	this->dummyTempMap.erase(dummyVar);

//	this->dummyValueMapTemp.erase(dummyVar);
//	this->dummyCompMapTemp.erase(dummyVar);
}

string ModelContext::getDummyValue(string& dummyVar)
{
	//looking into expanded model indexing
	boost::unordered_map<string,model_dummy_t>::iterator it1 = this->em->dummyMap.find(dummyVar);
	string rval = it1==this->em->dummyMap.end()? "":it1->second.second;

	//looking into the emcol temporary map if not found in model index
	if(rval.empty() && dummyEmcolTempMap.size()!=0){
		boost::unordered_map<string,string>::iterator it2 = dummyEmcolTempMap.find(dummyVar);
		rval = it2==dummyEmcolTempMap.end()?"":it2->second;
	}

	//looking into the temporary map if not found in above
	if(rval.empty())
	{
		boost::unordered_map<string,dummy_tmp_t>::iterator it3 = this->dummyTempMap.find(dummyVar);
		rval = it3==this->dummyTempMap.end()?"":it3->second.second;
//		boost::unordered_map<string,string>::iterator it3 = this->dummyValueMapTemp.find(dummyVar);
//		rval = it3==this->dummyValueMapTemp.end()?"":it3->second;
	}

	//looking into the parent if it still not found
	if(rval.empty() && this->parent != NULL)
	{
		rval = this->parent->getDummyValue(dummyVar);
	}
	else if(rval.empty() && this->parent == NULL)
	{
		LOG("****MODELLING ERROR*****  root node context reached - but not found - dummyVar["<<dummyVar<<"]");
		assert(false);
	}
	return rval;
}
ModelComp* ModelContext::getDummyComp(string& dummyVar)
{
	boost::unordered_map<string,model_dummy_t>::iterator it1 = this->em->dummyMap.find(dummyVar);
	ModelComp* rval = it1==this->em->dummyMap.end()? NULL:it1->second.first;

	if(rval==NULL){
		boost::unordered_map<string,dummy_tmp_t>::iterator it3 = this->dummyTempMap.find(dummyVar);
		rval = it3==this->dummyTempMap.end()?NULL:it3->second.first;
//		boost::unordered_map<string,ModelComp*>::iterator it3 = this->dummyCompMapTemp.find(dummyVar);
//		rval = it3==this->dummyCompMapTemp.end()?NULL:it3->second;
	}
	if(rval==NULL && this->parent != NULL)
	{
		rval = this->parent->getDummyComp(dummyVar);
	}
	else if(rval==NULL && this->parent == NULL)
	{
		LOG("****WARNING*****  root node context reached - but not found - dummyVar["<<dummyVar<<"]  *NULL* returned");
		assert(false);
	}
	return rval;
}

CompDescr* ModelContext::getCompValue(ModelComp* comp)
{
	LOG("getCompValue - contextID["<<this->getContextId()<<"] comp["<<comp->name<<"]");
	boost::unordered_map<ModelComp*,CompDescr*>::iterator it = this->compValueMap.find(comp);
	CompDescr* rval = it==this->compValueMap.end()? NULL:it->second;
	if(rval==NULL && this->parent!=NULL)
	{
		rval = this->parent->getCompValue(comp);
	}
	else if(rval==NULL && this->parent==NULL)
	{
		LOG("root node context reached - but not found - comp["<<comp->name<<"]  *NULL* returned - to be initialized.");
	}
	return rval;
}

void ModelContext::addCompValueMap(ModelComp* comp,CompDescr* value)
{
	pair<boost::unordered_map<ModelComp*,CompDescr*>::iterator,bool> ret;
	LOG("addCompValueMap - contextID["<<this->getContextId()<<"]comp["<<comp->name<<"]");
	ret = this->compValueMap.insert(pair<ModelComp*,CompDescr*>(comp,value));
	assert(ret.second);
}

bool ModelContext::getCalcSumSet(SyntaxNode* key,IndexSet** iset)
{
	bool rval = false;
	LOG("getCalcTempSet -- look for hashKey["<<key->print()<<"] ");
	boost::unordered_map<SyntaxNode*,IndexSet* >::iterator it_iset = this->tempISetMap.find(key);
	if(it_iset!=this->tempISetMap.end())
	{
		rval = true;
		*iset = it_iset->second;
	}
	return rval;
}

//
void ModelContext::addCalcSumSet(SyntaxNode* key,IndexSet* iset)
{
	LOG("addCalcSumSet  hashkey["<<key->print()<<"] -- set -- "+iset->toString());

	boost::unordered_map<SyntaxNode*,IndexSet* >::iterator it_mcs = this->tempISetMap.find(key);
	assert(it_mcs==this->tempISetMap.end()); //must not already exist

	this->tempISetMap[key] = iset;
}

void ModelContext::calculateMemoryUsage(unsigned long& size)
{
	size += sizeof(ModelContext);

	for(boost::unordered_map<ModelComp*,CompDescr*>::iterator it=compValueMap.begin();it!=compValueMap.end();it++)
	{
		size += sizeof(pair<ModelComp*,CompDescr*>);
		(*it).second->calculateMemoryUsage(size);
	}

	for(boost::unordered_map<string,dummy_tmp_t>::iterator it=dummyTempMap.begin();it!=dummyTempMap.end();it++)
	{
		size += sizeof(pair<string,dummy_tmp_t>);
		size += (*it).second.second.size() + 1;
		size += (*it).first.size() + 1;
	}

//	for(boost::unordered_map<string,string>::iterator it=dummyValueMapTemp.begin();it!=dummyValueMapTemp.end();it++)
//	{
//		size += sizeof(pair<string,string>);
//		size += (*it).first.size() + 1;
//		size += (*it).second.size() + 1;
//	}

	for(boost::unordered_map<SyntaxNode*,IndexSet*>::iterator it = tempISetMap.begin();it!=tempISetMap.end();it++)
	{
		size += sizeof(pair<SyntaxNode*,IndexSet*>);
		(*it).second->calculateMemoryUsage(size);
	}
}

