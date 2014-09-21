//============================================================================
// Name        : autodiff.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <sstream>
#include <numeric>
#include <boost/foreach.hpp>
#include "autodiff.h"
#include "Stack.h"
#include "Tape.h"
#include "BinaryOPNode.h"
#include "UaryOPNode.h"
#include "autodiff_assert.h"

using namespace std;

namespace AutoDiff
{

#if FORWARD_ENABLED

uint num_var = 0;

void hess_forward(Node* root, uint nvar, double** hess_mat)
{
	assert(nvar == num_var);
	uint len = (nvar+3)*nvar/2;
	root->hess_forward(len,hess_mat);
}

#endif


PNode* create_param_node(uint idx){
	return new PIndex(idx);
}

PNode* create_param_node(double& val){
	return new PVal(val);
}
VNode* create_var_node(uint idx)
{
	return new VNode(idx);
}
OPNode* create_binary_op_node(OPCODE code, Node* left, Node* right)
{
	return BinaryOPNode::createBinaryOpNode(code,left,right);
}
OPNode* create_uary_op_node(OPCODE code, Node* left)
{
	return UaryOPNode::createUnaryOpNode(code,left);
}
double eval_function(Node* root)
{
	assert(SD->size()==0);
	assert(SV->size()==0);
	root->eval_function();
	assert(SV->size()==1);
	double val = SV->pop_back();
	return val;
}

double grad_reverse(Node* root,vector<Node*>& vnodes, vector<double>& grad)
{
	grad.clear();
	BOOST_FOREACH(Node* node, vnodes)
	{
		assert(node->getType()==VNode_Type);
		static_cast<VNode*>(node)->adj = NaN_Double;
	}

	assert(SD->size()==0);
	root->grad_reverse_0();
	assert(SV->size()==1);
	root->grad_reverse_1_init_adj();	
	root->grad_reverse_1();
	assert(SD->size()==0);
	double val = SV->pop_back();
	assert(SV->size()==0);
	//int i=0;
	BOOST_FOREACH(Node* node, vnodes)
	{
		assert(node->getType()==VNode_Type);
		grad.push_back(static_cast<VNode*>(node)->adj);
		static_cast<VNode*>(node)->adj = NaN_Double;
	}
	assert(grad.size()==vnodes.size());
	//all nodes are VNode and adj == NaN_Double -- this reset adj for this expression tree by root
	return val;
}

double grad_reverse(Node* root, vector<Node*>& vnodes, col_compress_matrix_row& rgrad)
{
	BOOST_FOREACH(Node* node, vnodes)
	{
		assert(node->getType()==VNode_Type);
		static_cast<VNode*>(node)->adj = NaN_Double;
	}
	assert(SD->size()==0);
	root->grad_reverse_0();
	assert(SV->size()==1);
	root->grad_reverse_1_init_adj();
	root->grad_reverse_1();
	assert(SD->size()==0);
	double val = SV->pop_back();
	assert(SV->size()==0);
	uint i =0;  //var index
	uint n = 0; //non zero
	uint z = 0; //zero
	BOOST_FOREACH(Node* node, vnodes)
	{
		assert((node)->getType()==VNode_Type);
		double diff = static_cast<VNode*>(node)->adj;
		if(!isnan(diff)){
			n++;
			rgrad(i) = diff;
			static_cast<VNode*>(node)->adj = NaN_Double;
			if(diff == 0)  z++;
		}
		i++;
	}
	assert(i == vnodes.size());
	WARN(" grad_reverse -  this expression has -- ["<<n<<"] non zero grad - in which "<<z<<" zero value");
	//all nodes are VNode and adj == NaN_Double -- this reset adj for this expression tree by root
	return val;
}

double hess_reverse(Node* root,vector<Node*>& vnodes,vector<double>& chess)
{
	TT->clear();
	II->clear();
	assert(TT->empty());
	assert(II->empty());
	assert(TT->index==0);
	assert(II->index==0);
	chess.clear();

//	for(vector<Node*>::iterator it=nodes.begin();it!=nodes.end();it++)
//	{
//		assert((*it)->getType()==VNode_Type);
//		(*it)->index = 0;
//	} //this work complete in hess-reverse_0_init_index

	assert(root->n_in_arcs == 0);
	root->hess_reverse_0_init_n_in_arcs();
	assert(root->n_in_arcs == 1);
	root->hess_reverse_0();
	double val = NaN_Double;
	root->hess_reverse_get_x(TT->index,val);
//	cout<<TT->toString();
//	cout<<endl;
//	cout<<II->toString();
//	cout<<"======================================= hess_reverse_0"<<endl;
	root->hess_reverse_1_init_x_bar(TT->index);
	assert(root->n_in_arcs == 1);
	root->hess_reverse_1(TT->index);
	assert(root->n_in_arcs == 0);
	assert(II->index==0);
//	cout<<TT->toString();
//	cout<<endl;
//	cout<<II->toString();
//	cout<<"======================================= hess_reverse_1"<<endl;

	for(vector<Node*>::iterator it=vnodes.begin();it!=vnodes.end();it++)
	{
		assert((*it)->getType()==VNode_Type);
		chess.push_back(TT->get((*it)->index-1));
	}

	TT->clear();
	II->clear();
	root->hess_reverse_1_clear_index();
	return val;
}

double hess_reverse(Node* root,vector<Node*>& vnodes,col_compress_matrix_col& chess)
{
	TT->clear();
	II->clear();
	assert(TT->empty());
	assert(II->empty());
	assert(TT->index==0);
	assert(II->index==0);

//	for(vector<Node*>::iterator it=nodes.begin();it!=nodes.end();it++)
//	{
//		assert((*it)->getType()==VNode_Type);
//		(*it)->index = 0;
//	} //this work complete in hess-reverse_0_init_index

	assert(root->n_in_arcs == 0);
	//reset node index and n_in_arcs - for the Tape location
	root->hess_reverse_0_init_n_in_arcs();
	assert(root->n_in_arcs == 1);
	root->hess_reverse_0();
	double val = NaN_Double;
	root->hess_reverse_get_x(TT->index,val);
//	cout<<TT->toString();
//	cout<<endl;
//	cout<<II->toString();
//	cout<<"======================================= hess_reverse_0"<<endl;
	root->hess_reverse_1_init_x_bar(TT->index);
	assert(root->n_in_arcs == 1);
	root->hess_reverse_1(TT->index);
	assert(root->n_in_arcs == 0);
	assert(II->index==0);
//	cout<<TT->toString();
//	cout<<endl;
//	cout<<II->toString();
//	cout<<"======================================= hess_reverse_1"<<endl;

	uint i =0;
	BOOST_FOREACH(Node* node, vnodes)
	{
		assert(node->getType() == VNode_Type);
		//node->index = 0 means this VNode is not in the tree
		if(node->index!=0)
		{
			double hess = TT->get(node->index -1);
			if(!isnan(hess))
			{
				chess(i) = chess(i) + hess;
			}
		}
		i++;
	}
	assert(i==vnodes.size());
	root->hess_reverse_1_clear_index(); //clear node index on tape
	TT->clear();
	II->clear();
	return val;
}

uint nzGrad(Node* root)
{
	uint nzgrad,total = 0;
	boost::unordered_set<Node*> nodes;
	root->collect_vnodes(nodes,total);
	nzgrad = nodes.size();
	return nzgrad;
}

/*
 * number of non-zero gradient in constraint tree root that also belong to vSet
 */
uint nzGrad(Node* root, boost::unordered_set<Node*>& vSet)
{
	uint nzg= 0;   //non zero grad
	uint zg = 0;   //zero grad , vnode not in the expression rooted by root
	uint nnodes=0; //total nodes in this expression rooted by root
	boost::unordered_set<Node*> vnodes;
	root->collect_vnodes(vnodes,nnodes);
	TRACE("nzGrad - vnodes in expression - size["<<vnodes.size()<<"] -- number node["<<nnodes<<"]");
	BOOST_FOREACH(Node* n, vnodes)
	{
		if(vSet.find(n) != vSet.end())
		{
			nzg++;
		}
		else{
			WARN(" "<<n->toString(0)<<" not in vSet");
			zg++;
		}
	}
	assert(nzg+zg == vnodes.size());
	return nzg;
}

void nonlinearEdges(Node* root, EdgeSet& edges)
{
	root->nonlinearEdges(edges);
}

uint nzHess(EdgeSet& eSet,boost::unordered_set<Node*>& set1, boost::unordered_set<Node*>& set2)
{
	if(eSet.edges.size()==0)
	{
		return 0;
	}

	list<Edge>::iterator i = eSet.edges.begin();
	for(;i!=eSet.edges.end();)
	{
		Edge e =*i;
		Node* a = e.a;
		Node* b = e.b;
		if((set1.find(a)!=set1.end() && set2.find(b)!=set2.end())
			||
			(set1.find(b)!=set1.end() && set2.find(a)!=set2.end()))
		{
			//e is connected between set1 and set2
			i++;
		}
		else
		{
			i = eSet.edges.erase(i);
		}
	}
	uint diag=eSet.numSelfEdges();
	uint nzHess = (eSet.size())*2 - diag;
	return nzHess;
}

uint nzHess(EdgeSet& edges)
{
	uint diag=edges.numSelfEdges();
	uint nzHess = (edges.size())*2 - diag;
	return nzHess;
}

uint numTotalNodes(Node* root)
{
	uint total = 0;
	boost::unordered_set<Node*> nodes;
	root->collect_vnodes(nodes,total);
	return total;
}

string tree_expr(Node* root)
{
	ostringstream oss;
	oss<<"visiting tree == "<<endl;
	int level = 0;
	root->inorder_visit(level,oss);
	return oss.str();
}

void print_tree(Node* root)
{
	cout<<"visiting tree == "<<endl;
	int level = 0;
	root->inorder_visit(level,cout);
}

void autodiff_setup()
{
	Stack::diff = new Stack();
	Stack::vals = new Stack();
	Tape<uint>::indexTape = new Tape<uint>();
	Tape<double>::valueTape = new Tape<double>();
}

void autodiff_cleanup()
{
	delete Stack::diff;
	delete Stack::vals;
	delete Tape<uint>::indexTape;
	delete Tape<double>::valueTape;
}

} //AutoDiff namespace end
