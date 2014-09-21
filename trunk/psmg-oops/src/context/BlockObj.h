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
//! note: assume the objective function are only involved variables declared in it's own expanded model and parents.
//!       extra constraints can be created to relate to other parent or children blocks.
//! 1. can be used for computing the objective value
//! 2. can also be used for computing Lagrangian Hessian (only keep higher order term).
class BlockObj {
public:
	BlockObj();
	virtual ~BlockObj();

	AutoDiff::Node*	objective;

	void logBlock(ExpandedModel*);
	void logBlock(ExpandedModel*,ExpandedModel* emcol);
};

#endif /* CBLOCK_H_ */
