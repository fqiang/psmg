/*
 * PValue.h
 *
 *  Created on: 20 Sep 2014
 *      Author: s0965328
 */

#ifndef PVAL_H_
#define PVAL_H_

#include "PNode.h"

namespace AutoDiff {

class PVal: public AutoDiff::PNode {
public:

	double value;

	PVal(double pval);
	virtual ~PVal();

	double& pval();
};

} /* namespace AutoDiff */
#endif /* PVALUE_H_ */
