/*
 * PIndex.h
 *
 *  Created on: 20 Sep 2014
 *      Author: s0965328
 */

#ifndef PINDEX_H_
#define PINDEX_H_

#include "PNode.h"

namespace AutoDiff {

class PIndex: public AutoDiff::PNode {
public:
	uint idx;

	PIndex(uint idx);
	virtual ~PIndex();

	double& pval() ;
};

} /* namespace AutoDiff */
#endif /* PINDEX_H_ */
