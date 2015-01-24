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

int Stat::numObjGrad_NLP_LocalCall = 0;

int Stat::numConsFevalLocalCall = 0;
int Stat::numObjFevalLocalCall = 0;

int Stat::numNZJacA_LP_QP_Call = 0;
int Stat::numJacA_LP_QP_Call = 0;

int Stat::numNZHessQ_QP_Call = 0;
int Stat::numHessQ_QP_Call = 0;

//int Stat::numNZObjHessLocalCall =0;
//int Stat::numObjHessLocalCall =0;

int Stat::numGradObj_LP_QP_Call = 0;

int Stat::nnzJacA_LP_QP = 0;
int Stat::nnzHessQ_QP = 0;
int Stat::nnzJacA_NLP = 0;
int Stat::nnzLagHess_NLP =0;
void Stat::logStatistics(ostream& out)
{
	cout<<"["<<GV(rank)<<"/"<<GV(size)<<"] - numConsFevalLocalCall["<<Stat::numConsFevalLocalCall<<"]"<<endl;
	cout<<"["<<GV(rank)<<"/"<<GV(size)<<"] - numObjFevalLocalCall["<<Stat::numObjFevalLocalCall<<"]"<<endl;

	cout<<"["<<GV(rank)<<"/"<<GV(size)<<"] - numNZConsJac_NLP_LocalCall["<<Stat::numNZConsJac_NLP_LocalCall<<"]"<<endl;
	cout<<"["<<GV(rank)<<"/"<<GV(size)<<"] - numConsJac_NLP_LocalCall["<<Stat::numConsJac_NLP_LocalCall<<"]"<<endl;
	cout<<"["<<GV(rank)<<"/"<<GV(size)<<"] - numNZLagHess_NLP_LocalCall["<<Stat::numNZLagHess_NLP_LocalCall<<"]"<<endl;
	cout<<"["<<GV(rank)<<"/"<<GV(size)<<"] - numLagHess_NLP_LocalCall["<<Stat::numLagHess_NLP_LocalCall<<"]"<<endl;
	cout<<"["<<GV(rank)<<"/"<<GV(size)<<"] - numObjGrad_NLP_LocalCall["<<Stat::numObjGrad_NLP_LocalCall<<"]"<<endl;

//	cout<<"["<<GV(rank)<<"/"<<GV(size)<<"] - numNZObjHessLocalCall["<<Stat::numNZObjHessLocalCall<<"]"<<endl;
//	cout<<"["<<GV(rank)<<"/"<<GV(size)<<"] - numObjHessLocalCall["<<Stat::numObjHessLocalCall<<"]"<<endl;

	cout<<"["<<GV(rank)<<"/"<<GV(size)<<"] - numNZJacA_LP_QPCall["<<Stat::numNZJacA_LP_QP_Call<<"]"<<endl;
	cout<<"["<<GV(rank)<<"/"<<GV(size)<<"] - numJacA_LP_QPCall["<<Stat::numJacA_LP_QP_Call<<"]"<<endl;

	cout<<"["<<GV(rank)<<"/"<<GV(size)<<"] - numNZHessQ_QPCall["<<Stat::numNZHessQ_QP_Call<<"]"<<endl;
	cout<<"["<<GV(rank)<<"/"<<GV(size)<<"] - numHessQ_QPCall["<<Stat::numHessQ_QP_Call<<"]"<<endl;

	cout<<"["<<GV(rank)<<"/"<<GV(size)<<"] - numGradObj_LP_QP_Call["<<Stat::numGradObj_LP_QP_Call<<"]"<<endl;



	cout<<"["<<GV(rank)<<"/"<<GV(size)<<"] - nnzJacA_LP_QP["<<Stat::nnzJacA_LP_QP<<"]"<<endl;
	cout<<"["<<GV(rank)<<"/"<<GV(size)<<"] - nnzHessQ_QP["<<Stat::nnzHessQ_QP<<"]"<<endl;
	cout<<"["<<GV(rank)<<"/"<<GV(size)<<"] - nnzJacA_NLP["<<Stat::nnzJacA_NLP<<"]"<<endl;
	cout<<"["<<GV(rank)<<"/"<<GV(size)<<"] - nnzLagHess_NLP["<<Stat::nnzLagHess_NLP<<"]"<<endl;
}
