/*
 * BlockHV.h
 *
 *  Created on: 2 Apr 2014
 *      Author: s0965328
 */

#ifndef BLOCKHV_H_
#define BLOCKHV_H_

#include "autodiff.h"
#include <vector>


class BlockCons;
class BlockObj;
class ExpandedModel;

class BlockHV {
public:
	BlockHV(BlockCons* bc,BlockObj* o);
	virtual ~BlockHV();

	void logBlock(ExpandedModel*, ExpandedModel*, ostream& out);

	BlockCons* blkcons;
	BlockObj* blkobj;
};

#endif /* BLOCKHV_H_ */
