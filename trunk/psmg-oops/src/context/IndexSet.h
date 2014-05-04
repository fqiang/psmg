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

#include "../model/SetComp.h"
#include "CompDescr.h"
#include "Set.h"

using namespace std;
using namespace boost::multi_index;

typedef boost::tuple<string, Set*,SetComp*> iset_tuple;
//typedef multi_index_container<iset_tuple,
//		indexed_by<
//			random_access<>,
//			hashed_unique<  member<iset_tuple, string, &iset_tuple::get<0>() > >
//		>
//> iset_multi_map;
//
//typedef iset_multi_map::nth_index<0>::type iset_multi_map_by_order;
//typedef iset_multi_map::nth_index<1>::type iset_multi_map_by_dummy;

class IndexSet : public CompDescr {
public:
	static string TMP;
	static string NEWSET;
	string name;

	vector<iset_tuple> tuples;

//	boost::unordered_map<string, Set* > dummySetMap;
//	boost::unordered_map<string, SetComp*> dummyCompMap;

	IndexSet(string& name);
	virtual ~IndexSet();
	void addSet(string&,Set*,SetComp*); //TODO: one index set for now

	string toString();
	void calculateMemoryUsage(unsigned long& size);
};

#endif /* INDEXSET_H_ */
