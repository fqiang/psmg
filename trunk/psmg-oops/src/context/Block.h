/*
 * Block.h
 *
 *  Created on: 25 Sep 2013
 *      Author: s0965328
 */

#ifndef BLOCK_H_
#define BLOCK_H_

#include "ExpandedModel.h"
#include "autodiff.h"
#include <vector>
#include <boost/unordered_map.hpp>

using namespace std;

class Block {
public:
	Block();
	virtual ~Block();

	vector<ExpandedModel*> ems;

	virtual void logBlock(ostream& out);
};

#endif /* BLOCK_H_ */
