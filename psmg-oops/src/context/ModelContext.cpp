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
#include "../model/SyntaxNode.h"
#include "../model/ModelComp.h"
#include "../util/global_util_functions.h"


using namespace std;

static string CONTEXT_ID_NOT_INIT="NOT_INITIALIZED";

ModelContext::ModelContext(ModelContext* par):
		parent(par),em(NULL),moveUpLevel(0)
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
	dummyCompMapTemp.clear();
	dummyValueMapTemp.clear();
}

string ModelContext::getContextId()
{
	return this->em==NULL? CONTEXT_ID_NOT_INIT:em->name;
}

void ModelContext::addDummyCompValueMapTemp(string& dummyVar,ModelComp* comp,string& value)
{
	pair<boost::unordered_map<string,string>::iterator,bool> ret1;
	pair<boost::unordered_map<string,ModelComp*>::iterator,bool> ret2;
	ret1 = this->dummyValueMapTemp.insert(pair<string,string>(dummyVar,value));
	ret2 = this->dummyCompMapTemp.insert(pair<string,ModelComp*>(dummyVar,comp));
	assert(ret1.second && ret2.second );
}

void ModelContext::removeDummySetValueMapTemp(string& dummyVar)
{
	this->dummyValueMapTemp.erase(dummyVar);
	this->dummyCompMapTemp.erase(dummyVar);
}

string ModelContext::getDummyValue(string& dummyVar)
{
	//looking into expanded model indexing
	boost::unordered_map<string,string>::iterator it1 = this->em->dummyValueMap.find(dummyVar);
	string rval = it1==this->em->dummyValueMap.end()? "":it1->second;

	//looking into the temporary map if not found in model index
	if(rval.empty())
	{
		boost::unordered_map<string,string>::iterator it3 = this->dummyValueMapTemp.find(dummyVar);
		rval = it3==this->dummyValueMapTemp.end()?"":it3->second;
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
	boost::unordered_map<string,ModelComp*>::iterator it1 = this->em->dummySetMap.find(dummyVar);
	ModelComp* rval = it1==this->em->dummySetMap.end()? NULL:it1->second;

	if(rval==NULL){
		boost::unordered_map<string,ModelComp*>::iterator it3 = this->dummyCompMapTemp.find(dummyVar);
		rval = it3==this->dummyCompMapTemp.end()?NULL:it3->second;
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
		LOG("root node context reached - but not found - comp["<<comp->name<<"]  *NULL* returned");
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

void ModelContext::fillDummyValue(vector<string>& index)
{
	if(this->parent!=NULL)
	{
		this->parent->fillDummyValue(index);
	}
	boost::unordered_map<string,string>::iterator it;
	for(it=this->em->dummyValueMap.begin();it!=this->em->dummyValueMap.end();it++)
	{
		index.push_back(it->second);
	}
}


void ModelContext::fillDummyValue(ostringstream& oss)
{
	if(this->parent!=NULL)
	{
		this->parent->fillDummyValue(oss);
	}
	boost::unordered_map<string,string>::iterator it;
	for(it=this->em->dummyValueMap.begin();it!=this->em->dummyValueMap.end();it++)
	{
		oss<<it->second;
	}
}

void ModelContext::calculateMemoryUsage(unsigned long& size)
{
	size += sizeof(ModelContext);

	for(boost::unordered_map<ModelComp*,CompDescr*>::iterator it=compValueMap.begin();it!=compValueMap.end();it++)
	{
		size += sizeof(pair<ModelComp*,CompDescr*>);
		(*it).second->calculateMemoryUsage(size);
	}
	for(boost::unordered_map<string,ModelComp*>::iterator it=dummyCompMapTemp.begin();it!=dummyCompMapTemp.end();it++)
	{
		size += sizeof(pair<string,ModelComp*>);
		size += (*it).first.size() + 1;
	}
	for(boost::unordered_map<string,string>::iterator it=dummyValueMapTemp.begin();it!=dummyValueMapTemp.end();it++)
	{
		size += sizeof(pair<string,string>);
		size += (*it).first.size() + 1;
		size += (*it).second.size() + 1;
	}


	for(boost::unordered_map<SyntaxNode*,IndexSet*>::iterator it = tempISetMap.begin();it!=tempISetMap.end();it++)
	{
		size += sizeof(pair<SyntaxNode*,IndexSet*>);
		(*it).second->calculateMemoryUsage(size);
	}
}



//string ModelContext::getDummyValueTemp(string& dummyVar)
//{
//	boost::unordered_map<string,string>::iterator it = this->dummyValueMapTemp.find(dummyVar);
//	string rval = it==this->dummyValueMapTemp.end()?"":it->second;
//	if(rval.empty())
//	{
//		LOG("no index value for var["<<dummyVar<<"] at temp level!");
//	}
//	return rval;
//}

//string ModelContext::getConsDummyValAsKey()
//{
//	string rval = "";
//	boost::unordered_map<string,string>::iterator it = this->dummyValueMapCons.begin();
//	for(;it!=this->dummyValueMapCons.end();it++)
//	{
//		rval += it->second;
//	}
//	return rval;
//}

//string ModelContext::getModelDummyValAsKey(int& num)
//{
//	assert(num>=0);
//	LOG("getModelDummyValAsKey - int - contextId["<<this->getContextId()<<"] - num["<<num<<"]");
//	int numKey = num;
//	string rval="";
//	boost::unordered_map<int,string>::iterator it = this->cacheModelDummyVarKey.find(numKey);
//	rval = it==this->cacheModelDummyVarKey.end()?"":it->second;
//	if(it==this->cacheModelDummyVarKey.end())
//	{
//		boost::unordered_map<string,string>::iterator it2 = this->em2->dummyValueMap.begin();
//		for(;it2!=this->em2->dummyValueMap.end()&&num>0;it2++)
//		{
//			rval += it2->second;
//			--num;
//		}
//
//		if(parent!=NULL && num!=0){
//			rval = this->parent->getModelDummyValAsKey(num) + rval;
//		}
//
//		this->cacheModelDummyVarKey.insert(pair<int,string>(numKey,rval));
//		LOG("insert key["<<numKey<<"] for value - rval["<<rval<<"]");
//		assert(num==0);
//	}
//	else
//	{
//		assert(num==numKey);
//		num = 0;
//		LOG("find getModelDummyValAsKey in cache - ["<<numKey<<"] - rval["<<rval<<"]");
//	}
//	LOG("end getModelDummyValAsKey - int - contextId["<<this->getContextId()<<"] - num["<<num<<"] numKey["<<numKey<<"]- rval["<<rval<<"]");
//	return rval;
//}

