/*
 * SingleSymParam.h
 *
 *  Created on: 31 Jan 2014
 *      Author: s0965328
 */

#ifndef PARAMSINGLE_H_
#define PARAMSINGLE_H_

#include "Param.h"
class PValue;

class ParamSingle : public Param {
public:
	PValue* value;

	ParamSingle(ParamComp*);
	virtual ~ParamSingle();

	void setParamValue(PValue*);
	string toString();
	void calculateMemoryUsage(unsigned long& size);
};

#endif /* PARAMSINGLE_H_ */
