/*
 * Var.h
 *
 *  Created on: 11 Oct 2011
 *      Author: s0965328
 */

#ifndef VAR_H_
#define VAR_H_

#include "VarValue.h"
#include <ext/hash_map>
#include <string>

using namespace std;
using namespace __gnu_cxx;

class Var {
public:
	string name;
	int  card;
	int numIndicies;
	vector<VarValue*> varList;


	Var(string name_);
	virtual ~Var();

	void addVarValue(VarValue* val);
	int getCard();
	string toString();
	void calculateMemoryUsage(unsigned long& size);
};

#endif /* VAR_H_ */
