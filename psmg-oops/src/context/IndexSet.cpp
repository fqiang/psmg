/*
 * IndexSet.cpp
 *
 *  Created on: 3 May 2013
 *      Author: s0965328
 */

#include "IndexSet.h"
#include "../model/ModelComp.h";

IndexSet::IndexSet(string name_) : name(name_) {
	// TODO Auto-generated constructor stub

}

IndexSet::~IndexSet() {
	dummyVarNames.clear();
	setIndicies.clear();
}

void IndexSet::addSet(Set* aSet)
{
	assert(aSet->dim==this->dummyVarNames.size());
	assert(this->dummyVarNames.size()==this->comps.size());
	vector<string>::iterator it=aSet->setValues_data_order.begin();
	for(;it!=aSet->setValues_data_order.end();it++)
	{
		vector<string> v;
		v.push_back(*it);
		this->setIndicies.push_back(v);
	}
}

string IndexSet::toString() const
{
	ostringstream oss(ostringstream::out);
	vector< vector<string> >::const_iterator it1 = setIndicies.begin();
	vector<string>::const_iterator it2 = dummyVarNames.begin();
	vector<ModelComp*>::const_iterator it3 = comps.begin();

	oss<<"IndexSet:"<<name;
	assert(dummyVarNames.size()==comps.size());
	for(;it2!=dummyVarNames.end();it2++,it3++)
	{
		oss<<"{"<<(*it3)->id<<"-"<<*it2<<"} - ";
	}

	for(;it1!=setIndicies.end();it1++)
	{
		assert((*it1).size()==comps.size());
		vector<string>::const_iterator ind = (*it1).begin();
		for(;ind!=(*it1).end();ind++)
		{
			oss<<*ind;
		}
		oss<<"}";
	}
	return oss.str();
}

void IndexSet::calculateMemoryUsage(unsigned long& size)
{
		size += sizeof(IndexSet);
		size += name.size() + 1;

		vector< vector<string> >::const_iterator it1 = setIndicies.begin();
		vector<string>::const_iterator it2 = dummyVarNames.begin();
		vector<ModelComp*>::const_iterator it3 = comps.begin();

		for(;it1!=setIndicies.end();it1++,it2++,it3++)
		{
			size += (*it2).size() + 1;
			size += sizeof(ModelComp*);
			vector<string>::const_iterator ind = (*it1).begin();
			for(;ind!=(*it1).end();ind++)
			{
				size += (*it1).size() + 1;
			}
		}
}
