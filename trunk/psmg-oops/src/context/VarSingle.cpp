/*
 * Var.cpp
 *
 *  Created on: 11 Oct 2011
 *      Author: s0965328
 */

#include "../util/global_util_functions.h"
#include "VarSingle.h"
#include <string>
#include <sstream>

using namespace std;

VarSingle::VarSingle(string& i,double u, double l,AutoDiff::Node* v)
:indicies(i),ub(u),lb(l),adv(v) {
	LOG("VarSingle::VarSingle indicies["<<indicies<<"] - ub["<<ub<<"] lb["<<lb<<"]");
}

VarSingle::~VarSingle() {
	LOG("VarSingle --- delete --- "<<this->toString());
}

string VarSingle::toString() const
{
	ostringstream oss;
	oss<<"{"<<lb<<"<["<<indicies<<"]<"<<ub<<"}  ---  {"<<adv->toString(0)<<"}";
	return oss.str();
}

void VarSingle::calculateMemoryUsage(unsigned long& size) const
{
	LOG_MEM("VarSingle::calculateMemoryUsage -- name["<<indicies<<"]");
	long pre = size;
	size += sizeof(VarSingle);
	size += indicies.size();
	LOG_MEM(" -- this VarSingle usage ["<<size-pre<<"]");
}
