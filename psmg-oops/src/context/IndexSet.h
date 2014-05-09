/*
 * IndexSet.h
 *
 *  Created on: 3 May 2013
 *      Author: s0965328
 */

#ifndef INDEXSET_H_
#define INDEXSET_H_

#include <boost/tuple/tuple.hpp>
#include <boost/unordered_map.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>

class Set;
class SetComp;

#include "CompDescr.h"

using namespace std;
using namespace boost::multi_index;

//1. dummy index name, 2. the corresponding set, 3, SetComp that the dummy index referred.
typedef boost::tuple<string, Set*,SetComp*> iset_tuple;


class IndexSet : public CompDescr {
public:
	static string TMP;
	static string NEWSET;
	string name;

	vector<iset_tuple> tuples;

	IndexSet(string& name);
	virtual ~IndexSet();
	void addSet(string&,Set*,SetComp*); //TODO: one index set for now

	string toString();
	void calculateMemoryUsage(unsigned long& size);
};

#endif /* INDEXSET_H_ */
