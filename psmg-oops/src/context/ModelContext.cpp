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

//static string CONTEXT_ID_NOT_INIT="NOT_INITIALIZED";

ModelContext::ModelContext(ExpandedModel* e): em(e), parent(NULL)
{
	if( e->parent!=NULL ) parent = &(e->parent->ctx);
	TRACE("ModelContext - constructor - ");

	//initialize default bucket size! -- memory optimization
	compValueMap.bucket_size(20);		//20 comp value(set, param) as default.
	dummyTempMap.bucket_size(10);      //10 dummy index most likely
	dummyEmcolTempMap.bucket_size(5);  //5 level as default size
	tempISetMap.bucket_size(5);        //5 temp index set
}

ModelContext::~ModelContext()
{
	TRACE("Delete ModelContext");
	this->dropContent();
}

void ModelContext::dropContent()
{
	used = false;
	//must delete IndexSet before CompDescr* map
	//because IndexSet only delete a Set if its name == "TMP_" , once the CompDescr* deleted
	//it is not possible for IndexSet to test name=="TMP_" for the set already deleted
	for(boost::unordered_map<SyntaxNode*,IndexSet*>::iterator it=tempISetMap.begin();it!=tempISetMap.end();it++)
	{
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
}

void ModelContext::addDummyEmcolTempMap(string& dummyVar, string& value)
{
	pair<boost::unordered_map<string,string>::iterator,bool> ret1;
	ret1 = this->dummyEmcolTempMap.insert(pair<string,string>(dummyVar,value));
	assert(ret1.second);
}

void ModelContext::removeDummyEmcolTempMap(string& dummyVar)
{
	uint i = this->dummyEmcolTempMap.erase(dummyVar);
	assert(i==1);
}
void ModelContext::addDummyCompValueMapTemp(string& dummyVar,ModelComp* comp,string& value)
{
	pair<boost::unordered_map<string,dummy_tmp_t>::iterator,bool> ret1;
	dummy_tmp_t a_dummy_tmp = std::make_pair<ModelComp*,string>(comp,value);
	ret1 = this->dummyTempMap.insert(pair<string,dummy_tmp_t>(dummyVar,a_dummy_tmp));
	assert(ret1.second);
}

void ModelContext::removeDummySetValueMapTemp(string& dummyVar)
{
	uint i = this->dummyTempMap.erase(dummyVar);
	assert(i==1);
}

string& ModelContext::getDummyValue(string& dummyVar)
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
	}

	//looking into the parent if it still not found
	if(rval.empty() && this->parent != NULL)
	{
		rval = this->parent->getDummyValue(dummyVar);
	}
	else if(rval.empty() && this->parent == NULL)
	{
		TRACE("****MODELLING ERROR*****  root node context reached - but not found - dummyVar["<<dummyVar<<"]");
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
	}
	if(rval==NULL && this->parent != NULL)
	{
		rval = this->parent->getDummyComp(dummyVar);
	}
	else if(rval==NULL && this->parent == NULL)
	{
		TRACE("****WARNING*****  root node context reached - but not found - dummyVar["<<dummyVar<<"]  *NULL* returned");
		assert(false);
	}
	return rval;
}

CompDescr* ModelContext::getCompValue(ModelComp* comp)
{
	TRACE("getCompValue - contextID["<<this->em->name<<"] comp["<<comp->name<<"]");
	boost::unordered_map<ModelComp*,CompDescr*>::iterator it = this->compValueMap.find(comp);
	CompDescr* rval = it==this->compValueMap.end()? NULL:it->second;
	if(rval==NULL && this->parent!=NULL)
	{
		rval = this->parent->getCompValue(comp);
	}
	else if(rval==NULL && this->parent==NULL)
	{
		TRACE("root node context reached - but not found - comp["<<comp->name<<"]  *NULL* returned - to be initialized.");
	}
	return rval;
}

void ModelContext::addCompValueMap(ModelComp* comp,CompDescr* value)
{
	pair<boost::unordered_map<ModelComp*,CompDescr*>::iterator,bool> ret;
	TRACE("addCompValueMap - contextID["<<this->em->name<<"]comp["<<comp->name<<"]");
	ret = this->compValueMap.insert(pair<ModelComp*,CompDescr*>(comp,value));
	assert(ret.second);
}

bool ModelContext::getCalcSumSet(SyntaxNode* key,IndexSet** iset)
{
	bool rval = false;
	TRACE("getCalcTempSet -- look for hashKey["<<key<<"] ");
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
	TRACE("addCalcSumSet  hashkey["<<key<<"] -- set -- "+iset->toString());
	pair<boost::unordered_map<SyntaxNode*,IndexSet*>::iterator, bool > ret;
	ret = this->tempISetMap.insert(pair<SyntaxNode*,IndexSet*>(key,iset));
	assert(ret.second);
}

//equal only if it is exactly same memory location
bool ModelContext::operator==(const ModelContext& other) const
{
	return this == &(other);
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
		size += (*it).first.size() + 1;
		size += (*it).second.second.size() + 1; //the string
	}

	for(boost::unordered_map<SyntaxNode*,IndexSet*>::iterator it = tempISetMap.begin();it!=tempISetMap.end();it++)
	{
		size += sizeof(pair<SyntaxNode*,IndexSet*>);
		(*it).second->calculateMemoryUsage(size);
	}

	assert(dummyEmcolTempMap.size()==0);
	for(boost::unordered_map<string,string>::iterator it=dummyEmcolTempMap.begin();it!=dummyEmcolTempMap.end();it++)
	{
		size += sizeof(pair<string,string>);
		size += (*it).first.size() + 1;
		size += (*it).second.size() + 1;
	}

}

