/*
 * BlockHV.h
 *
 *  Created on: 2 Apr 2014
 *      Author: s0965328
 */

#ifndef BLOCKHV_H_
#define BLOCKHV_H_

#include "../autodiff/autodiff.h"
#include <vector>


class ExpandedModel;

using namespace std;

class BlockHV {
public:
	BlockHV();
	virtual ~BlockHV();

	AutoDiff::Node* node;
	void logBlock(ExpandedModel*, ExpandedModel*);

};

#endif /* BLOCKHV_H_ */
