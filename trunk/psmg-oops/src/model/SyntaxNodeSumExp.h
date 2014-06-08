/*
 * SyntaxNodeSumExp.h
 *
 *  Created on: 9 May 2014
 *      Author: s0965328
 */

#ifndef SYNTAXNODESUMEXP_H_
#define SYNTAXNODESUMEXP_H_

#include "SyntaxNode.h"

class SyntaxNodeSumExp: public SyntaxNode {
public:
	int nProbs; //! the number of probls applied on this SumExp node.
				//! this should be the same as the number of Prob[n1]*...*Prob[n] times expr
				//! the n1, n2 ... should be decoded according to the dummy variable of the emcol and its parent

	SyntaxNodeSumExp(SyntaxNode* expr,int level);
	SyntaxNodeSumExp(const SyntaxNodeSumExp& src);
	virtual ~SyntaxNodeSumExp();

	virtual std::ostream& put(std::ostream& s);
	virtual void calculateMemoryUsage(unsigned long& size);
};

#endif /* SYNTAXNODESUMEXP_H_ */
