/*
 * Var.cpp
 *
 *  Created on: 11 Oct 2011
 *      Author: s0965328
 */

#include "../util/util.h"
#include "VarSingle.h"
#include "../autodiff/VNode.h"
#include <string>
#include <sstream>

using namespace std;
using namespace AutoDiff;

VarSingle::VarSingle(string& i, uint idx, double u, double l)
:indicies(i),ub(u),lb(l), adv(idx){
	TRACE("VarSingle::VarSingle indicies["<<indicies<<"] - idx ["<<idx<<"] ub["<<ub<<"] lb["<<lb<<"]");
}

VarSingle::~VarSingle() {
	TRACE("VarSingle --- delete --- "<<this->toString());
}

string VarSingle::toString()
{
	ostringstream oss;
	oss<<"{"<<lb<<"<["<<indicies<<"]<"<<ub<<"}  ---  {"<<this->adv.toString(0)<<"}";
	return oss.str();
}

void VarSingle::calculateMemoryUsage(unsigned long& size) const
{
	MEM("VarSingle::calculateMemoryUsage -- name["<<indicies<<"]");
	long pre = size;
	size += sizeof(VarSingle);
	size += indicies.size();
	MEM(" -- this VarSingle usage ["<<size-pre<<"]");
}
