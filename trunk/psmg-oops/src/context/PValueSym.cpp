/*
 * SymValue.cpp
 *
 *  Created on: 22 Mar 2014
 *      Author: s0965328
 */

#include <typeinfo>
#include <sstream>
#include "../util/util.h"
#include "PValueSym.h"

PValueSym::PValueSym(const string& v) : PValue(), value(v){
	// TODO Auto-generated constructor stub
}

PValueSym::PValueSym(const PValueSym& other) : PValue(other), value(other.value)
{
	TRACE("PValueSym copy "<<this->toString());
}
PValueSym::~PValueSym() {
	TRACE("PValueSym delete "<<this->toString());
}
PValueSym* PValueSym::clone()
{
	TRACE("PValueSym clone "<<this->toString());
	return new PValueSym(*this);
}

PValueSym* PValueSym::plus(PValue* other) {
	return error();
}
PValueSym* PValueSym::accumulate(PValue* other) {
	return error();
}
PValueSym* PValueSym::minus(PValue* other) {
	return error();
}
PValueSym* PValueSym::neg()
{
	return error();
}
PValueSym* PValueSym::times(PValue* other) {
	return error();
}
PValueSym* PValueSym::divid(PValue* other) {
	return error();
}
PValueSym* PValueSym::power(PValue* other) {
	return error();
}
PValueSym* PValueSym::power(double& other) {
	return error();
}

bool PValueSym::isGT(PValue* other){
	assert(false);
	return false;
}
bool PValueSym::isEQ(PValue* other)
{
	assert(typeid(*other)==typeid(PValueSym));
	return this->value.compare(static_cast<PValueSym*>(other)->value)==0;
}
bool PValueSym::isNE(PValue* other)
{
	assert(typeid(*other)==typeid(PValueSym));
	return this->value.compare(static_cast<PValueSym*>(other)->value)!=0;
}


string PValueSym::toString(){
	ostringstream oss;
	oss<<"{SymValue:"<<this->value<<"}";
	return oss.str();
}

void PValueSym::calculateMemoryUsage(unsigned long& size)
{
	size+=sizeof(PValueSym);
	size+=this->value.size() + 1;
}

PValueSym* PValueSym::error()
{
	assert(false);
	return NULL;
}
