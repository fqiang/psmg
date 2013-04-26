#ifndef SIMPLSET_H_
#define SIMPLSET_H_
/*
 * SimpleSet.h
 *
 *  Created on: 27 Sep 2011
 *      Author: s0965328
 */

/** @class SimpleSet
 *  This class represents a contiguous set defined using 1..T or similar.
 */

#include "SetNode.h"

class SimpleSet: public SetNode {
private:
   int lower_bound_;
   SyntaxNode *lbc_;
   int upper_bound_;
   SyntaxNode *ubc_;

public:
   SimpleSet(SyntaxNode *bnd1, SyntaxNode *bnd2);

   /// Retrieve the members of the set
   //std::vector<std::string> members(AmplModel& context) const;
   virtual void calculateMemoryUsage(unsigned long& size);
};

#endif
