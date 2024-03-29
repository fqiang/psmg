/*
 * PNode.h
 *
 *  Created on: 8 Apr 2013
 *      Author: s0965328
 */

#ifndef PNODE_H_
#define PNODE_H_

#include "Node.h"
namespace AutoDiff {

using namespace std;
class PNode: public Node {
public:
	PNode();
	virtual ~PNode();
	void eval_function();
	void grad_reverse_0();
	void grad_reverse_1_init_adj();
	void grad_reverse_1();
	void update_adj(double& v);

#if FORWARD_ENABLED
	void hess_forward(uint len,double** ret_vec);
#endif

	//Hessian pattern detection
	void nonlinear_edges(EdgeSet&);

	//reverse Hessian-vector
	uint hess_reverse_0();
	void hess_reverse_0_get_values(uint i,double& x,double& x_bar,double& w,double& w_bar);
	void hess_reverse_1(uint i);
	void hess_reverse_1_init_x_bar(uint);
	void update_x_bar(uint, double);
	void update_w_bar(uint, double);
	void hess_reverse_1_get_xw(uint, double&,double&);
	void hess_reverse_get_x(uint,double& x);
	
	//edge pushing algrithm for full Hessian
	uint hess_reverse_full0();
	void hess_reverse_full0_get_x(uint, double&);
	void hess_reverse_full1_init_x_bar(uint);
	void hess_reverse_full1(uint,EdgeSet&);
	void hess_reverse_full1_update_x_bar(uint i,double& v);

	//utility methods
	void collect_vnodes(boost::unordered_set<Node*>& nodes,uint& total);
	void inorder_visit(int level,ostream& oss);
	TYPE getType();
	string toString(int level) = 0;

	//value looking up
	virtual double& pval() = 0;
};

} // end namespace foo

#endif /* PNODE_H_ */
