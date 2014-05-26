/*
 * Block.h
 *
 *  Created on: 25 Sep 2013
 *      Author: s0965328
 */

#ifndef BLOCKDEP_H_
#define BLOCKDEP_H_

#include "../autodiff/autodiff.h"
#include <vector>

using namespace std;
class ExpandedModel;

class BlockDep {
public:
	BlockDep();
	virtual ~BlockDep();

	vector<ExpandedModel*> ems;

	uint getNumDepVars();
	virtual void logBlock(ostream& out);
};

#endif /* BLOCK_H_ */
