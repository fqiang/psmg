/*
 * SingleSymParam.cpp
 *
 *  Created on: 31 Jan 2014
 *      Author: s0965328
 */

#include "ParamSingle.h"

ParamSingle::ParamSingle(ParamComp* comp) : Param(comp), value(NULL) {
	// TODO Auto-generated constructor stub

}

ParamSingle::~ParamSingle() {
	// TODO Auto-generated destructor stub
	delete this->value;
}

void ParamSingle::setParamValue(PValue* v)
{
	assert(v!=NULL);
	value = v;
	card++;
}
string ParamSingle::toString()
{
	ostringstream oss;
	oss<<"[SingleParam]["<<name<<"]: value["<<value->toString()<<"]"<<endl;
	return oss.str();
}

void ParamSingle::calculateMemoryUsage(unsigned long& size)
{
	size += sizeof(ParamSingle);
	value->calculateMemoryUsage(size);
}
