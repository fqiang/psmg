/*
 * DummyVarGen.cpp
 *
 *  Created on: 4 Oct 2011
 *      Author: s0965328
 */

#include "DummyVarGen.h"
#include <sstream>
#include <string>

using namespace std;

string DummyVarGen::dummy = "dummy_";
int DummyVarGen::num = 0;

DummyVarGen::DummyVarGen() {

}

DummyVarGen::~DummyVarGen() {
}

string DummyVarGen::nextDummy()
{
	ostringstream oss;
	oss<<DummyVarGen::dummy<<DummyVarGen::num;
	num++;
	return oss.str();
}

