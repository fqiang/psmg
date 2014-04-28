/*
 * IndexSet.cpp
 *
 *  Created on: 3 May 2013
 *      Author: s0965328
 */

#include "IndexSet.h"
#include "../model/SetComp.h"

string IndexSet::TMP = "TMP_";
string IndexSet::NEWSET = "NEWSET_";

IndexSet::IndexSet(string& name_) : name(name_) {

}

IndexSet::~IndexSet()
{
	if(name.compare(0,7,IndexSet::NEWSET)==0)
	{
		for(boost::unordered_map<string,Set*>::iterator it=dummySetMap.begin();it!=dummySetMap.end();it++)
		{
			assert(it->second->name.compare(0,4,Set::TMP)==0);
			delete it->second;
		}
	}
	this->dummySetMap.clear();
	this->dummyCompMap.clear();
}


void IndexSet::addSet(string& dummyVar, Set* aSet,SetComp* comp)
{
	assert(aSet->dim==1); //TODO for now just handle one-dim set
	this->dummySetMap.insert(pair<string,Set*>(dummyVar,aSet));
	this->dummyCompMap.insert(pair<string,SetComp*>(dummyVar,comp));
}

string IndexSet::toString()
{
	ostringstream oss(ostringstream::out);
	boost::unordered_map<string,Set*>::iterator it = this->dummySetMap.begin();
	oss<<"IndexSet:["<<name<<"]"<<endl;
	for(;it!=this->dummySetMap.end();it++)
	{
		oss<<"["<<it->first<<"] ";
		oss<<it->second->toString()<<endl;
	}
	return oss.str();
}

void IndexSet::calculateMemoryUsage(unsigned long int& size)
{
	size += sizeof(IndexSet);
	size += name.size() + 1;
	boost::unordered_map<string,Set*>::iterator it = this->dummySetMap.begin();
	for(;it!=this->dummySetMap.end();it++)
	{
		size += sizeof(pair<string,Set*>);
		size += it->first.size()+1;
		string setname = it->second->name;
		if(setname.compare(0,4,Set::TMP)==0)
		{
			it->second->calculateMemoryUsage(size);
		}
	}
}


//IndexSet::~IndexSet() {
//	dummyVarNames.clear();
//	setIndicies.clear();
//}

//string IndexSet::toString()
//{
//	ostringstream oss(ostringstream::out);
//	vector< vector<string> >::const_iterator it1 = setIndicies.begin();
//	vector<string>::const_iterator it2 = dummyVarNames.begin();
//	vector<ModelComp*>::const_iterator it3 = comps.begin();
//
//	oss<<"IndexSet:"<<name;
//	assert(dummyVarNames.size()==comps.size());
//	for(;it2!=dummyVarNames.end();it2++,it3++)
//	{
//		oss<<"{"<<(*it3)->id<<"-"<<*it2<<"} - ";
//	}
//
//	for(;it1!=setIndicies.end();it1++)
//	{
//		assert((*it1).size()==comps.size());
//		vector<string>::const_iterator ind = (*it1).begin();
//		for(;ind!=(*it1).end();ind++)
//		{
//			oss<<*ind;
//		}
//		oss<<"}";
//	}
//	return oss.str();
//}

//void IndexSet::calculateMemoryUsage(unsigned long& size)
//{
//		size += sizeof(IndexSet);
//		size += name.size() + 1;
//
//		vector< vector<string> >::const_iterator it1 = setIndicies.begin();
//		vector<string>::const_iterator it2 = dummyVarNames.begin();
//		vector<ModelComp*>::const_iterator it3 = comps.begin();
//		assert(dummyVarNames.size()==comps.size());
//
//		for(;it2!=dummyVarNames.end();it2++,it3++)
//		{
//			size += (*it2).size() + 1;
//			size += sizeof(ModelComp*);
//		}
//
//		for(;it1!=setIndicies.end();it1++)
//		{
//			assert((*it1).size()==comps.size());
//			size += sizeof(vector<string>);
//			vector<string>::const_iterator ind = (*it1).begin();
//			for(;ind!=(*it1).end();ind++)
//			{
//				size += (*ind).size() + 1;
//			}
//		}
//}


//void IndexSet::addSet(Set* aSet)
//{
//	assert(aSet->dim==this->dummyVarNames.size());
//	assert(this->dummyVarNames.size()==this->comps.size());
//	vector<string>::iterator it=aSet->setValues_data_order.begin();
//	for(;it!=aSet->setValues_data_order.end();it++)
//	{
//		vector<string> v;
//		v.push_back(*it);
//		this->setIndicies.push_back(v);
//	}
//}


//string IndexSet::toString()
//{
//	ostringstream oss(ostringstream::out);
//	vector< vector<string> >::const_iterator it1 = setIndicies.begin();
//	vector<string>::const_iterator it2 = dummyVarNames.begin();
//	vector<ModelComp*>::const_iterator it3 = comps.begin();
//
//	oss<<"IndexSet:"<<name;
//	assert(dummyVarNames.size()==comps.size());
//	for(;it2!=dummyVarNames.end();it2++,it3++)
//	{
//		oss<<"{"<<(*it3)->id<<"-"<<*it2<<"} - ";
//	}
//
//	for(;it1!=setIndicies.end();it1++)
//	{
//		assert((*it1).size()==comps.size());
//		vector<string>::const_iterator ind = (*it1).begin();
//		for(;ind!=(*it1).end();ind++)
//		{
//			oss<<*ind;
//		}
//		oss<<"}";
//	}
//	return oss.str();
//}

