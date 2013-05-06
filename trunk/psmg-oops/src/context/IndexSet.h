/*
 * IndexSet.h
 *
 *  Created on: 3 May 2013
 *      Author: s0965328
 */

#ifndef INDEXSET_H_
#define INDEXSET_H_

#include "CompDescr.h"
#include "Set.h"

class IndexSet : public CompDescr {
public:
	string name;
	vector<ModelComp*> comps;
	vector<string> dummyVarNames;
	vector< vector<string> > setIndicies;

	IndexSet(string name_);
	virtual ~IndexSet();
	string toString() const;
	void addSet(Set*);
	void calculateMemoryUsage(unsigned long& size);
};

#endif /* INDEXSET_H_ */
