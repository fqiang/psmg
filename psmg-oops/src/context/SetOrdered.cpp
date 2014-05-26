/*
 * OrderedSet.cpp
 *
 *  Created on: 24 Jan 2014
 *      Author: s0965328
 */

#include "SetOrdered.h"

#include "../util/util.h"

SetOrdered::SetOrdered(SetComp* comp):Set(comp){
	TRACE("Create OrderedSet -");
}

SetOrdered::~SetOrdered() {
	TRACE("OrderedSet::~OrderedSet -- delete "<<toString());
	this->setOrders.clear();
	this->setKeys.clear();
}

int SetOrdered::setOrder(string& key)
{
	boost::unordered_map<string,int>::iterator i = this->setOrders.find(key);
	assert(i!=this->setOrders.end());
	return (*i).second;
}

void SetOrdered::copyFromSet(Set* src)
{
	for(vector<string>::iterator i=src->setValues_data_order.begin();i!=src->setValues_data_order.end();i++)
	{
		card++;
		this->setValues_data_order.push_back(*i);
		this->setOrders.insert(pair<string,int>(*i,card));
		this->setKeys.insert(pair<int,string>(card,*i));
	}
}

void SetOrdered::addSetValue(const string& value)
{
	if(!this->contains(value))
	{
		card++;
		TRACE("SetOrdered - addSetValue index["<<card<<"] ["<<value<<"]");
		this->setValues_data_order.push_back(value);
		this->setOrders.insert(pair<string,int>(value,card));
		this->setKeys.insert(pair<int,string>(card,value));
	}
	else{
		TRACE("OrderedSet --- already has ["<<value<<"]");
	}
//	TRACE("OrderedSet --addSetValue- set contains "<<this->toString());
}

bool SetOrdered::contains(const string& key)
{
	boost::unordered_map<string,int>::iterator it = this->setOrders.find(key);
	if(it!=this->setOrders.end())
	{
		return true;
	}
	return false;
}

string SetOrdered::toString()
{
	TRACE("OrderedSet["<<name<<"] card["<<card<<"]");
	ostringstream oss;
	vector<string>::const_iterator i;
	for(i= this->setValues_data_order.begin();i!=this->setValues_data_order.end();i++)
	{
		oss<<(*i)<<"|";
	}
	return oss.str();
}


void SetOrdered::calculateMemoryUsage(unsigned long& size)
{
	Set::calculateMemoryUsage(size);
	size += sizeof(SetOrdered);
	for(boost::unordered_map<string,int>::iterator it=setOrders.begin();it!=setOrders.end();it++)
	{
		size += sizeof(pair<string,int>);
		size += (*it).first.size() + 1;
	}
	for(boost::unordered_map<int,string>::iterator it=setKeys.begin();it!=setKeys.end();it++)
	{
		size += sizeof(pair<int,string>);
		size += (*it).second.size() + 1;
	}
}
