/*
 * BlockANLP.h
 *
 *  Created on: 14 May 2014
 *      Author: s0965328
 */

#ifndef BLOCKANLP_H_
#define BLOCKANLP_H_

#include "../autodiff/autodiff.h"
#include <vector>

class BlockDep;
class ExpandedModel;
using namespace std;

class BlockANLP {
public:
	BlockANLP(BlockDep*);
	virtual ~BlockANLP();

	BlockDep* block;
	vector<AutoDiff::Node*> cons;

	void logBlock(ExpandedModel*, ExpandedModel* emcol);
};

#endif /* BLOCKANLP_H_ */
