/*
 * SyntaxNodeString.cpp
 *
 *  Created on: 18 Apr 2014
 *      Author: s0965328
 */

#include "SyntaxNodeString.h"

#include "ObjComp.h"
#include "../parser/sml.tab.h"

SyntaxNodeString::SyntaxNodeString(string& val) : SyntaxNode(STRING),val(val){

}

SyntaxNodeString::~SyntaxNodeString() {
}

SyntaxNodeString::SyntaxNodeString(const SyntaxNodeString& src):SyntaxNode(src)
{
	val = src.val;
}

SyntaxNodeString* SyntaxNodeString::clone()
{
	return new SyntaxNodeString(*this);
}

string SyntaxNodeString::value() const {
	return val;
}

void SyntaxNodeString::calculateMemoryUsage(unsigned long& size) {
	size += sizeof(SyntaxNodeString);
	this->calculateBaseValueVector(size);
}
