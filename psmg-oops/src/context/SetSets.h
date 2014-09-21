/*
 * SetSets.h
 *
 *  Created on: 11 Jun 2014
 *      Author: s0965328
 */

#ifndef SETSETS_H_
#define SETSETS_H_

#include "Set.h"
#include "../model/SetComp.h"
#include <string.h>
#include <boost/unordered_map.hpp>

using namespace std;

class SetSets: public CompDescr {
public:

	boost::unordered_map<string,Set*> sets;
	string name;
	int dim;

	SetSets(string& name, int dim);
	SetSets(SetComp*);
	virtual ~SetSets();

	Set* getSet(const string& key);
	void addSetValue(const string& key,const string value);

	virtual string toString();
	virtual void calculateMemoryUsage(unsigned long& size);
};

#endif /* SETSETS_H_ */
