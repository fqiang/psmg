/*
 * Set.cpp
 *
 *  Created on: 29 Jan 2014
 *      Author: s0965328
 */

#include "Set.h"

#include "../util/util.h"
#include "../model/SetComp.h"
#include "SetSimple.h"
#include "SetOrdered.h"


string Set::TMP = "TMP_";

Set::Set(string& name, int dim):name(name),dim(dim),card(0)
{

}

Set::Set(SetComp* comp):dim(comp->setDim),name(comp->name),card(0) {
	TRACE("create Set name["<<name<<"] dim["<<dim<<"] card["<<card<<"]");
}

Set::~Set() {
	TRACE("Set::~Set --- Delete Set -- --------["<<this->name<<"] ["<<this<<"]");
}

void Set::deleteSet(Set* set)
{
	if(set->name.compare(0,4,Set::TMP)==0)
	{
		delete set;
	}
}

Set* Set::createSet(SetComp* comp)
{
	assert(comp!=NULL);
	Set* rval = NULL;
	if(comp->isOrdered)
	{
		rval = new SetOrdered(comp);
	}
	else
	{
		rval = new SetSimple(comp);
	}
	return rval;
}

void Set::setDiff(Set* sub, Set* rval)
{
	assert(this->dim == sub->dim);
	TRACE("setDiff -   ["<<this->name<<"_DIFF_"<<sub->name<<"]");

	vector<string>::iterator i;
	for(i= this->setValues_data_order.begin();i!=this->setValues_data_order.end();i++)
	{
		if(!sub->contains(*i))
		{
			rval->addSetValue(*i);
		}
	}
}

void Set::setCross(Set* other, Set* rval)
{
	TRACE("setCross -  ["<<this->name<<"  CROSS  "<<other->name<<"]");
	vector<string>::iterator i;
	vector<string>::iterator j;
	ostringstream oss(ostringstream::out);
	for(i= this->setValues_data_order.begin();i!=this->setValues_data_order.end();i++)
	{
		for(j= other->setValues_data_order.begin();j!=other->setValues_data_order.end();j++)
		{
			oss<<*i;
			oss<<*j;
			rval->addSetValue(oss.str());
			oss.str("");
			oss.clear();
		}
	}
}

void Set::setUnion(Set* other, Set* rval)
{
	TRACE("setUnion - ["<<this->name<<"  UNION  "<<other->name<<"]");
	rval->copyFromSet(this);
	for(vector<string>::iterator i= other->setValues_data_order.begin();i!=other->setValues_data_order.end();i++)
	{
		rval->addSetValue(*i);
	}
}


bool Set::contains(const string& value)
{
	vector<string>::iterator it = this->setValues_data_order.begin();
	for(;it!=this->setValues_data_order.end();it++)
	{
		if((*it).compare(value)==0)
		{
			return true;
		}
	}
	return false;
}

void Set::calculateMemoryUsage(unsigned long& size)
{
	size += name.size() + 1;
	for(vector<string>::iterator it = setValues_data_order.begin();it != setValues_data_order.end();it++) {
		size += (*it).size() + 1;
	}
}
