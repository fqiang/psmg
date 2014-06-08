/*
 * SyntaxNodeSumExp.cpp
 *
 *  Created on: 9 May 2014
 *      Author: s0965328
 */

#include "SyntaxNodeSumExp.h"


#include "ObjComp.h"
#include "../parser/sml.tab.h"

SyntaxNodeSumExp::SyntaxNodeSumExp(SyntaxNode* expr, int nPb) : SyntaxNode(SUMEXP,expr),nProbs(nPb){
	// TODO Auto-generated constructor stub
}

SyntaxNodeSumExp::SyntaxNodeSumExp(const SyntaxNodeSumExp& src) : SyntaxNode(src)
{
	nProbs = src.nProbs;
}

SyntaxNodeSumExp::~SyntaxNodeSumExp() {
	// TODO Auto-generated destructor stub
}

ostream& SyntaxNodeSumExp::put(ostream& s)
{
	assert(nchild()==1);
	s << "sum_exp{" << nProbs<<"}";
	s << "(" << this->values[0] <<")";
	return s;
}

void SyntaxNodeSumExp::calculateMemoryUsage(unsigned long& size)
{
	size += sizeof(SyntaxNodeSumExp);
	this->calculateBaseValueVector(size);
}
