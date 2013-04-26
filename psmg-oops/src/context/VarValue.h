/*
 * VarValue.h
 *
 *  Created on: 11 Oct 2011
 *      Author: s0965328
 */

#ifndef VARVALUE_H_
#define VARVALUE_H_

#include <string>
#include <vector>
#include <limits>

using namespace std;

const double dINF = std::numeric_limits<double>::infinity();
class VarValue {
public:
	//vector<string> indicies;
	string varKey;
	string indicies;
	int numIndicies;
	double value;
	double ub;
	double lb;


	VarValue(vector<string> ind,double lower,double upper);
	string toString();

	virtual ~VarValue();
	void calculateMemoryUsage(unsigned long& size);
};

#endif /* VARVALUE_H_ */
