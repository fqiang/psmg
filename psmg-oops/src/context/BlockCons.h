/*
 * EMBlock.h
 *
 *  Created on: 16 May 2013
 *      Author: s0965328
 */

#ifndef BLOCKCONS_H_
#define BLOCKCONS_H_

#include "autodiff.h"
#include <vector>

class BlockDep;
class ExpandedModel;
using namespace std;

class BlockCons{
public:

	BlockCons(BlockDep*);
	virtual ~BlockCons();

	void logBlock(ExpandedModel*, ExpandedModel* emcol, ostream& out);

	BlockDep* block;
	vector<AutoDiff::Node*> cons;
};

#endif /* EMBLOCK_H_ */
