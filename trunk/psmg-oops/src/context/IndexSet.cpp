/*
 * IndexSet.cpp
 *
 *  Created on: 3 May 2013
 *      Author: s0965328
 */

#include "IndexSet.h"
#include <sstream>

#include "../util/util.h"
#include "Set.h"
#include "../model/SetComp.h"

string IndexSet::TMP = "TMP_";
string IndexSet::NEWSET = "NEWSET_";

IndexSet::IndexSet(string& name_) : name(name_) {

}

IndexSet::~IndexSet()
{
	TRACE("Deleting -------  IndexSet -- ["<<name<<"]");
	if(name.compare(0,7,IndexSet::NEWSET)==0)
	{
		BOOST_FOREACH(iset_tuple& tuple, tuples)
		{
			assert(tuple.set->name.compare(0,4,Set::TMP)==0);
			delete tuple.set;
		}
	}
	tuples.clear();
}


void IndexSet::addSet(string& dummyVar, Set* aSet,SetComp* comp)
{
	assert(aSet->dim==1); //TODO for now just handle one-dim set
	iset_tuple tuple(dummyVar,aSet,comp);
	tuples.push_back(tuple);
}

string IndexSet::toString()
{
	ostringstream oss(ostringstream::out);
	oss<<"IndexSet:["<<name<<"]"<<endl;
	BOOST_FOREACH(iset_tuple& tuple, tuples)
	{
		oss<<"["<<tuple.dummyVar<<" -> "<<tuple.setcomp->name<<"] ";
		oss<<tuple.set->toString()<<endl;
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
			assert(tuple.set->name.compare(0,4,Set::TMP)==0);
			tuple.set->calculateMemoryUsage(size);
		}
	}

}
