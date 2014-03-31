/*
 * Var.h
 *
 *  Created on: 11 Oct 2011
 *      Author: s0965328
 */

#ifndef VARSINGLE_H_
#define VARSINGLE_H_

#include <string>
#include <limits>

#include "autodiff.h"

using namespace std;

class VarSingle {
public:
	string indicies;
	double ub;
	double lb;
	AutoDiff::Node* adv;

	VarSingle(string& indicies_, double ub_, double lb_,AutoDiff::Node* v);
	virtual ~VarSingle();

	string toString() const;
	void calculateMemoryUsage(unsigned long& size) const;
};

#endif /* VARSINGLE_H_ */
