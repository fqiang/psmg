/*
 * DummyVariableGenerator.h
 *
 *  Created on: 4 Oct 2011
 *      Author: s0965328
 */

#ifndef DUMMYVARIABLEGENERATOR_H_
#define DUMMYVARIABLEGENERATOR_H_

#include <string>
using namespace std;

class DummyVariableGenerator {

private:
	DummyVariableGenerator();
	virtual ~DummyVariableGenerator();


public:
	static string nextDummy();
	static string dummy;
	static int num;
};

#endif /* DUMMYVARIABLEGENERATOR_H_ */
