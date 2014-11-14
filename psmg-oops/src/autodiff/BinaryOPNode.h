/*
 * BinaryOPNode.h
 *
 *  Created on: 6 Nov 2013
 *      Author: s0965328
 */

#ifndef BINARYOPNODE_H_
#define BINARYOPNODE_H_

#include "OPNode.h"

namespace AutoDiff {

class EdgeSet;

class BinaryOPNode: public OPNode {
public:

	static Node* createBinaryOpNode(OPCODE op, Node* left, Node* right);
	virtual ~BinaryOPNode();

	void eval_function();

	void grad_reverse_0();
	void grad_reverse_1();

#if FORWARD_ENABLED
	void hess_forward(uint len, double** ret_vec);
#endif

	//tape based Hessian, clear node index
	void hess_reverse_clear_index();

	//Hessian pattern detection
	void nonlinear_edges(EdgeSet& a);

	//reverse Hessian-vector
	uint hess_reverse_0();
	void hess_reverse_0_init_n_in_arcs();
	void hess_reverse_0_get_values(uint,double&, double&, double&, double&);
	void hess_reverse_1(uint i);
	void hess_reverse_1_init_x_bar(uint);
	void update_x_bar(uint,double);
	void update_w_bar(uint,double);
	void hess_reverse_1_get_xw(uint, double&,double&);
	void hess_reverse_get_x(uint,double& x);

	//reverse edge pushing for full Hessian
	uint hess_reverse_full0();
	void hess_reverse_full0_get_x(uint, double&);
	void hess_reverse_full1_init_x_bar(uint);
	void hess_reverse_full1(uint,EdgeSet&);
	void hess_reverse_full1_update_x_bar(uint i,double& v);

	//utility methods
	void collect_vnodes(boost::unordered_set<Node*>& nodes,uint& total);
	void inorder_visit(int level,ostream& oss);
	string toString(int level);

	Node* right;

private:
	BinaryOPNode(OPCODE op, Node* left, Node* right);
	void calc_eval_function();
	void calc_grad_reverse_0();
	void hess_forward_calc0(uint& len, double* lvec, double* rvec,double* ret_vec);
};

} /* namespace AutoDiff */
#endif /* BINARYOPNODE_H_ */
