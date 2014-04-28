/*
 * SyntaxNodeID.cpp
 *
 *  Created on: 28 Sep 2011
 *      Author: s0965328
 */

#include "SyntaxNodeID.h"
#include "SyntaxNodeString.h"
#include "ObjComp.h"
#include "../st_model/StochCtx.h"
#include "../parser/sml.tab.h"
#include <sstream>

using namespace  std;


SyntaxNodeID::SyntaxNodeID(string i) :
   SyntaxNode(ID), id(i){

	LOG("SyntaxNodeID constructor called  - opCode["<<ID<<"] ID["<<id<<"]");
}

SyntaxNodeID::~SyntaxNodeID()
{
}

SyntaxNodeID::SyntaxNodeID(const SyntaxNodeID& src):SyntaxNode(src)
{
	id = src.id;
}
SyntaxNode*  SyntaxNodeID::clone()
{
	SyntaxNode* newnode = NULL;
	//translate the node dummy and stage dummy if necessary
	if(id.compare(SCTX::currCtx->nd) == 0)
	{
		newnode = new SyntaxNodeID(SCTX::currCtx->model_dummy);
	}
	else if(id.compare(SCTX::currCtx->st) == 0)
	{
		newnode = new SyntaxNodeString(SCTX::currCtx->stagename);
	}
	else
	{
		newnode =  new SyntaxNodeID(*this);
	}
	return newnode;
}

void SyntaxNodeID::calculateMemoryUsage(unsigned long& size)
{
	size += sizeof(SyntaxNodeID);
	size += this->id.size() + 1;
	this->calculateBaseValueVector(size);
}
