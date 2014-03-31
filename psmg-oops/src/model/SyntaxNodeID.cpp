/*
 * SyntaxNodeID.cpp
 *
 *  Created on: 28 Sep 2011
 *      Author: s0965328
 */

#include "SyntaxNodeID.h"

#include "ObjComp.h"
#include "../parser/sml.tab.h"
#include <sstream>

using namespace  std;


SyntaxNodeID::SyntaxNodeID(string _id, long _stochparent) :
   SyntaxNode(ID), id(_id), stochparent(_stochparent) {

	LOG("SyntaxNodeID constructor called  - opCode["<<ID<<"] ID["<<id<<"]");
}

void SyntaxNodeID::findOpCode(int oc, list<SyntaxNode*> *lnd) {
   if(oc==ID) lnd->push_back(this);
}

SyntaxNode* SyntaxNodeID::deep_copy()
{
	return new SyntaxNodeID(id.c_str(), stochparent);
}

void SyntaxNodeID::calculateMemoryUsage(unsigned long& size)
{
	size += sizeof(SyntaxNodeID);
	size += this->id.size() + 1;
	this->calculateBaseValueVector(size);
}
