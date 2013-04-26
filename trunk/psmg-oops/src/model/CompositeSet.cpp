/*
 * CompositeSet1.cpp
 *
 *  Created on: 27 Sep 2011
 *      Author: s0965328
 */

#include "CompositeSet.h"
#include "../util/global_util_functions.h"

CompositeSet::CompositeSet(int opCode_, SyntaxNode *set1, SyntaxNode *set2) :
      SetNode(opCode_, set1, set2)
{
	assert((opCode_ == CROSS) || (opCode_ == DIFF));
	LOG("CompositeSet constructor called  -- opCode["<<opCode_<<"]");
}

CompositeSet::~CompositeSet()
{
}

void CompositeSet::calculateMemoryUsage(unsigned long& size)
{
	size += sizeof(CompositeSet);
	this->calculateBaseValueVector(size);
}
