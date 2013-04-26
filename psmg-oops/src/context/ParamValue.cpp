/*
 * ParamValue.cpp
 *
 *  Created on: 29 Sep 2011
 *      Author: s0965328
 */

#include "ParamValue.h"
#include <sstream>
#include <string>

using namespace std;

ParamValue::ParamValue(vector<string> ind,double val)
{
	vector<string>::iterator i;
	for(i=ind.begin();i!=ind.end();i++)
	{
		this->indices.push_back(*i);
	}
	this->value = val;
}
ParamValue::~ParamValue()
{
	indices.clear();
}

string ParamValue::getParamIndicesKey()
{
	ostringstream oss;
	vector<string>::iterator i;
	for(i=this->indices.begin();i!=this->indices.end();i++)
	{
		oss<<(*i);
	}
	return oss.str();
}

string ParamValue::toString()
{
	ostringstream oss;
	oss<<"["<<getParamIndicesKey()<<"]{"<<this->value<<"}";
	return oss.str();
}

void ParamValue::calculateMemoryUsage(unsigned long& size)
{
	size += sizeof(ParamValue);
	for(vector<string>::iterator it=indices.begin();it!=indices.end();it++)
	{
		size += sizeof(string);
		size += (*it).size() + 1;
	}
}

