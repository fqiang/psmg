/*
 * ValueNode.h
 *
 *  Created on: 28 Sep 2011
 *      Author: s0965328
 */

#ifndef VALUENODE_H_
#define VALUENODE_H_

#include "SyntaxNode.h"
#include "../util/global_util_functions.h"
#include <sstream>

/** @class ValueNode
 *  Represents a value.
 */
class ValueNode : public SyntaxNode
{
  public:
   const double value;

  public:
   ValueNode(double new_value);
   ValueNode(long new_value);
   virtual ~ValueNode();
   std::string getValue() const;

   std::ostream& put(std::ostream&s) const { return s << this->value; }
   SyntaxNode *deep_copy() { return new ValueNode(value); }
   SyntaxNode *clone() { return deep_copy(); }
   virtual void calculateMemoryUsage(unsigned long& size);
};


#endif /* VALUENODE_H_ */
