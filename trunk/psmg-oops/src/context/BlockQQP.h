/*
 * BlockQQP.h
 *
 *  Created on: 14 May 2014
 *      Author: s0965328
 */

#ifndef BLOCKQQP_H_
#define BLOCKQQP_H_

#include <vector>
#include "../autodiff/autodiff.h"

class ExpandedModel;
class BlockDep;


class BlockQQP {
public:
	BlockQQP(BlockDep* b);

	BlockDep* block;
	std::vector<AutoDiff::Node*> objs;
	virtual ~BlockQQP();

	void logBlock(ExpandedModel*, ExpandedModel*);
};

#endif /* BLOCKQQP_H_ */
