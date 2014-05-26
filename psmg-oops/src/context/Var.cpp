/*
 * Var.cpp
 *
 *  Created on: 25 Mar 2014
 *      Author: s0965328
 */


#include "Var.h"
#include "VarSingle.h"
#include "../util/util.h"

Var::Var(string& name_): name(name_) {

}

Var::~Var() {
	var_multi_map_by_order& var_by_order = this->varMultiMap.get<0>();
	var_multi_map_by_order::iterator i = var_by_order.begin();
	for(;i!=var_by_order.end();i++){
		VarSingle* varsingle = *i;
		delete varsingle;
	}
	this->varMultiMap.clear();
}

string Var::toString()
{
	ostringstream oss;
	oss<<"Var["<<name<<"]"<<endl;

	var_multi_map_by_order& var_by_order = this->varMultiMap.get<0>();
	var_multi_map_by_order::iterator i = var_by_order.begin();
	for(;i!=var_by_order.end();i++){
		oss<<"           "<<(*i)->toString()<<endl;
	}
	return oss.str();
}
void Var::calculateMemoryUsage(unsigned long& size)
{
	MEM("Var::calculateMemoryUsage -- name["<<name<<"]");
	long pre = size;
	size += sizeof(Var);
	size += name.size() + 1;

	var_multi_map_by_order& var_by_order = this->varMultiMap.get<0>();
	var_multi_map_by_order::iterator i = var_by_order.begin();
	for(;i!=var_by_order.end();i++){
		(*i)->calculateMemoryUsage(size);
	}
//	cout<<this->toString()<<endl;
	MEM(" -- this Var usage ["<<size-pre<<"]");
}
