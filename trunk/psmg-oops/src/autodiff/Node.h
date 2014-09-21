/*
 * Node.h
 *
 *  Created on: 8 Apr 2013
 *      Author: s0965328
 */

#ifndef NODE_H_
#define NODE_H_

#include <boost/unordered_set.hpp>
#include "auto_diff_types.h"

using namespace std;

namespace AutoDiff {

class EdgeSet;

class Node {
public:
	Node();
	virtual ~Node();

	virtual void eval_function() = 0;
	virtual void grad_reverse_0() = 0;
	virtual void grad_reverse_1_init_adj() = 0;
	virtual void grad_reverse_1() = 0;
	virtual void update_adj(double& v) = 0;
	virtual uint hess_reverse_0() = 0;
	virtual void hess_reverse_0_init_n_in_arcs();
	virtual void hess_reverse_0_get_values(uint,double&, double&,double&, double&) = 0;
	virtual void hess_reverse_1(uint i) = 0;
	virtual void hess_reverse_1_init_x_bar(uint) = 0;
	virtual void update_x_bar(uint,double) = 0;
	virtual void update_w_bar(uint,double) = 0;
	virtual void hess_reverse_1_get_xw(uint, double&,double&) = 0;
	virtual void hess_reverse_get_x(uint,double& x)=0;
	virtual void hess_reverse_1_clear_index();
	//routing for checking non-zero structures
	virtual void collect_vnodes(boost::unordered_set<Node*>& nodes,uint& total) = 0;
	virtual void nonlinearEdges(EdgeSet&) = 0;
#if FORWARD_ENABLED
	virtual void hess_forward(uint len, double** ret_vec) = 0;
#endif

	//other utility methods
	virtual void inorder_visit( int level,ostream& oss) = 0;
	virtual string toString(int levl)  = 0;
	virtual TYPE getType() = 0;


	//! index on the tape
	uint index;
	//! number of incoming arcs
	//! n_in_arcs in root node equals 1 before evaluation and 0 after evaluation
	uint n_in_arcs;

	static uint DEFAULT_INDEX;

};

} // end namespace foo

#endif /* NODE_H_ */
