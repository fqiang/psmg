/*
 * ValueValue.h
 *
 *  Created on: 22 Mar 2014
 *      Author: s0965328
 */

#ifndef PVALUEVALUE_H_
#define PVALUEVALUE_H_

#include "PValue.h"

class PValueValue : public PValue {
public:
	double value;

	PValueValue(double);
	PValueValue(const PValueValue&);
	virtual ~PValueValue();

	virtual PValueValue* clone();
	virtual PValueValue* plus(PValue* other);
	virtual PValueValue* accumulate(PValue* other);
	virtual PValueValue* neg();
	virtual PValueValue* minus(PValue* other);
	virtual PValueValue* times(PValue* other);
	virtual PValueValue* divid(PValue* other);
	virtual PValueValue* power(PValue* other);
	virtual PValueValue* power(double&);

	virtual bool isGT(PValue* other);
	virtual bool isEQ(PValue* other);
	virtual bool isNE(PValue* other);

	virtual string toString();
	virtual void calculateMemoryUsage(unsigned long&);
};

#endif /* PVALUEVALUE_H_ */
