/*
 * VNode.h
 *
 *  Created on: 8 Apr 2013
 *      Author: s0965328
 */

#ifndef VNODE_H_
#define VNODE_H_

#include "ActNode.h"

namespace AutoDiff {
class VNode: public ActNode {
public:
//	VNode(double v=NaN_Double);
	VNode(uint idx);
	virtual ~VNode();
	void collect_vnodes(boost::unordered_set<Node*>& nodes,uint& total);
	void eval_function();
	void grad_reverse_0();
	void grad_reverse_1();
	uint hess_reverse_0();
	void hess_reverse_0_get_values(uint i,double& x, double& x_bar, double& w, double& w_bar);
	void hess_reverse_1(uint i);
	void hess_reverse_1_init_x_bar(uint);
	void update_x_bar(uint,double);
	void update_w_bar(uint,double);
	void hess_reverse_1_get_xw(uint,double&,double&);
	void hess_reverse_get_x(uint,double& x);


	uint hess_reverse_full0();
	void hess_reverse_full0_get_x(uint, double&);
	void hess_reverse_full1_init_x_bar(uint);
	void hess_reverse_full1(uint,EdgeSet&);
	void hess_reverse_full1_update_x_bar(uint i,double& v);

	void nonlinearEdges(EdgeSet&);

	void inorder_visit(int level,ostream& oss);
	string toString(int level);
	TYPE getType();

	double& val();

#if FORWARD_ENABLED
	void hess_forward(uint nvar, double** ret_vec);
	//! used for only forward hessian
	//! the id has to be assigned starting from 0
	int id;
	static int DEFAULT_ID;
#endif
//	double val;
	uint idx;  //idx for value look up.
	double u;


};

} // end namespace foo

#endif /* VNODE_H_ */
