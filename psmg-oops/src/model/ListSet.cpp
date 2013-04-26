/*
 * ListSet1.cpp
 *
 *  Created on: 27 Sep 2011
 *      Author: s0965328
 */

#include "ListSet.h"
#include "../util/global_util_functions.h"


ListSet::ListSet(SyntaxNode *list) : SetNode(LBRACE, list,NULL)
{
	LOG("ListSet constructor called  --- opCode["<<LBRACE<<"] list["<<list<<"]");
}

ListSet::~ListSet()
{

}

void ListSet::calculateMemoryUsage(unsigned long& size)
{
	size += sizeof(ListSet);
	this->calculateBaseValueVector(size);
}
