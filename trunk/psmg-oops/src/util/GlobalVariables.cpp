/*
 * GlobalVariables.cpp
 *
 *  Created on: 18 Feb 2013
 *      Author: s0965328
 */

#include <string>
#include "GlobalVariables.h"
#include "CompilerMarco.h"


int GlobalVariables::rank = 0;
int GlobalVariables::size = 0;
std::string GlobalVariables::modelfilename = "";
std::string GlobalVariables::datafilename = "";
std::string GlobalVariables::outfilename = "";
std::string GlobalVariables::amplcommand = "/home/s0965328/research/ampl-student/ampl";
int GlobalVariables::degreeParallel = 1;
bool GlobalVariables::logParseModel = true;
#ifdef DEBUG
bool GlobalVariables::debug = false;
#else
bool GlobalVariables::debug = false;
#endif
bool GlobalVariables::writeMPS = false;
bool GlobalVariables::writeMatlab = false;
bool GlobalVariables::solve = false;
std::string GlobalVariables::hostname = "";

void GlobalVariables::debugPrintAll()
{
	LOG("Rank ["<<GlobalVariables::rank<<"]");
	LOG("Size ["<<GlobalVariables::size<<"]");
	LOG("hostname ["<<GlobalVariables::hostname<<"]");
	LOG("Debug["<<GlobalVariables::debug<<"]");
	LOG("data file [" << GlobalVariables::datafilename << "]");
	LOG("model file [" << GlobalVariables::modelfilename << "]");
	LOG("out file "<< GlobalVariables::outfilename<<"]");
	LOG("Degree of Parallel["<<GlobalVariables::degreeParallel<<"]");
	LOG("AmplCommand: ["<<GlobalVariables::amplcommand<<"]");
	LOG("write matlab ["<<GlobalVariables::writeMatlab<<"]");
	LOG("write MPS ["<<GlobalVariables::writeMPS<<"]");
	LOG("logParseModel ["<<GlobalVariables::logParseModel<<"]");
	LOG("solve ["<<GlobalVariables::solve<<"]");
}

