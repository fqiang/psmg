/*
 * Var.h
 *
 *  Created on: 25 Mar 2014
 *      Author: s0965328
 */

#ifndef VAR_H_
#define VAR_H_


#include <boost/multi_index_container.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>

#include "CompDescr.h"

#include "VarSingle.h"

using namespace std;
using namespace boost;
using namespace boost::multi_index;

typedef multi_index_container<VarSingle*,
		indexed_by<
			random_access<>,
			hashed_unique<  member<VarSingle, string, &VarSingle::indicies> >
		>
> var_multi_map;

typedef var_multi_map::nth_index<0>::type var_multi_map_by_order;
typedef var_multi_map::nth_index<1>::type var_multi_map_by_indicies;

class Var : public CompDescr {
public:
	Var(string&);
	virtual ~Var();

	string name;
	var_multi_map varMultiMap;

	string toString();
	void calculateMemoryUsage(unsigned long& size);
};

#endif /* VAR_H_ */
