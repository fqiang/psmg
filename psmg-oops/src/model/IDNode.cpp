/*
 * IDNode.cpp
 *
 *  Created on: 28 Sep 2011
 *      Author: s0965328
 */

#include "IDNode.h"
#include "../parser/sml.tab.h"
#include "../util/global_util_functions.h"
#include <sstream>

using namespace  std;


IDNode::IDNode(const char* id, long new_stochparent) :
   SyntaxNode(ID), name(id), stochparent(new_stochparent) {

	LOG("IDNode constructor called  - opCode["<<ID<<"] ID["<<name<<"]");
}

void IDNode::findOpCode(int oc, list<SyntaxNode*> *lnd) {
   if(oc==ID) lnd->push_back(this);
}

SyntaxNode* IDNode::deep_copy()
{
	return new IDNode(name.c_str(), stochparent);
}

void IDNode::calculateMemoryUsage(unsigned long& size)
{
	size += sizeof(IDNode);
	size += this->name.size() + 1;
	this->calculateBaseValueVector(size);
}
