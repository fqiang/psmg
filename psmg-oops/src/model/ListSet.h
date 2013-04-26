/*
 * ListSet1.h
 *
 *  Created on: 27 Sep 2011
 *      Author: s0965328
 */

#ifndef LISTSET_H_
#define LISTSET_H_

#include "SetNode.h"

class ListSet: public SetNode
{
	private:
	   //std::vector<std::string> set;
	public:
		ListSet(SyntaxNode *list);
		virtual ~ListSet();
		virtual void calculateMemoryUsage(unsigned long& size);
};

#endif /* LISTSET_H_ */
