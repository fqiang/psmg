/*
 * BlockLP.h
 *
 *  Created on: 3 Apr 2014
 *      Author: s0965328
 */

#ifndef BLOCKLP_H_
#define BLOCKLP_H_

#include <vector>
#include "autodiff.h"

class ExpandedModel;

class BlockLP {
public:

	BlockLP(ExpandedModel*,ExpandedModel*);
	virtual ~BlockLP();

	ExpandedModel* emrow;
	ExpandedModel* emcol;
	//calculated constraint set for the emrow.
	//same emrow X different emcol will give different part of the jacobian matrix declared by the emrow
	std::vector<AutoDiff::Node*> cons;

	void logBlock(ExpandedModel* emrow, ExpandedModel* emcol,ostream& out);
};

#endif /* BLOCKLP_H_ */
