/*
 * IndexingSet.cpp
 *
 *  Created on: Apr 16, 2012
 *      Author: s0965328
 */

#include "IndexingSet.h"

IndexingSet::IndexingSet(SyntaxNode* set1)
	:SetNode(LBRACE, set1)
{
	LOG("IndexingSet constructor called -- opCode["<<LBRACE<<"]");

}

IndexingSet::~IndexingSet()
{
	// TODO Auto-generated destructor stub
}

void IndexingSet::calculateMemoryUsage(unsigned long& size)
{
	size += sizeof(IndexingSet);
	this->calculateBaseValueVector(size);
}
