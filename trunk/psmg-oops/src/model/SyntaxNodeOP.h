/*
 * SyntaxNodeOP.h
 *
 *  Created on: 28 Sep 2011
 *      Author: s0965328
 */

#ifndef SYNTAXNODEOP_H_  //this is because OPNODE_H_ conflicts with AutoDiff::OPNode class
#define SYNTAXNODEOP_H_

#include "SyntaxNode.h"

/** @class OpNode
 *  Represents an operator.
 */
class SyntaxNodeOP : public SyntaxNode {
  public:
   SyntaxNodeOP(int opCode, SyntaxNode *op1, SyntaxNode *op2=NULL);
   ~SyntaxNodeOP();
   SyntaxNodeOP(const SyntaxNodeOP& src);

   virtual SyntaxNodeOP* clone();
   virtual std::ostream& put(std::ostream& s);
};

#endif /* OPNODE_H_ */
