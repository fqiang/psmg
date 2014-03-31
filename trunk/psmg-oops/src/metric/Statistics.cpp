/*
* Statistics.cpp
*
*  Created on: 18 Feb 2013
*      Author: s0965328
*/

#include <iostream>
#include "../sml/GlobalVariables.h"
#include "Statistics.h"

using namespace std;

int Statistics::numNZConsJacLocalCall =0;
int Statistics::numConsEvalLocalCall =0;
int Statistics::numConsJacLocalCall =0;
int Statistics::numNZConsHessLocalCall =0;
int Statistics::numConsHessLocalCall =0;

int Statistics::numNZObjHessLocalCall =0;
int Statistics::numObjEvalLocalCall =0;
int Statistics::numObjGradLocalCall =0;
int Statistics::numObjHessLocalCall =0;


void Statistics::logStatistics(ostream& out)
{
	cout<<"["<<GlobalVariables::rank<<"/"<<GlobalVariables::size<<"] - numNZConsJacLocalCall["<<Statistics::numNZConsJacLocalCall<<"]"<<endl;
	cout<<"["<<GlobalVariables::rank<<"/"<<GlobalVariables::size<<"] - numConsEvalLocalCall["<<Statistics::numConsEvalLocalCall<<"]"<<endl;
	cout<<"["<<GlobalVariables::rank<<"/"<<GlobalVariables::size<<"] - numConsJacLocalCall["<<Statistics::numConsJacLocalCall<<"]"<<endl;
	cout<<"["<<GlobalVariables::rank<<"/"<<GlobalVariables::size<<"] - numNZConsHessLocalCall["<<Statistics::numNZConsHessLocalCall<<"]"<<endl;
	cout<<"["<<GlobalVariables::rank<<"/"<<GlobalVariables::size<<"] - numConsHessLocalCall["<<Statistics::numConsHessLocalCall<<"]"<<endl;
	cout<<"["<<GlobalVariables::rank<<"/"<<GlobalVariables::size<<"] - numNZObjHessLocalCall["<<Statistics::numNZObjHessLocalCall<<"]"<<endl;
	cout<<"["<<GlobalVariables::rank<<"/"<<GlobalVariables::size<<"] - numObjEvalLocalCall["<<Statistics::numObjEvalLocalCall<<"]"<<endl;
	cout<<"["<<GlobalVariables::rank<<"/"<<GlobalVariables::size<<"] - numObjGradLocalCall["<<Statistics::numObjGradLocalCall<<"]"<<endl;
	cout<<"["<<GlobalVariables::rank<<"/"<<GlobalVariables::size<<"] - numObjHessLocalCall["<<Statistics::numObjHessLocalCall<<"]"<<endl;

}
