/*
* Statistics.cpp
*
*  Created on: 18 Feb 2013
*      Author: s0965328
*/

#include <iostream>
#include "../sml/Config.h"
#include "Stat.h"

using namespace std;

int Stat::numNZConsJac_NLP_LocalCall =0;
int Stat::numConsJac_NLP_LocalCall =0;
int Stat::numNZLagHess_NLP_LocalCall =0;
int Stat::numLagHess_NLP_LocalCall =0;

int Stat::numNZJacA_LP_QP_Call = 0;
int Stat::numJacA_LP_QP_Call = 0;

int Stat::numNZHessQ_QP_Call = 0;
int Stat::numHessQ_QP_Call = 0;

int Stat::numNZObjHessLocalCall =0;
int Stat::numObjHessLocalCall =0;

int Stat::numGradObjCCall = 0;
int Stat::numConsFevalLocalCall = 0;
int Stat::numObjFevalCall = 0;

void Stat::logStatistics(ostream& out)
{
	cout<<"["<<GV(rank)<<"/"<<GV(size)<<"] - numNZConsJac_NLP_LocalCall["<<Stat::numNZConsJac_NLP_LocalCall<<"]"<<endl;
	cout<<"["<<GV(rank)<<"/"<<GV(size)<<"] - numConsJac_NLP_LocalCall["<<Stat::numConsJac_NLP_LocalCall<<"]"<<endl;
	cout<<"["<<GV(rank)<<"/"<<GV(size)<<"] - numNZLagHess_NLP_LocalCall["<<Stat::numNZLagHess_NLP_LocalCall<<"]"<<endl;
	cout<<"["<<GV(rank)<<"/"<<GV(size)<<"] - numLagHess_NLP_LocalCall["<<Stat::numLagHess_NLP_LocalCall<<"]"<<endl;

	cout<<"["<<GV(rank)<<"/"<<GV(size)<<"] - numNZObjHessLocalCall["<<Stat::numNZObjHessLocalCall<<"]"<<endl;
	cout<<"["<<GV(rank)<<"/"<<GV(size)<<"] - numObjHessLocalCall["<<Stat::numObjHessLocalCall<<"]"<<endl;

	cout<<"["<<GV(rank)<<"/"<<GV(size)<<"] - numNZJacA_LP_QPCall["<<Stat::numNZJacA_LP_QP_Call<<"]"<<endl;
	cout<<"["<<GV(rank)<<"/"<<GV(size)<<"] - numJacA_LP_QPCall["<<Stat::numJacA_LP_QP_Call<<"]"<<endl;

	cout<<"["<<GV(rank)<<"/"<<GV(size)<<"] - numNZHessQ_QPCall["<<Stat::numNZHessQ_QP_Call<<"]"<<endl;
	cout<<"["<<GV(rank)<<"/"<<GV(size)<<"] - numHessQ_QPCall["<<Stat::numHessQ_QP_Call<<"]"<<endl;

	cout<<"["<<GV(rank)<<"/"<<GV(size)<<"] - numGradObjCall["<<Stat::numGradObjCCall<<"]"<<endl;
	cout<<"["<<GV(rank)<<"/"<<GV(size)<<"] - numConsFevalLocalCall["<<Stat::numConsFevalLocalCall<<"]"<<endl;
	cout<<"["<<GV(rank)<<"/"<<GV(size)<<"] - numObjFevalCall["<<Stat::numObjFevalCall<<"]"<<endl;

}
