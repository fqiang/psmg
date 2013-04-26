/*
 * OpNode.cpp
 *
 *  Created on: 28 Sep 2011
 *      Author: s0965328
 */

#include "OpNode.h"
#include "../util/global_util_functions.h"
#include "../parser/sml.tab.h"

OpNode::OpNode(int opCode_, SyntaxNode *op1, SyntaxNode *op2) :
  SyntaxNode(opCode_, op1, op2), left(NULL)
{
	LOG("OpNode constructor called -- opCode["<<opCode_<<"]");
	assert(op1);
	if(!op2) {
		// Unary operation eg -1, op sign sits to left of operand
		right = op1;
	} else {
	// Binary operation
		left = op1;
		right = op2;
	}
}

OpNode *OpNode::deep_copy() {
   if(left) {
      return new OpNode(opCode, left->deep_copy(), right->deep_copy());
   } else {
      return new OpNode(opCode, right->deep_copy());
   }
}

OpNode *OpNode::clone() {
   if(left) {
      return new OpNode(opCode, left, right);
   } else {
      return new OpNode(opCode, right);
   }
}


ostream& OpNode::put(ostream &s) const {
  if(left) s << left << " ";

  switch(opCode) {
    case '+':
    case '-':
    case '*':
    case '/':
       s << (char) opCode;
       break;
    case ASSIGN:  s << "=";   break;
    case GE:      s << ">=";  break;
    case GT:      s << ">";   break;
    case LE:      s << "<=";  break;
    case LT:      s << "<";   break;
    case EQ:      s << "==";  break;
    case NE:      s << "!=";  break;
    case IN:      s << "in";  break;
    case DEFINED: s << ":=";  break;
    case POWER:   s << "**";  break;
    default:
      cerr << "Unknown opCode for OpNode: " << opCode << endl;
      exit(1);
  }

  s << " " << right;

  return s;
}

void OpNode::calculateMemoryUsage(unsigned long& size)
{
	size += sizeof(OpNode);
	this->calculateBaseValueVector(size);
}

