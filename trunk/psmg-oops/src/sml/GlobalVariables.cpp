/*
 * GlobalVariables.cpp
 *
 *  Created on: 18 Feb 2013
 *      Author: s0965328
 */

#include <string>
#include "GlobalVariables.h"
#include "../util/CompilerMarco.h"

using namespace std;

int GlobalVariables::rank = 0;
int GlobalVariables::size = 0;
string GlobalVariables::modelfilename = "";
string GlobalVariables::datafilename = "";
string GlobalVariables::outfilename = "";
string GlobalVariables::amplcommand = "/home/s0965328/research/ampl-student/ampl";
bool GlobalVariables::logParseModel = true;
#ifdef DEBUG
bool GlobalVariables::debug = true;
#else
bool GlobalVariables::debug = false;
#endif
bool GlobalVariables::writeMPS = false;
bool GlobalVariables::writeMatlab = false;
bool GlobalVariables::solve = false;
string GlobalVariables::hostname = "";
string GlobalVariables::executable = "";
string GlobalVariables::logdir = "./log/";

void GlobalVariables::printAll()
{
	LOG("Rank ["<<GlobalVariables::rank<<"]");
	LOG("Size ["<<GlobalVariables::size<<"]");
	LOG("hostname ["<<GlobalVariables::hostname<<"]");
	LOG("Debug["<<GlobalVariables::debug<<"]");
	LOG("data file [" << GlobalVariables::datafilename << "]");
	LOG("model file [" << GlobalVariables::modelfilename << "]");
	LOG("out file "<< GlobalVariables::outfilename<<"]");
	LOG("AmplCommand: ["<<GlobalVariables::amplcommand<<"]");
	LOG("write matlab ["<<GlobalVariables::writeMatlab<<"]");
	LOG("write MPS ["<<GlobalVariables::writeMPS<<"]");
	LOG("logParseModel ["<<GlobalVariables::logParseModel<<"]");
	LOG("solve ["<<GlobalVariables::solve<<"]");
	LOG("logdir ["<<GlobalVariables::logdir<<"]");
}

