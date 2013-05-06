/*
 * Var.cpp
 *
 *  Created on: 11 Oct 2011
 *      Author: s0965328
 */

#include "../util/global_util_functions.h"
#include "Var.h"
#include <string>
#include <sstream>

using namespace std;

Var::Var(string name_, int card_, int numInd_, double ub_, double lb_)
:name(name_),card(card_),numIndicies(numInd_),ub(ub_),lb(lb_) {
	LOG("Var constructor name["<<name<<"]");
}

Var::~Var() {
	this->indicies.clear();
	this->values.clear();
}

void Var::addVarValue(ostringstream& oss,double val)
{
//	assert(this->numIndicies == val->numIndicies);
	LOG("addVarValue -- Var["<<this->name<<"] val["<<val<<"] oss["<<oss.str()<<"]");
	this->indicies.push_back(oss.str());
	this->values.push_back(val);
}

void Var::addVarValue(vector<string>& ind, double val)
{
	LOG("addVarValue -- Var["<<this->name<<"]"<<"val:"<<val<<"]");
	ostringstream oss(ostringstream::out);
	for(vector<string>::iterator it=ind.begin();it!=ind.end();++it)
	{
		oss<<*it;
	}
	this->indicies.push_back(oss.str());
	this->values.push_back(val);

}

int Var::getCard()
{
	return this->card;
}

string Var::toString()
{
	ostringstream oss;
	vector<string>::iterator it;
	vector<double>::iterator it2;
	oss<<name<<"{";
	for(it=this->indicies.begin(),it2=this->values.begin();it!=this->indicies.end();it++,it2++)
	{
		oss<<"["<<(*it)<<"|"<<*it2<<"]";
	}
	oss<<"}";
	return oss.str();
}

void Var::calculateMemoryUsage(unsigned long& size)
{
	LOG_MEM("Var::calculateMemoryUsage -- name["<<name<<"]");
	long pre = size;
	size += sizeof(Var);
	size += name.size();
	vector<string>::iterator it;
	for(it=indicies.begin();it!=indicies.end();it++)
	{
		size += sizeof(string);
		size += (*it).size();
	}

	size += sizeof(double) * this->values.size();
	LOG_MEM(" -- this var usage ["<<size-pre<<"]");
}
