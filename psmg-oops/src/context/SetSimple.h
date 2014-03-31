#ifndef SETSIMPLE_H_
#define SETSIMPLE_H_

#include "Set.h"
#include "../model/SetComp.h"
#include <string>

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
		virtual void addSetValue(string& key);
		virtual void addSetValue(ostringstream& oss);
		virtual bool contains(string&);
		virtual void calculateMemoryUsage(unsigned long& size);
	private:
};

#endif
