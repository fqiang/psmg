/*
 * Context.cpp
 *
 *  Created on: 10 Oct 2011
 *      Author: s0965328
 */

#include "ModelContext.h"
#include "ExpandedModel2.h"
#include "../model/ModelComp.h"
#include "assert.h"
#include "../util/global_util_functions.h"
#include "Set.h"

#include <ext/hash_map>
#include <vector>
#include <sstream>

using namespace std;

static string CONTEXT_ID_NOT_INIT="NOT_INITIALIZED";

ModelContext::ModelContext(ModelContext* _parent): parent(_parent),
		//modelDummyValKey(""),isModelDummyValKeyCalculated(false),
		moveUpLevel(0),isCompsCalculated(false),em(NULL),
		varNameReady(false),conNameReady(false),isUsed(true)
{
	LOG("ModelContext - constr - parent_ctx["<<parent<<"]");
	//assert(_parent!=NULL);
}

ModelContext::~ModelContext() {
	for(__gnu_cxx::hash_map<string,CompDescr*>::iterator it=compValueMap.begin();it!=compValueMap.end();it++)
	{
		delete it->second;
	}

	dummySetMapCons.clear();
	dummyValueMapCons.clear();
	dummySetMapTemp.clear();
	dummyValueMapTemp.clear();
	cacheModelDummyVarKey.clear();
	tempISetMap.clear();
//	tempSetModelCompMap.clear();
//	for(__gnu_cxx::hash_map<string,Set*>::iterator it=tempSetSetMap.begin();it!=tempSetSetMap.end();it++)
//	{
//		delete it->second;
//	} -- segfault because 1. copy set 2. complete set from compDescr.  -- Feng
//	tempSetDummyVarMap.clear();
//	tempSetSetMap.clear();
	localConNames.clear();
	localVarNames.clear();
	dummySetMap.clear();
	dummyValueMap.clear();
}

void ModelContext::recursiveMarkContextUsed()
{
	if(!isUsed){
		this->isUsed = true;
		if(parent!=NULL)
		{
			parent->recursiveMarkContextUsed();
		}
	}
}

void ModelContext::addDummySetValueMap(string& dummyVar,ModelComp* comp,string value)
{
	LOG("add model dummy ["<<dummyVar<<"] value["<<value<<"]");
	pair<__gnu_cxx::hash_map<string,string>::iterator,bool> ret1;
	pair<__gnu_cxx::hash_map<string,ModelComp*>::iterator,bool> ret2;
	ret1 = this->dummyValueMap.insert(pair<string,string>(dummyVar,value));
	ret2 = this->dummySetMap.insert(pair<string,ModelComp*>(dummyVar,comp));
	assert(ret1.second && ret2.second);
}

void ModelContext::addDummySetValueMapCons(string& dummyVar,ModelComp* comp,string value)
{
	pair<__gnu_cxx::hash_map<string,string>::iterator,bool> ret1;
	pair<__gnu_cxx::hash_map<string,ModelComp*>::iterator,bool> ret2;
	ret1 = this->dummyValueMapCons.insert(pair<string,string>(dummyVar,value));
	ret2 = this->dummySetMapCons.insert(pair<string,ModelComp*>(dummyVar,comp));
	assert(ret1.second && ret2.second);
}

void ModelContext::addDummySetValueMapTemp(string& dummyVar,ModelComp* comp,string value)
{
	pair<__gnu_cxx::hash_map<string,string>::iterator,bool> ret1;
	pair<__gnu_cxx::hash_map<string,ModelComp*>::iterator,bool> ret2;
	ret1 = this->dummyValueMapTemp.insert(pair<string,string>(dummyVar,value));
	ret2 = this->dummySetMapTemp.insert(pair<string,ModelComp*>(dummyVar,comp));
	assert(ret1.second && ret2.second );
}

void ModelContext::removeDummySetValueMapCons(string& dummyVar)
{
	this->dummySetMapCons.erase(dummyVar);
	this->dummyValueMapCons.erase(dummyVar);
}

void ModelContext::removeDummySetValueMapTemp(string& dummyVar)
{
	this->dummyValueMapTemp.erase(dummyVar);
	this->dummySetMapTemp.erase(dummyVar);
}

