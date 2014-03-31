/*
 * DumCompEntry.cpp
 *
 *  Created on: 17 Dec 2013
 *      Author: s0965328
 */
#include <cassert>
#include <sstream>
#include "DumCompEntry.h"

DumCompEntry::DumCompEntry(ModelComp* comp, int index): comp(comp), index(index){
	assert(comp->type == TSET);
}

DumCompEntry::~DumCompEntry() {
	// TODO Auto-generated destructor stub
}

string DumCompEntry::toString()
{
	ostringstream oss;
	oss<<"{"<<comp->name<<":"<<this->index<<"}";
	return oss.str();
}

