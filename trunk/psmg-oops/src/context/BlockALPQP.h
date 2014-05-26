/*
 * BlockLP.h
 *
 *  Created on: 3 Apr 2014
 *      Author: s0965328
 */

#ifndef BLOCKALPQP_H_
#define BLOCKALPQP_H_

#include <vector>
#include "../autodiff/autodiff.h"

class ExpandedModel;

class BlockALPQP {
public:

	BlockALPQP();
	virtual ~BlockALPQP();

	//calculated constraint set for the emrow.
	//same emrow X different emcol will give different part of the jacobian matrix declared by the emrow
	std::vector<AutoDiff::Node*> cons;

	void logBlock(ExpandedModel* emrow, ExpandedModel* emcol);
};

#endif /* BLOCKALPQP_H_ */
