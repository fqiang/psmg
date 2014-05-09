/*
 * ValueValue.cpp
 *
 *  Created on: 22 Mar 2014
 *      Author: s0965328
 */

#include <cassert>
#include <typeinfo>
#include <cmath>
#include <sstream>
#include "../util/util.h"
#include "PValueValue.h"

PValueValue::PValueValue(double v) : PValue(), value(v){
	// TODO Auto-generated constructor stub
}
PValueValue::PValueValue(const PValueValue& other) : PValue(other), value(other.value)
{
	LOG("PValueVaule copy "<<this->toString());
}

PValueValue::~PValueValue() {
	LOG("PValueVaule delete "<<this->toString());
}

PValueValue* PValueValue::clone()
{
	LOG("PValueValue::clone() -- "<<this->toString());
	return new PValueValue(*this);
}

PValueValue* PValueValue::plus(PValue* other)
{
	assert(typeid(*other)==typeid(PValueValue));
	PValueValue* rval = new PValueValue(*this);
	rval->value = this->value +  static_cast<PValueValue*>(other)->value;
	return rval;
}
PValueValue* PValueValue::accumulate(PValue* other)
{
	assert(typeid(*other)==typeid(PValueValue));
	this->value = this->value +  static_cast<PValueValue*>(other)->value;
	return this;
}
PValueValue* PValueValue::minus(PValue* other)
{
	assert(typeid(*other)==typeid(PValueValue));
	PValueValue* rval = new PValueValue(*this);
	rval->value =this->value - static_cast<PValueValue*>(other)->value;
	return rval;
}
PValueValue* PValueValue::neg()
{
	PValueValue* rval = new PValueValue(*this);
	rval->value = -rval->value;
	return rval;
}
PValueValue* PValueValue::times(PValue* other)
{
	assert(typeid(*other)==typeid(PValueValue));
	PValueValue* rval = new PValueValue(*this);
	rval->value =this->value * static_cast<PValueValue*>(other)->value;
	return rval;
}
PValueValue* PValueValue::divid(PValue* other)
{
	assert(typeid(*other)==typeid(PValueValue));
	PValueValue* rval = new PValueValue(*this);
	rval->value =this->value / static_cast<PValueValue*>(other)->value;
	return rval;
}
PValueValue* PValueValue::power(PValue* other)
{
	assert(typeid(*other)==typeid(PValueValue));
	PValueValue* rval = new PValueValue(*this);
	rval->value =pow(this->value,static_cast<PValueValue*>(other)->value);
	return rval;
}
PValueValue* PValueValue::power(double& other)
{
	PValueValue* rval = new PValueValue(*this);
	rval->value =pow(this->value,other);
	return rval;
}

bool PValueValue::isGT(PValue* other)
{
	assert(typeid(*other)==typeid(PValueValue));
	return (this->value - static_cast<PValueValue*>(other)->value)>0;
}
bool PValueValue::isEQ(PValue* other)
{
	assert(typeid(*other)==typeid(PValueValue));
	return (this->value - static_cast<PValueValue*>(other)->value)==0;
}
bool PValueValue::isNE(PValue* other)
{
	assert(typeid(*other)==typeid(PValueValue));
	return (this->value - static_cast<PValueValue*>(other)->value)!=0;
}


string PValueValue::toString(){
	ostringstream oss;
	oss<<"{ValueValue:"<<this->value<<"}";
	return oss.str();
}

void PValueValue::calculateMemoryUsage(unsigned long& size)
{
	size += sizeof(PValueValue);
}
