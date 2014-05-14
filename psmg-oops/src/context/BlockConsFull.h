/*
 * EMBlock.h
 *
 *  Created on: 16 May 2013
 *      Author: s0965328
 */

#ifndef BLOCKCONSFULL_H_
#define BLOCKCONSFULL_H_

#include "autodiff.h"
#include <vector>

class BlockDep;
class ExpandedModel;
using namespace std;

class BlockConsFull{
public:

	BlockConsFull(BlockDep*);
	virtual ~BlockConsFull();

	void logBlock(ExpandedModel*);

	BlockDep* block;
	vector<AutoDiff::Node*> cons;
};

#endif /* EMBLOCKFULL_H_ */
