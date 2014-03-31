/*
 * DumCompEntry.h
 *
 *  Created on: 17 Dec 2013
 *      Author: s0965328
 */

#ifndef DUMCOMPENTRY_H_
#define DUMCOMPENTRY_H_

#include "ModelComp.h"

using namespace std;
class DumCompEntry {
public:
	ModelComp* comp;
	int index;

	DumCompEntry(ModelComp*, int);
	string toString();
	virtual ~DumCompEntry();
};

#endif /* DUMCOMPENTRY_H_ */
