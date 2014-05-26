/*
 * CBlock.h
 *
 *  Created on: 24 Sep 2013
 *      Author: s0965328
 */

#ifndef BLOCKOBJ_H_
#define BLOCKOBJ_H_

#include "../autodiff/autodiff.h"

class ExpandedModel;
class BlockDep;

using namespace std;
//! the block objective
//! note: assume the objective function are only involved variabels declared in it's own expanded model.
//!       extra constraints can be created to relate to other parent or children blocks.
class BlockObjFull {
public:
	BlockObjFull(BlockDep* b);
	virtual ~BlockObjFull();

	BlockDep* block;
	AutoDiff::Node*	objective;

	void logBlock(ExpandedModel*);
};

#endif /* CBLOCK_H_ */
