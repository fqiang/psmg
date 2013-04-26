/*
 * IndexingSet.h
 *
 *  Created on: Apr 16, 2012
 *      Author: s0965328
 */

#ifndef INDEXINGSET_H_
#define INDEXINGSET_H_

#include "SetNode.h"

class IndexingSet: public SetNode
{
public:
	IndexingSet(SyntaxNode* set1);
	virtual ~IndexingSet();
	virtual void calculateMemoryUsage(unsigned long& size);
};

#endif /* INDEXINGSET_H_ */