string ModelContext::getModelDummyValAsKey(int& num)
{
	assert(num>=0);
	LOG("getModelDummyValAsKey - int - contextId["<<this->getContextId()<<"] - num["<<num<<"]");
	int numKey = num;
	string rval="";
	__gnu_cxx::hash_map<int,string>::iterator it = this->cacheModelDummyVarKey.find(numKey);
	rval = it==this->cacheModelDummyVarKey.end()?"":it->second;
	ExpandedModel2* em2=static_cast<ExpandedModel2*>(this->em);
	if(it==this->cacheModelDummyVarKey.end())
	{
		__gnu_cxx::hash_map<string,string>::iterator it2 = this->dummyValueMap.begin();
		for(;it2!=this->dummyValueMap.end()&&num>0;it2++)
		{
			rval += it2->second;
			--num;
		}

		if(parent!=NULL && num!=0){
			rval = this->parent->getModelDummyValAsKey(num) + rval;
		}

		this->cacheModelDummyVarKey.insert(pair<int,string>(numKey,rval));
		LOG("insert key["<<numKey<<"] for value - rval["<<rval<<"]");
		assert(num==0);
	}
	else
	{
		assert(num==numKey);
		num = 0;
		LOG("find getModelDummyValAsKey in cache - ["<<numKey<<"] - rval["<<rval<<"]");
	}
	LOG("end getModelDummyValAsKey - int - contextId["<<this->getContextId()<<"] - num["<<num<<"] numKey["<<numKey<<"]- rval["<<rval<<"]");
	return rval;
}

string ModelContext::getConsDummyValAsKey()
{
	string rval = "";
	__gnu_cxx::hash_map<string,string>::iterator it = this->dummyValueMapCons.begin();
	for(;it!=this->dummyValueMapCons.end();it++)
	{
		rval += it->second;
	}
	return rval;
}

string ModelContext::getDummyValueCons(string& dummyVar)
{
	__gnu_cxx::hash_map<string,string>::iterator it = this->dummyValueMapCons.find(dummyVar);
	string rval = it==this->dummyValueMapCons.end()?"":it->second;
	if(rval.empty())
	{
		LOG("no index value for var["<<dummyVar<<"] at constraint level!");
	}
	return rval;
}

string ModelContext::getDummyValueTemp(string& dummyVar)
{
	__gnu_cxx::hash_map<string,string>::iterator it = this->dummyValueMapTemp.find(dummyVar);
	string rval = it==this->dummyValueMapTemp.end()?"":it->second;
	if(rval.empty())
	{
		LOG("no index value for var["<<dummyVar<<"] at temp level!");
	}
	return rval;
}

string& ModelContext::getDummyValue(string& dummyVar)
{
	ExpandedModel2* em2=static_cast<ExpandedModel2*>(this->em);
	__gnu_cxx::hash_map<string,string>::iterator it1 = this->dummyValueMap.find(dummyVar);
	string rval = it1==this->dummyValueMap.end()? "":it1->second;

	if(rval.empty())
	{
		__gnu_cxx::hash_map<string,string>::iterator it2 = this->dummyValueMapCons.find(dummyVar);
		rval = it2==this->dummyValueMapCons.end()? "":it2->second;
	}

	if(rval.empty())
	{
		__gnu_cxx::hash_map<string,string>::iterator it3 = this->dummyValueMapTemp.find(dummyVar);
		rval = it3==this->dummyValueMapTemp.end()?"":it3->second;
	}

	if(rval.empty() && this->parent != NULL)
	{
		rval = this->parent->getDummyValue(dummyVar);
	}
	else if(rval.empty() && this->parent == NULL)
	{
		LOG("****WARNING*****  root node context reached - but not found - dummyVar["<<dummyVar<<"]  *empty string* returned");
	}
	return rval;
}
ModelComp* ModelContext::getDummySet(string& dummyVar)
{
	ExpandedModel2* em2=static_cast<ExpandedModel2*>(this->em);
	__gnu_cxx::hash_map<string,ModelComp*>::iterator it1 = this->dummySetMap.find(dummyVar);
	ModelComp* rval = it1==this->dummySetMap.end()? NULL:it1->second;

	if(rval==NULL){
		__gnu_cxx::hash_map<string,ModelComp*>::iterator it2 = this->dummySetMapCons.find(dummyVar);
		rval = it2==this->dummySetMapCons.end()? NULL:it2->second;
	}
	if(rval==NULL){
		__gnu_cxx::hash_map<string,ModelComp*>::iterator it3 = this->dummySetMapTemp.find(dummyVar);
		rval = it3==this->dummySetMapTemp.end()?NULL:it3->second;
	}
	assert(rval!=NULL);
	return rval;
}

string ModelContext::getContextId()
{
	return this->em==NULL? CONTEXT_ID_NOT_INIT:static_cast<ExpandedModel2*>(em)->name;
}

void ModelContext::fillDummyValue(vector<string>& index)
{
	ExpandedModel2* em2=static_cast<ExpandedModel2*>(this->em);
	if(this->parent!=NULL)
	{
		this->parent->fillDummyValue(index);
	}
	__gnu_cxx::hash_map<string,string>::iterator it;
	for(it=this->dummyValueMap.begin();it!=this->dummyValueMap.end();it++)
	{
		index.push_back(it->second);
	}
}


