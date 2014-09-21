/*
 * SetSets.cpp
 *
 *  Created on: 11 Jun 2014
 *      Author: s0965328
 */

#include "SetSets.h"
#include "SetSimple.h"

#include "../util/util.h"

SetSets::SetSets(string& name, int dim) : name(name),dim(dim){

}
SetSets::SetSets(SetComp* comp):name(comp->name),dim(comp->setDim){

}

SetSets::~SetSets() {
	boost::unordered_map<string,Set*>::const_iterator i;
	for(i= this->sets.begin();i!=this->sets.end();i++)
	{
		delete i->second;
	}
}

Set* SetSets::getSet(const string& key)
{
	boost::unordered_map<string,Set*>::iterator i = this->sets.find(key);
	assert(i!=this->sets.end());
	return (*i).second;
}

void SetSets::addSetValue(const string& key,const string value)
{
	boost::unordered_map<string,Set*>::iterator i = this->sets.find(key);
	if(i==this->sets.end())
	{
		Set* set = new SetSimple(Set::TMP,1);
		sets.insert(pair<string,Set*>(key,set));
		set->addSetValue(value);
	}
	else
		(*i).second->addSetValue(value);
}

string SetSets::toString()
{
	TRACE("SetSets["<<name<<"] dim["<<dim<<"]");
	ostringstream oss;
	boost::unordered_map<string,Set*>::const_iterator i;
	for(i= this->sets.begin();i!=this->sets.end();i++)
	{
		oss<<"[key:"<<(*i).first<<"]"<<endl;
		oss<<(*i).second->toString()<<endl;
	}
	return oss.str();
}
void  SetSets::calculateMemoryUsage(unsigned long& size)
{
	size += sizeof(SetSets);
	size += name.size() +1;
	boost::unordered_map<string, Set*>::const_iterator i;
	for(i=this->sets.begin();i!=this->sets.end();i++)
	{
		size += sizeof(pair<string,Set*>);
		size += i->first.size()+1;
		i->second->calculateMemoryUsage(size);
	}
}
