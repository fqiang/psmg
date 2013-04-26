/*
 * DummyVariableGenerator.cpp
 *
 *  Created on: 4 Oct 2011
 *      Author: s0965328
 */

#include "DummyVariableGenerator.h"
#include <sstream>
#include <string>

using namespace std;

string DummyVariableGenerator::dummy = "dummy_";
int DummyVariableGenerator::num = 0;

DummyVariableGenerator::DummyVariableGenerator() {

}

DummyVariableGenerator::~DummyVariableGenerator() {
}

string DummyVariableGenerator::nextDummy()
{
	ostringstream oss;
	oss<<DummyVariableGenerator::dummy<<DummyVariableGenerator::num;
	num++;
	return oss.str();
}

