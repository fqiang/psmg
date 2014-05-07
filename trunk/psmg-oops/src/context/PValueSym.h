/*
 * SymValue.h
 *
 *  Created on: 22 Mar 2014
 *      Author: s0965328
 */

#ifndef PVALUESYM_H_
#define PVALUESYM_H_

#include "PValue.h"

class PValueSym : public PValue {
public:
	string value;

	PValueSym(const string&);
	PValueSym(const PValueSym& other);
	virtual ~PValueSym();

	virtual PValueSym* clone();
	virtual PValueSym* plus(PValue* other);
	virtual PValueSym* accumulate(PValue* other);
	virtual PValueSym* minus(PValue* other);
	virtual PValueSym* neg();
	virtual PValueSym* times(PValue* other);
	virtual PValueSym* divid(PValue* other);
	virtual PValueSym* power(PValue* other);
	virtual PValueSym* power(double&);

	virtual bool isGT(PValue* other);
	virtual bool isEQ(PValue* other);
	virtual bool isNE(PValue* other);

	virtual string toString();
	virtual void calculateMemoryUsage(unsigned long&);

private:
	PValueSym* error();
};

#endif /* PVALUESYM_H_ */
