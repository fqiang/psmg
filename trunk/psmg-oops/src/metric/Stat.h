#ifndef STATISTICS
#define STATISTICS

#include <string>

using namespace std;

class Stat{
 public:
	static int numNZJacA_LP_QP_Call;
	static int numJacA_LP_QP_Call;


	static int numNZHessQ_QP_Call;
	static int numHessQ_QP_Call;

	static int numNZConsJac_NLP_LocalCall;
	static int numConsJac_NLP_LocalCall;
	static int numNZLagHess_NLP_LocalCall;
	static int numLagHess_NLP_LocalCall;

	static int numNZObjHessLocalCall;
	static int numObjHessLocalCall;

	static int numGradObjCCall;
	static int numConsFevalLocalCall;
	static int numObjFevalCall;

	static void logStatistics(ostream& out);
};

#endif
