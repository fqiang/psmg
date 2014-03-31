/*
 * SyntaxNodeValue.cpp
 *
 *  Created on: 28 Sep 2011
 *      Author: s0965328
 */

#include "SyntaxNodeValue.h"

#include "ObjComp.h"
#include "../parser/sml.tab.h"

SyntaxNodeValue::SyntaxNodeValue(double val) : SyntaxNode(VALUE), val(val)
{
	LOG("SyntaxNodeValue constructor --- val["<<val<<"]");
}

SyntaxNodeValue::~SyntaxNodeValue() {

}

string SyntaxNodeValue::value() const {
	assert(opCode == VALUE);
	std::ostringstream ost;
	ost << val;
	return ost.str();
}

void SyntaxNodeValue::calculateMemoryUsage(unsigned long& size) {
	size += sizeof(SyntaxNodeValue);
	this->calculateBaseValueVector(size);
}
