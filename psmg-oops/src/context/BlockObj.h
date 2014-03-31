/*
 * CBlock.h
 *
 *  Created on: 24 Sep 2013
 *      Author: s0965328
 */

#ifndef OBJBLOCK_H_
#define OBJBLOCK_H_

#include "ExpandedModel.h"
#include "autodiff.h"
#include "Block.h"



using namespace std;

class BlockObj {
public:
	BlockObj(Block*);
	virtual ~BlockObj();

	Block* block;
	AutoDiff::Node*	objective;

	void logBlock(ExpandedModel*, ExpandedModel*, ostream& out);
};

#endif /* CBLOCK_H_ */
