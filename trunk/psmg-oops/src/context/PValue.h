/*
 * PValue.h
 *
 *  Created on: 22 Mar 2014
 *      Author: s0965328
 */

#ifndef PVALUE_H_
#define PVALUE_H_

#include <string>

using namespace std;

class PValue {
public:
	PValue();
	PValue(PValue&);
	virtual ~PValue();

	virtual PValue* clone() = 0;
	virtual PValue* plus(PValue* other) = 0;
	virtual PValue* accumulate(PValue* other) =0;
	virtual PValue* neg() = 0;
	virtual PValue* minus(PValue* other) = 0;
	virtual PValue* times(PValue* other) = 0;
	virtual PValue* divid(PValue* other) = 0;
	virtual PValue* power(PValue* other) = 0;
	virtual PValue* power(double&) = 0;

	virtual bool isGT(PValue*) =0;
	virtual bool isEQ(PValue*) =0;
	virtual bool isNE(PValue*) =0;

	virtual string toString() = 0;
	virtual void calculateMemoryUsage(unsigned long& size) =0;
};

#endif /* PVALUE_H_ */
