#ifndef SETSIMPLE_H_
#define SETSIMPLE_H_

#include "Set.h"
#include <string>

class SetComp;

using namespace std;
using namespace __gnu_cxx;

class SetSimple: public Set{
	private:

	public:
		SetSimple(string& name, int dim);
		SetSimple(SetComp*);
		virtual ~SetSimple();

		virtual string toString();
		virtual void copyFromSet(Set* src);
		virtual void addSetValue(const string& key);
		virtual void calculateMemoryUsage(unsigned long& size);
	private:
};

#endif
