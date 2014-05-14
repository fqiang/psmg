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


class BlockConsFull;
class BlockObjFull;
class ExpandedModel;

class BlockHV {
public:
	BlockHV(BlockConsFull* bc,BlockObjFull* o);
	virtual ~BlockHV();

	void logBlock(ExpandedModel*, ExpandedModel*, ostream& out);

	BlockConsFull* blkcons;
	BlockObjFull* blkobj;
};

#endif /* BLOCKHV_H_ */
