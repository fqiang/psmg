/*
 * OrderedSet.h
 *
 *  Created on: 24 Jan 2014
 *      Author: s0965328
 */

#ifndef SETORDERED_H_
#define SETORDERED_H_

#include "Set.h"
#include <string.h>
#include <boost/unordered_map.hpp>
using namespace std;

class SetOrdered : public Set {
public:

	boost::unordered_map<string,int> setOrders;
	boost::unordered_map<int,string> setKeys;

	SetOrdered(SetComp* comp);
	virtual ~SetOrdered();

	int setOrder(string& key);

	virtual void addSetValue(string& key);
	virtual void addSetValue(ostringstream& oss);
	virtual string toString();

	virtual void copyFromSet(Set* src);
	virtual bool contains(string&);
	virtual void calculateMemoryUsage(unsigned long& size);
};

#endif /* SETORDERED_H_ */
