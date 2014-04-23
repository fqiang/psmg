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

  public:
   string id;
   SyntaxNodeID(string i);
   SyntaxNodeID(const SyntaxNodeID& src);
   ~SyntaxNodeID();
   virtual SyntaxNode* clone();

   std::ostream& put(std::ostream& s) {
      return s << id;
   }

   virtual void calculateMemoryUsage(unsigned long& size);
};

#endif /* IDNODE_H_ */
