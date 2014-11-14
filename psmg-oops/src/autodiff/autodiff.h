/*
 * autodiff.h
 *
 *  Created on: 16 Apr 2013
 *      Author: s0965328
 */

#ifndef AUTODIFF_H_
#define AUTODIFF_H_
#include "autodiff_types.h"
#include "../util/util.h"
#include "Node.h"
#include "VNode.h"
#include "OPNode.h"
#include "BinaryOPNode.h"
#include "UaryOPNode.h"
#include "PNode.h"
#include "PIndex.h"
#include "PVal.h"
#include "ActNode.h"
#include "EdgeSet.h"


/*
 * + Function and Gradient Evaluation
 * The tapeless implementation for function and derivative evaluation
 * Advantage for tapeless:
 * 			Low memory usage
 * 			Function evaluation use one stack
 * 			Gradient evaluation use two stack.
 * Disadvantage for tapeless:
 * 			Inefficient if the expression tree have repeated nodes.
 * 			for example:
 * 						 root
 * 						 /  \
 * 						*    *
 * 					   / \	/ \
 * 				      x1 x1 x1 x1
 * 				      Tapeless implementation will go through all the edges.
 * 				      ie. adjoint of x will be updated 4 times for the correct
 * 				      gradient of x1.While the tape implemenation can discovery this
 * 				      dependence and update adjoint of x1 just twice. The computational
 * 				      graph (DAG) for a taped implemenation will be look like bellow.
 * 				       root
 * 				        /\
 * 				        *
 * 				        /\
 * 				        x1
 *
 * + Forward Hessian Evaluation:
 * This is an inefficient implementation of the forward Hessian method. It will evaluate the diagonal
 * and upper triangular of the Hessian. The gradient is also evaluation in the same routine. The result
 * will be returned in an array.
 * To use this method, one have to provide a len parameter. len = (nvar+3)*nvar/2 where nvar is the number
 * of independent variables. ie. x_1 x_2 ... x_nvar. And the varaible id need to be a consequent integer start
 * with 0.
 * ret_vec will contains len number of doubles. Where the first nvar elements is the gradient vector,
 * and the rest of (nvar+1)*nvar/2 elements are the upper/lower plus the diagonal part of the Hessian matrix
 * in row format.
 * This algorithm is inefficient, because at each nodes, it didn't check the dependency of the independent
 * variables up to the current node. (or it is hard to do so for this setup). Therefore, it computes a full
 * for loops over each independent variable (ie. assume they are all dependent), for those independent
 * variables that are not dependent at the current node, zero will be produced by computation.
 * By default the forward mode hessian routing is disabled. To enable the forward hessian interface, the
 * compiler marco FORWARD_ENABLED need to be set equal to 1 in autodiff_types.h
 *
 * + Reverse Hessian*Vector Evaluation:
 * Simple, building a tape in the forward pass, and a reverse pass will evaluate the Hessian*vector. The implemenation
 * also discovery the repeated subexpression and use one piece of memory on the tape for the same subexpression. This
 * allow efficient evaluation, because the repeated subexpression only evaluate once in the forward and reverse pass.
 * This algorithm can be called n times to compute a full Hessian, where n equals the number of independent
 * variables.
 * */

namespace AutoDiff{

	//node creation methods
	extern VNode* create_var_node(uint& idx);
	extern PVal* create_param_node(double& val);
	extern PIndex* create_param_node(uint idx);
	extern Node* create_uary_op_node(OPCODE code, Node* left);
	extern Node* create_binary_op_node(OPCODE code, Node* left,Node* right);

	//for computing nonlinear interaction of variables in the computation graph by root
	extern void nonlinear_edges(Node* root, EdgeSet& edges);

	//single constraint version
	extern double eval_function(Node* root);
	extern uint nzGrad(Node* root);
	extern double grad_reverse(Node* root,vector<Node*>& nodes, vector<double>& grad);
	extern uint nzHess(EdgeSet&);
	extern double hess_reverse(Node* root, vector<Node*>& nodes, vector<double>& dhess);

	//multiple constraints version
	extern uint nzGrad(Node* root, boost::unordered_set<Node*>& vnodes);
	extern uint nzGrad(Node* root, std::vector<Node*>& vlist, col_compress_imatrix_row& rgrad);
	extern double grad_reverse(Node* root, vector<Node*>& nodes, col_compress_matrix_row& rgrad);
	extern uint nzHess(EdgeSet&,boost::unordered_set<Node*>& set1, boost::unordered_set<Node*>& set2);
	extern uint nzHess(EdgeSet&,boost::unordered_map<Node*,uint>& colvMap, boost::unordered_map<Node*,uint>& rowvMap,col_compress_imatrix& m);
	extern double hess_reverse(Node* root, vector<Node*>& nodes, col_compress_matrix& hess);
	extern double hess_reverse(Node* root, vector<Node*>& nodes, col_compress_matrix_col& hess);
	extern double hess_reverse_ep(Node* root, boost::unordered_map<Node*,uint>& colvMap, boost::unordered_map<Node*,uint>& rowvMap, col_compress_matrix& hess);

#if FORWARD_ENDABLED
	//forward methods
	extern void hess_forward(Node* root, uint nvar, double** hess_mat);
#endif

	//utiliy methods
	extern uint num_total_nodes(Node*);
	extern void autodiff_setup();
	extern void autodiff_cleanup();

	//debug use
	extern string texpr(Node* root);
	extern void ptree(Node* root);
};

#endif /* AUTODIFF_H_ */
