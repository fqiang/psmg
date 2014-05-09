/*
 * IndexSet.cpp
 *
 *  Created on: 3 May 2013
 *      Author: s0965328
 */

#include "IndexSet.h"

#include <boost/foreach.hpp>

#include "Set.h"
#include "../model/SetComp.h"

string IndexSet::TMP = "TMP_";
string IndexSet::NEWSET = "NEWSET_";

IndexSet::IndexSet(string& name_) : name(name_) {

}

IndexSet::~IndexSet()
{
	if(name.compare(0,7,IndexSet::NEWSET)==0)
	{
		BOOST_FOREACH(iset_tuple& tuple, tuples)
		{
			assert(tuple.get<1>()->name.compare(0,4,Set::TMP)==0);
			delete tuple.get<1>();
		}
	}
	tuples.clear();
}


void IndexSet::addSet(string& dummyVar, Set* aSet,SetComp* comp)
{
	assert(aSet->dim==1); //TODO for now just handle one-dim set
	iset_tuple tuple = boost::make_tuple<string,Set*,SetComp*>(dummyVar,aSet,comp);
	tuples.push_back(tuple);
}

string IndexSet::toString()
{
	ostringstream oss(ostringstream::out);
	oss<<"IndexSet:["<<name<<"]"<<endl;
	BOOST_FOREACH(iset_tuple& tuple, tuples)
	{
		oss<<"["<<tuple.get<0>()<<" -> "<<tuple.get<2>()->name<<"] ";
		oss<<tuple.get<1>()->toString()<<endl;
	}
	return oss.str();
}

void IndexSet::calculateMemoryUsage(unsigned long int& size)
{
	size += sizeof(IndexSet);
	size += name.size() + 1;

	size += sizeof(iset_tuple)*tuples.size();

	if(name.compare(0,7,IndexSet::NEWSET)==0)
	{
		BOOST_FOREACH(iset_tuple& tuple, tuples)
		{
			assert(tuple.get<1>()->name.compare(0,4,Set::TMP)==0);
			tuple.get<1>()->calculateMemoryUsage(size);
		}
	}

}
