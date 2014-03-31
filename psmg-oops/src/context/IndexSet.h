/*
 * IndexSet.h
 *
 *  Created on: 3 May 2013
 *      Author: s0965328
 */

#ifndef INDEXSET_H_
#define INDEXSET_H_

#include <boost/unordered_map.hpp>
#include "../model/SetComp.h"
#include "CompDescr.h"
#include "Set.h"

using namespace std;

class IndexSet : public CompDescr {
public:
	static string TMP;
	string name;

	boost::unordered_map<string, Set*> dummySetMap;
	boost::unordered_map<string, SetComp*> dummyCompMap;

	IndexSet(string& name);
	virtual ~IndexSet();
//	void addSet(Set*);
	void addSet(string&,Set*,SetComp*); //TODO: one index set for now

	string toString();
	void calculateMemoryUsage(unsigned long& size);
};

#endif /* INDEXSET_H_ */
