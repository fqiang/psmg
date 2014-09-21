/*
 * PValue.cpp
 *
 *  Created on: 20 Sep 2014
 *      Author: s0965328
 */

#include "PVal.h"

namespace AutoDiff {

PVal::PVal(double pval):PNode(), value(pval) {
	// TODO Auto-generated constructor stub

}

PVal::~PVal() {
	// TODO Auto-generated destructor stub
}


double& PVal::pval() {
	return value;
}

} /* namespace AutoDiff */
