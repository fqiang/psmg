/*
 * DummyVarGen.h
 *
 *  Created on: 4 Oct 2011
 *      Author: s0965328
 */

#ifndef DUMMYVARGEN_H_
#define DUMMYVARGEN_H_

#include <string>
using namespace std;

class DummyVarGen {

private:
	DummyVarGen();
	virtual ~DummyVarGen();


public:
	static string nextDummy();
	static string dummy;
	static int num;
};

#endif /* DUMMYVARGEN_H_ */
