/*
 * SymParam.h
 *
 *  Created on: 16 Dec 2013
 *      Author: s0965328
 */

#ifndef PARAMMULT_H_
#define PARAMMULT_H_

#include <string>
#include <boost/unordered_map.hpp>
#include "Param.h"
#include "PValue.h"

using namespace std;
using namespace __gnu_cxx;

class ParamMult : public Param {
public:
	boost::unordered_map<string,PValue*> paramValues;

	ParamMult(ParamComp* comp);
	~ParamMult();

	void addParamValue(const string& indiciesKey, PValue*);
	PValue* findParamValue(const string& indiciesKey);
	string toString();
	void calculateMemoryUsage(unsigned long& size);
};

#endif /* PARAMMULT_H_ */
