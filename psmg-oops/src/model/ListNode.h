/*
 * ListNode.h
 *
 *  Created on: 28 Sep 2011
 *      Author: s0965328
 */

#ifndef LISTNODE_H_
#define LISTNODE_H_

#include "SyntaxNode.h"

using namespace std;

/** @class ListNode
 *  Represents a comma separated list of SyntaxNodes.
 */
class ListNode: public SyntaxNode {

  public:
   ListNode(int opCode=',', SyntaxNode *val1 = NULL, SyntaxNode *val2 = NULL);
   ostream& put(std::ostream& s) const;
   virtual ~ListNode();
   virtual void calculateMemoryUsage(unsigned long& size);
};

ListNode *addItemToListOrCreate(int oc, ListNode *list, SyntaxNode *newitem);

#endif /* LISTNODE_H_ */
