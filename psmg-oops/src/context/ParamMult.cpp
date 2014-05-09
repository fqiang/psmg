/*
 * ParamMult.cpp
 *
 *  Created on: 16 Dec 2013
 *      Author: s0965328
 */

#include "ParamMult.h"
#include "../util/util.h"


ParamMult::ParamMult(ParamComp* comp):Param(comp)
{
	LOG("Create ParamMult - name["<<name<<"] numIndicies["<<numIndicies<<"]");
}

ParamMult::~ParamMult()
{
	boost::unordered_map<string,PValue*>::const_iterator i;
	for(i=this->paramValues.begin();i!=this->paramValues.end();i++)
	{
		delete (*i).second;
	}
	paramValues.clear();
}

void ParamMult::addParamValue(const string& indiciesKey, PValue* value)
{
	LOG("ParamMult - Add ParamValue name["<<name<<"] card["<<card<<"] ["<<indiciesKey<<" <= "<<value->toString()<<"]");
	pair<boost::unordered_map<string,PValue*>::iterator,bool> ret;
	ret = paramValues.insert(pair<string,PValue*>(indiciesKey,value));
	assert(ret.second);
	card++;
}

PValue* ParamMult::findParamValue(const string& indiciesKey)
{
	LOG("-- ParamMult::findParamValue - key["<<indiciesKey<<"] );//in"<<endl<<this->toString());
	PValue* rval = (this->paramValues.find(indiciesKey)->second);
	LOG("-- ParamMult::findParamValue -- found  - ["<<rval<<"]");
	return rval;
}

string ParamMult::toString()
{
	ostringstream oss;
	oss<<"ParamMult - name["<<name<<"]"<<endl;
	boost::unordered_map<string,PValue*>::const_iterator i;
	for(i=this->paramValues.begin();i!=this->paramValues.end();i++)
	{
		oss<<"["<<i->first<<"-->"<<i->second->toString()<<"] ";
	}
	return oss.str();
}


void ParamMult::calculateMemoryUsage(unsigned long& size)
{
	size += sizeof(ParamMult);
	size += name.size() + 1;
	boost::unordered_map<string,PValue*>::iterator it = paramValues.begin();
	for(;it!=paramValues.end();it++)
	{
		size += sizeof(pair<string,PValue*>);
		it->second->calculateMemoryUsage(size);
	}
}
