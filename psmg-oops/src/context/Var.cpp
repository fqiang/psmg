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

Var::Var(string name_):name(name_),card(0),numIndicies(0) {
	LOG("Var constructor name["<<name<<"]");

}

Var::~Var() {
	for(vector<VarValue*>::iterator it=varList.begin();it!=varList.end();it++){
		delete *(it);
	}
	varList.clear();
}

void Var::addVarValue(VarValue* val)
{
	assert(this->numIndicies == val->numIndicies);
	LOG("addVarValue -- Var["<<this->name<<"] val["<<val->toString()<<"] numIndicies["<<numIndicies<<"]");
	this->varList.push_back(val);
	//this->card++;
}

int Var::getCard()
{
	return this->card;
}

string Var::toString()
{
	ostringstream oss;
	vector<VarValue*>::iterator it;
	for(it=this->varList.begin();it!=this->varList.end();it++)
	{
		oss<<name<<(*it)->toString()<<endl;
	}
	return oss.str();
}

void Var::calculateMemoryUsage(unsigned long& size)
{
	LOG_MEM("Var::calculateMemoryUsage -- name["<<name<<"]");
	long pre = size;
	size += sizeof(Var);
	size += name.size()+1;
	vector<VarValue*>::iterator it;
	for(it=varList.begin();it!=varList.end();it++)
	{
		size += sizeof(VarValue*);
		(*it)->calculateMemoryUsage(size);
	}
	LOG_MEM(" -- this var usage ["<<size-pre<<"]");
}