void ModelContext::fillDummyValue(ostringstream& oss)
{
	ExpandedModel2* em2=static_cast<ExpandedModel2*>(this->em);
	if(this->parent!=NULL)
	{
		this->parent->fillDummyValue(oss);
	}
	__gnu_cxx::hash_map<string,string>::iterator it;
	for(it=this->dummyValueMap.begin();it!=this->dummyValueMap.end();it++)
	{
		oss<<it->second;
	}
}

CompDescr* ModelContext::getCompValue(ModelComp* comp)
{
	LOG("getCompValue - contextID["<<this->getContextId()<<"] comp["<<comp->id<<"] key["<<comp->getHashKey()<<"]");
	__gnu_cxx::hash_map<string,CompDescr*>::iterator it = this->compValueMap.find(comp->getHashKey());
	CompDescr* rval = it==this->compValueMap.end()? NULL:it->second;
	if(rval==NULL && this->parent!=NULL)
	{
		rval = this->parent->getCompValue(comp);
	}
	else if(rval==NULL && this->parent==NULL)
	{
		LOG("root node context reached - but not found - comp["<<comp->id<<"]  *NULL* returned");
	}
	return rval;
}
void ModelContext::addCompValueMap(ModelComp* comp,CompDescr* value)
{
	pair<__gnu_cxx::hash_map<string,CompDescr*>::iterator,bool> ret;
	LOG("addCompValueMap - contextID["<<this->getContextId()<<"]comp["<<comp->id<<"] key["<<comp->getHashKey()<<"]");
	ret = this->compValueMap.insert(pair<string,CompDescr*>(comp->getHashKey(),value));
	assert(ret.second);
}

void ModelContext::removeCurrLevelCompValueMap(ModelComp* comp)
{
	__gnu_cxx::hash_map<string,CompDescr*>::iterator it = this->compValueMap.find(comp->getHashKey());
	assert(it!=this->compValueMap.end());
	delete it->second;
	this->compValueMap.erase(it);
}

//bool ModelContext::getCalcTempSet(string& hashKey,vector<ModelComp*>& comps,Set** aSet,vector<string>& dummyVars)
//{
//	bool rval = false;
//	LOG("look for hashKey["<<hashKey<<"] ");
//	__gnu_cxx::hash_map<string,vector<ModelComp*> >::iterator it_mcs = this->tempSetModelCompMap.find(hashKey);
//	__gnu_cxx::hash_map<string,Set* >::iterator it_set = this->tempSetSetMap.find(hashKey);
//	__gnu_cxx::hash_map<string,vector<string> >::iterator it_vars = this->tempSetDummyVarMap.find(hashKey);
//	assert(it_set==this->tempSetSetMap.end()||(it_set->second->dim == it_mcs->second.size() && it_set->second->dim == it_vars->second.size()));
//	if(it_mcs!=this->tempSetModelCompMap.end())
//	{
//		assert(it_vars!=this->tempSetDummyVarMap.end());
//		comps = it_mcs->second;
//		*aSet = it_set->second;
//		dummyVars = it_vars->second;
//		LOG("found TempSet for hashKey["<<hashKey<<"]");
//
//		vector<string>::iterator it_dummyVars = dummyVars.begin();
//		for(vector<ModelComp*>::iterator it_comps=comps.begin();it_comps!=comps.end();it_comps++,it_dummyVars++)
//		{
//			LOG("comp["<<(*it_comps)->id<<"] dummy["<<(*it_dummyVars)<<"]"<<(*aSet)->toString());
//		}
//		rval = true;
//	}
//
//	return rval;
//}

bool ModelContext::getCalcSumSet(string& hashKey,IndexSet** iset)
{
	bool rval = false;
	LOG("getCalcTempSet -- look for hashKey["<<hashKey<<"] ");
	__gnu_cxx::hash_map<string,IndexSet* >::iterator it_iset = this->tempISetMap.find(hashKey);
	if(it_iset!=this->tempISetMap.end())
	{
		rval = true;
		*iset = it_iset->second;
	}
	return rval;
}

void ModelContext::addCalcSumSet(string& hashKey,IndexSet* iset)
{
	LOG("add for hashKey["<<hashKey<<"] over set - "+iset->toString());

	__gnu_cxx::hash_map<string,IndexSet* >::iterator it_mcs = this->tempISetMap.find(hashKey);
	assert(it_mcs==this->tempISetMap.end()); //must not already exist

	this->tempISetMap[hashKey] = iset;
}

