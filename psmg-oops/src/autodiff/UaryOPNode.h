/*
 * UaryOPNode.h
 *
 *  Created on: 6 Nov 2013
 *      Author: s0965328
 */

#ifndef UARYOPNODE_H_
#define UARYOPNODE_H_

#include "OPNode.h"

namespace AutoDiff {

class UaryOPNode: public OPNode {
public:
	static Node* createUnaryOpNode(OPCODE op, Node* left);
	virtual ~UaryOPNode();

	void eval_function();

	void grad_reverse_0();
	void grad_reverse_1();

#if FORWARD_ENABLED
	void hess_forward(uint len, double** ret_vec);
#endif

	//Hessian pattern dection
	void nonlinear_edges(EdgeSet&);

	//tape based Hessian, clear node index
	void hess_reverse_clear_index();

	//reverse Hessian-vector product
	uint hess_reverse_0();
	void hess_reverse_0_init_n_in_arcs();
	void hess_reverse_0_get_values(uint i,double& x, double& x_bar, double& w, double& w_bar);
	void hess_reverse_1(uint i);
	void hess_reverse_1_init_x_bar(uint);
	void update_x_bar(uint, double v);
	void update_w_bar(uint, double v);
	void hess_reverse_1_get_xw(uint, double&,double&);
	void hess_reverse_get_x(uint,double& x);

	//edge pushing algorithm for full Hessian computation
	uint hess_reverse_full0();
	void hess_reverse_full0_get_x(uint, double&);
	void hess_reverse_full1_init_x_bar(uint);
	void hess_reverse_full1(uint,EdgeSet&);
	void hess_reverse_full1_update_x_bar(uint i,double& v);


	//utility methods
	std::string toString(int level);
	void inorder_visit(int level,ostream& oss);
	void collect_vnodes(boost::unordered_set<Node*> & nodes,uint& total);

private:
	UaryOPNode(OPCODE op, Node* left);
	void calc_eval_function();
	void calc_grad_reverse_0();
	void hess_forward_calc0(uint& len, double* lvec,double* ret_vec);
};

} /* namespace AutoDiff */
#endif /* UARYOPNODE_H_ */
