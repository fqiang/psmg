/*
 * Set.h
 *
 *  Created on: 29 Jan 2014
 *      Author: s0965328
 */

#ifndef SET_H_
#define SET_H_

#include "CompDescr.h"
#include "../model/SetComp.h"
#include "../util/global_util_functions.h"

class Set : public CompDescr {
public:
	static string TMP;
	static void deleteSet(Set* set);
	static Set* createSet(SetComp*);

	int dim; //same as in ModelComp
	string name;
	int card;

	vector<string> setValues_data_order;
	Set(SetComp*);
	Set(string& name, int dim);
	virtual ~Set();


	virtual void setDiff(Set* sub, Set* rval);
	virtual void setCross(Set* other, Set* rval);
	virtual void setUnion(Set* other, Set* rval);
	virtual void calculateMemoryUsage(unsigned long& size);
	virtual bool contains(const string&);

	virtual void copyFromSet(Set* src) = 0;
	virtual void addSetValue(const string& key) = 0;
	virtual string toString() = 0;
};

#endif /* SET_H_ */
