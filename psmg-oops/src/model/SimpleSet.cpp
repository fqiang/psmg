/*
 * SimpleSet.cpp
 *
 *  Created on: 27 Sep 2011
 *      Author: s0965328
 */

#include "SimpleSet.h"
#include "ValueNode.h"
#include "../util/global_util_functions.h"

using namespace std;

SimpleSet::SimpleSet(SyntaxNode *bnd1, SyntaxNode *bnd2) :
   SetNode(DOTDOT, bnd1, bnd2),
   lbc_(bnd1),
   ubc_(bnd2)
{
	LOG("SimpleSet constructor called");

	ValueNode *inode;

	if( (inode = dynamic_cast<ValueNode*>(lbc_)) )
	{
	  lbc_ = NULL;
	  lower_bound_ = inode->value;
	}

	if( (inode = dynamic_cast<ValueNode*>(ubc_)) )
	{
	  ubc_ = NULL;
	  upper_bound_ = inode->value;
	}

	//parsed_ = !(lbc_ || ubc_);
}

void SimpleSet::calculateMemoryUsage(unsigned long& size)
{
	size += sizeof(SimpleSet);
	this->calculateBaseValueVector(size);
}

