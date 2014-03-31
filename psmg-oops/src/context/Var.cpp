/*
 * Var.cpp
 *
 *  Created on: 25 Mar 2014
 *      Author: s0965328
 */


#include "Var.h"

Var::Var(string& name_): name(name_) {

}

Var::~Var() {
	this->varMultiMap.clear();
}

string Var::toString()
{
	ostringstream oss;
	oss<<"Var["<<name<<"]"<<endl;

	var_multi_map_by_order& var_by_order = this->varMultiMap.get<0>();
	var_multi_map_by_order::iterator i = var_by_order.begin();
	for(;i!=var_by_order.end();i++){
		oss<<"           "<<i->toString()<<endl;
	}
	return oss.str();
}
void Var::calculateMemoryUsage(unsigned long& size)
{
	LOG_MEM("Var::calculateMemoryUsage -- name["<<indicies<<"]");
	long pre = size;
	size += sizeof(Var);
	size += name.size();
	var_multi_map_by_order& var_by_order = this->varMultiMap.get<0>();
	var_multi_map_by_order::iterator i = var_by_order.begin();
	for(;i!=var_by_order.end();i++){
		i->calculateMemoryUsage(size);
	}
	LOG_MEM(" -- this Var usage ["<<size-pre<<"]");
}
