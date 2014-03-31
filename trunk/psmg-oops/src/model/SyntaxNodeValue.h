/*
 * SyntaxNodeValue.h
 *
 *  Created on: 28 Sep 2011
 *      Author: s0965328
 */

#ifndef VALUENODE_H_
#define VALUENODE_H_

#include "SyntaxNode.h"
#include <sstream>

/** @class SyntaxNodeValue
 *  Represents a value.
 */
class SyntaxNodeValue : public SyntaxNode
{
  public:
   const double val;

  public:
   SyntaxNodeValue(double new_value);
   virtual ~SyntaxNodeValue();
   std::string value() const;

   std::ostream& put(std::ostream&s) { return s << this->val; }
   SyntaxNode *deep_copy() { return new SyntaxNodeValue(val); }
   SyntaxNode *clone() { return deep_copy(); }
   virtual void calculateMemoryUsage(unsigned long& size);
};


#endif /* VALUENODE_H_ */
