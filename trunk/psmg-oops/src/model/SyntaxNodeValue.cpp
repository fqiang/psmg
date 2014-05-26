/*
 * SyntaxNodeValue.cpp
 *
 *  Created on: 28 Sep 2011
 *      Author: s0965328
 */

#include "SyntaxNodeValue.h"

#include "../util/util.h"

#include "ObjComp.h"
#include "../parser/sml.tab.h"

SyntaxNodeValue::SyntaxNodeValue(double val) : SyntaxNode(VALUE), val(val)
{
	TRACE("SyntaxNodeValue constructor --- val["<<val<<"]");
}

SyntaxNodeValue::~SyntaxNodeValue() {
}

SyntaxNodeValue::SyntaxNodeValue(const SyntaxNodeValue& src):SyntaxNode(src)
{
	val = src.val;
}

SyntaxNodeValue* SyntaxNodeValue::clone()
{
	return new SyntaxNodeValue(*this);
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
