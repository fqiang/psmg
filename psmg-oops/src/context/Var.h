/*
 * Var.h
 *
 *  Created on: 11 Oct 2011
 *      Author: s0965328
 */

#ifndef VAR_H_
#define VAR_H_

#include <ext/hash_map>
#include <string>
#include <limits>

using namespace std;
using namespace __gnu_cxx;

#define D_POS_INFI std::numeric_limits<double>::infinity()
#define D_NEG_INFI -std::numeric_limits<double>::infinity()
#define D_NaN std::numeric_limits<double>::quiet_NaN()
class Var {
public:
	string name;
	int  card;
	int numIndicies;
	double ub;
	double lb;
	vector<string> indicies;
	vector<double> values;

	Var(string name_,int card_, int numInd_, double ub_, double lb_);
	virtual ~Var();

	void addVarValue(ostringstream& oss,double val);
	void addVarValue(vector<string>& ind,double val);
	int getCard();
	string toString();
	void calculateMemoryUsage(unsigned long& size);
};

#endif /* VAR_H_ */
