/*
 * ListNode.cpp
 *
 *  Created on: 28 Sep 2011
 *      Author: s0965328
 */

#include "../util/global_util_functions.h"
#include "ListNode.h"
#include "../parser/sml.tab.h"

ListNode::ListNode(int opCode, SyntaxNode *val1, SyntaxNode *val2) :
     SyntaxNode(opCode, val1, val2)
{
	LOG("ListNode  constructor called -- with opCode["<<opCode<<"] val1["<<val1<<"]");
}

ListNode::~ListNode()
{
}

ostream& ListNode::put(ostream& s) const {
   SyntaxNode::iterator i = begin();
   if (i == end())
     return s;

   char sep = (opCode == ' ') ? ' ' : ',';

   s << *i;
   while (++i != end())
      s << sep << *i;

   return s;
}

/* --------------------------------------------------------------------------
addItemToListOrCreate
-------------------------------------------------------------------------- */
/** A 'List' is a SyntaxNode of opCode COMMA or ' ' with a variable number
 *  of arguments.
 *  This function takes (a possibly existing) list and adds an item to it.
 *
 *  Both the list and the item can be NULL:
 *  - if the item is NULL then the old list is simply returned;
 *  - if the list is NULL then a list with opCode 'oc' is created from the
 *    single item that is passed.
 */
ListNode * addItemToListOrCreate(int oc, ListNode *list, SyntaxNode *newitem)
{
	assert(oc==COMMA);
	assert(newitem!=NULL);
	LOG("In addItemToListOrCreate -- ");
	if(newitem==NULL)
	{
		return list;
	}

	if (list!=NULL)
	{
		LOG("list ["<<list<<"] nchild["<<list->values.size()<<"]");
		assert(oc == list->getOpCode());
		return (ListNode*) list->push_back(newitem);
	}
	else
	{
		assert(oc==' '||oc==COMMA);
		assert(oc!=' ');  //feng -- this will always true at today 's code
		return new ListNode(oc, newitem,NULL);
	}
	LOG("Exit addItemToListOrCreate -- ");
}

void ListNode::calculateMemoryUsage(unsigned long& size)
{
	size += sizeof(ListNode);
	this->calculateBaseValueVector(size);
}