//void ModelContext::addCalcTempSet(string& hashKey,vector<ModelComp*>& comps,Set* aSet,vector<string>& dummyVars)
//{
//	LOG_SYS_MEM("BeforeaddCalcTempSet");
//	LOG("add for hashKey["<<hashKey<<"] over set{");
//
//	vector<string>::iterator it_dummyVars = dummyVars.begin();
//	for(vector<ModelComp*>::iterator it_comps=comps.begin();it_comps!=comps.end();it_comps++,it_dummyVars++)
//	{
//		LOG("comp["<<(*it_comps)->id<<"] dummy["<<(*it_dummyVars)<<"]"<<(aSet)->toString());
//	}
//
//	__gnu_cxx::hash_map<string,vector<ModelComp*> >::iterator it_mcs = this->tempSetModelCompMap.find(hashKey);
//	assert(it_mcs==this->tempSetModelCompMap.end()); //must not already exist
//
//	this->tempSetModelCompMap[hashKey] = comps;
//	this->tempSetSetMap[hashKey] = aSet;
//	this->tempSetDummyVarMap[hashKey] = dummyVars;
//	LOG_SYS_MEM("AfteraddCalcTempSet");
//}

void ModelContext::calculateMemoryUsage(unsigned long& size)
{
	size += sizeof(ModelContext);

	for(hash_map<string,ModelComp*>::iterator it=dummySetMap.begin();it!=dummySetMap.end();it++)
	{
		size += sizeof(pair<string,ModelComp*>);
		size += (*it).first.size() + 1;
	}
	for(hash_map<string,string>::iterator it=dummyValueMap.begin();it!=dummyValueMap.end();it++)
	{
		size += sizeof(pair<string,string>);
		size += (*it).first.size() + 1;
		size += (*it).second.size()+ 1;
	}

	for(hash_map<string,CompDescr*>::iterator it=compValueMap.begin();it!=compValueMap.end();it++)
	{
		size += sizeof(pair<string,CompDescr*>);
		size += (*it).first.size() + 1;
		(*it).second->calculateMemoryUsage(size);
	}
	for(hash_map<string,ModelComp*>::iterator it=dummySetMapCons.begin();it!=dummySetMapCons.end();it++)
	{
		size += sizeof(pair<string,ModelComp*>);
		size += (*it).first.size() + 1;
	}
	for(hash_map<string,string>::iterator it=dummyValueMapCons.begin();it!=dummyValueMapCons.end();it++)
	{
		size += sizeof(pair<string,string>);
		size += (*it).first.size() + 1;
		size += (*it).second.size() + 1;
	}
	for(hash_map<string,ModelComp*>::iterator it=dummySetMapTemp.begin();it!=dummySetMapTemp.end();it++)
	{
		size += sizeof(pair<string,ModelComp*>);
		size += (*it).first.size() + 1;
	}
	for(hash_map<string,string>::iterator it=dummyValueMapTemp.begin();it!=dummyValueMapTemp.end();it++)
	{
		size += sizeof(pair<string,string>);
		size += (*it).first.size() + 1;
		size += (*it).second.size() + 1;
	}
	for(hash_map<int,string>::iterator it=cacheModelDummyVarKey.begin();it!=cacheModelDummyVarKey.end();it++)
	{
		size += sizeof(pair<int,string>);
		size += (*it).second.size() + 1;
	}
//	for(hash_map<string,vector<ModelComp*> >::iterator it=tempSetModelCompMap.begin();it!=tempSetModelCompMap.end();it++)
//	{
//		size += sizeof(pair<string,vector<ModelComp*> >);
//		size += (*it).first.size() + 1;
//		size += (*it).second.size()*sizeof(ModelComp*);
//	}
//	for(hash_map<string,Set*>::iterator it=tempSetSetMap.begin();it!=tempSetSetMap.end();it++)
//	{
//		size += sizeof(pair<string,Set*>);
//		size += (*it).first.size()+1;
//		(*it).second->calculateMemoryUsage(size);
//	}
//	for(hash_map<string,vector<string> >::iterator it=tempSetDummyVarMap.begin();it!=tempSetDummyVarMap.end();it++)
//	{
//		size += sizeof(pair<string,vector<string> >);
//		size += (*it).first.size() + 1;
//		for(vector<string>::iterator it2 = (*it).second.begin();it2!=(*it).second.end();it2++)
//		{
//			size += sizeof(string);
//			size += (*it2).size() + 1;
//		}
//
//	}

	for(hash_map<string,IndexSet*>::iterator it = tempISetMap.begin();it!=tempISetMap.end();it++)
	{
		size += sizeof(pair<string,IndexSet*>);
		size += (*it).first.size() + 1;
		(*it).second->calculateMemoryUsage(size);
	}

	for(list<string>::iterator it=localConNames.begin();it!=localConNames.end();it++)
	{
		size += sizeof(string);
		size += (*it).size() + 1;
	}
	for(list<string>::iterator it=localVarNames.begin();it!=localVarNames.end();it++)
	{
		size += sizeof(string);
		size += (*it).size() + 1;
	}

	//size += modelDummyValKey.size() + 1;
}
