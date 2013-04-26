/*
 * VarValue.cpp
 *
 *  Created on: 11 Oct 2011
 *      Author: s0965328
 */

#include "VarValue.h"
#include <sstream>

VarValue::VarValue(vector<string> ind,double lower,double upper): value(0), lb(lower),ub(upper) {
	this->varKey = "";
	this->indicies = "";
	this->numIndicies = ind.size();
	vector<string>::iterator i;
	for(i=ind.begin();i!=ind.end();i++)
	{
//		this->indicies.push_back(*i);
		this->varKey += *i;
		this->indicies += *i + "|";
	}
}

VarValue::~VarValue() {
	//indicies.clear();
}

//string& VarValue::getVarKey()
//{
//	ostringstream oss;
//	vector<string>::iterator i;
//	for(i=this->indicies.begin();i!=this->indicies.end();i++)
//	{
//		oss<<(*i);
//	}
//	return oss.str();
//	return this->varKey;
//}

string VarValue::toString()
{
	ostringstream oss;
	oss<<"[";
	vector<string>::iterator i;
//	for(i=this->indicies.begin();i!=this->indicies.end();i++)
//	{
//		oss<<(*i);
//	}
	oss<<this->indicies;
	oss<<"]{lb:"<<lb<<"}{ub:"<<ub<<"}";

	return oss.str();
}

void VarValue::calculateMemoryUsage(unsigned long& size)
{
	size += sizeof(VarValue);
	size += sizeof(string);
	size += this->varKey.size() + 1;
	size += sizeof(string);
	size += this->indicies.size() + 1;

//	for(vector<string>::iterator it=indicies.begin();it!=indicies.end();it++)
//	{
//		size += sizeof(string);
//		size += (*it).size()+1;
//	}
}
