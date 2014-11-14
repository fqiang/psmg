/*
 * Node.h
 *
 *  Created on: 8 Apr 2013
 *      Author: s0965328
 */

#ifndef NODE_H_
#define NODE_H_

#include <boost/unordered_set.hpp>
#include "autodiff_types.h"

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


#if FORWARD_ENABLED
	virtual void hess_forward(uint len, double** ret_vec) = 0;
#endif

	//Hessian pattern dectection
	virtual void nonlinear_edges(EdgeSet&) = 0;

	//reset node index on tape for Hessian computation
	virtual void hess_reverse_clear_index();

	//reverse Hessian-vector computation
	virtual uint hess_reverse_0() = 0;
	virtual void hess_reverse_0_init_n_in_arcs();
	virtual void hess_reverse_0_get_values(uint,double&, double&,double&, double&) = 0;
	virtual void hess_reverse_1(uint i) = 0;
	virtual void hess_reverse_1_init_x_bar(uint) = 0;
	virtual void update_x_bar(uint,double) = 0;
	virtual void update_w_bar(uint,double) = 0;
	virtual void hess_reverse_1_get_xw(uint, double&,double&) = 0;
	virtual void hess_reverse_get_x(uint,double& x)=0;

	//edge pushing algorithm for full Hessian
	virtual uint hess_reverse_full0() = 0;
	virtual void hess_reverse_full0_get_x(uint, double&) = 0;
	virtual void hess_reverse_full1_init_x_bar(uint) = 0;
	virtual void hess_reverse_full1(uint,EdgeSet&) = 0;
	virtual void hess_reverse_full1_update_x_bar(uint i,double& v) = 0;


	//other utility methods
	virtual void inorder_visit( int level,ostream& oss) = 0;
	virtual void collect_vnodes(boost::unordered_set<Node*>& nodes,uint& total) = 0;
	virtual string toString(int levl)  = 0;
	virtual TYPE getType() = 0;

	//debug use
	virtual void print();
	virtual void ptree();


	//! index on the tape
	uint index;
	//! number of incoming arcs
	//! n_in_arcs in root node equals 1 before evaluation and 0 after evaluation
	uint n_in_arcs;

	static uint DEFAULT_INDEX;

};

} // end namespace foo

#endif /* NODE_H_ */
