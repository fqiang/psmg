#ifndef STATISTICS
#define STATISTICS

#include <string>

using namespace std;

class Statistics{
 public:
	static int numConsEvalLocalCall;
	static int numNZConsJacLocalCall;
	static int numConsJacLocalCall;
	static int numNZConsHessLocalCall;
	static int numConsHessLocalCall;

	static int numObjEvalLocalCall;
	static int numObjGradLocalCall;
	static int numNZObjHessLocalCall;
	static int numObjHessLocalCall;

	static void logStatistics(ostream& out);
};

#endif
