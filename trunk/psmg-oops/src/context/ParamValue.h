/*
 * ParamValue.h
 *
 *  Created on: 29 Sep 2011
 *      Author: s0965328
 */

#ifndef PARAMVALUE_H_
#define PARAMVALUE_H_

#include <vector>
#include <string>

using namespace std;

class ParamValue
{
	public:
		vector<string> indices; //will be the set value key
		double value;

		ParamValue(vector<string>,double);
		virtual ~ParamValue();

		string getParamIndicesKey();
		string toString();
		void calculateMemoryUsage(unsigned long& size);
};

#endif /* PARAMVALUE_H_ */
