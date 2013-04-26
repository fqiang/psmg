/*
 * OpNode.h
 *
 *  Created on: 28 Sep 2011
 *      Author: s0965328
 */

#ifndef OPNODE_H_
#define OPNODE_H_

#include "SyntaxNode.h"

/** @class OpNode
 *  Represents an operator.
 */
class OpNode : public SyntaxNode {
  public:
   SyntaxNode *left;
   SyntaxNode *right;

  public:
   OpNode(int opCode, SyntaxNode *op1, SyntaxNode *op2=NULL);
   std::ostream& put(std::ostream& s) const;
   OpNode *deep_copy();
   OpNode *clone();
   void findIDREF(std::list<ModelComp*>& lmc) {
      if(left) left->findIDREF(lmc);
      if(right) right->findIDREF(lmc);
   }
   void findIDREF(std::list<SyntaxNode*> *lnd) {
      if(left) left->findIDREF(lnd);
      if(right) right->findIDREF(lnd);
   }
   void findOpCode(int oc, std::list<SyntaxNode*> *lnd) {
      if(left) left->findOpCode(oc, lnd);
      if(right) right->findOpCode(oc, lnd);
   }
   virtual void calculateMemoryUsage(unsigned long& size);
};

#endif /* OPNODE_H_ */
