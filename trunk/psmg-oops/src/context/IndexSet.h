/*
 * IndexSet.h
 *
 *  Created on: 3 May 2013
 *      Author: s0965328
 */

#ifndef INDEXSET_H_
#define INDEXSET_H_

#include <boost/foreach.hpp>
#include <vector>

class Set;
class SetComp;

#include "CompDescr.h"

using namespace std;

//1. dummy index name, 2. the corresponding set, 3, SetComp that the dummy index referred.
typedef struct iset_tuple{
	string dummyVar;
	Set* set;
	SetComp* setcomp;
	iset_tuple(string a, Set* s, SetComp* sp):dummyVar(a),set(s),setcomp(sp){
	}
} iset_tuple;


class IndexSet : public CompDescr {
public:
	static string TMP;
	static string NEWSET;
	string name;

	std::vector<iset_tuple> tuples;

	IndexSet(string& name);
	virtual ~IndexSet();
	void addSet(string&,Set*,SetComp*); //TODO: one index set for now

	string toString();
	void calculateMemoryUsage(unsigned long& size);
};

#endif /* INDEXSET_H_ */
