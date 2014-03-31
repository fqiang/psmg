/*
 * EMBlock.h
 *
 *  Created on: 16 May 2013
 *      Author: s0965328
 */

#ifndef CONSBLOCK_H_
#define CONSLOCK_H_

#include "ExpandedModel.h"
#include "autodiff.h"
#include "Block.h"
#include <vector>

using namespace std;

class BlockCons{
public:

	BlockCons(Block*);
	virtual ~BlockCons();

	void logBlock(ExpandedModel*, ExpandedModel*, ostream& out);

	Block* block;
	vector<AutoDiff::Node*> cons;
};

#endif /* EMBLOCK_H_ */
