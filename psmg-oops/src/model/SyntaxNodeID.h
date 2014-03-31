/*
 * SyntaxNodeID.h
 *
 *  Created on: 28 Sep 2011
 *      Author: s0965328
 */

#ifndef IDNODE_H_
#define IDNODE_H_

#include "SyntaxNode.h"

using namespace std;

/* ----------------------------------------------------------------------------
SyntaxNodeID
---------------------------------------------------------------------------- */
/** @class SyntaxNodeID
 *  A node on the tree representing a user identifier (ie variable name).
 */
class SyntaxNodeID : public SyntaxNode {

 private:
   long stochparent;

  public:
   string id;
   SyntaxNodeID(string id, long stochparent=0);
   void findIDREF(std::list<ModelComp*> &lmc) { return; }
   void findIDREF(std::list<SyntaxNode*> *lnd) { return; }
   // We never search for ID:
   void findOpCode(int oc, std::list<SyntaxNode*> *lnd);
   SyntaxNode *deep_copy();
   std::ostream& put(std::ostream& s) {
      return s << id;
   }

   void setStochParent(long parent) {
     stochparent = parent;
   }

   long getStochParent() const {
     return stochparent;
   }
   virtual void calculateMemoryUsage(unsigned long& size);
};

#endif /* IDNODE_H_ */
