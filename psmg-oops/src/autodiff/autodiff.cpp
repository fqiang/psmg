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


PIndex* create_param_node(uint idx){
	return new PIndex(idx);
}

PVal* create_param_node(double& val){
	return new PVal(val);
}
VNode* create_var_node(uint idx)
{
	return new VNode(idx);
}
Node* create_binary_op_node(OPCODE code, Node* left, Node* right)
{
	return BinaryOPNode::createBinaryOpNode(code,left,right);
}
Node* create_uary_op_node(OPCODE code, Node* left)
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
	root->hess_reverse_clear_index();
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

	root->hess_reverse_0();
	double val = NaN_Double;
	root->hess_reverse_get_x(TT->index,val);
//	cout<<TT->toString();
//	cout<<endl;
//	cout<<II->toString();
//	cout<<"======================================= hess_reverse_0"<<endl;
	root->hess_reverse_1_init_x_bar(TT->index);

	assert(root->n_in_arcs == 0);
	//reset node index and n_in_arcs - for the Tape location
	root->hess_reverse_0_init_n_in_arcs();
	assert(root->n_in_arcs == 1);
	assert(root->n_in_arcs == 1);
	root->hess_reverse_1(TT->index);
	assert(root->n_in_arcs == 0);
//	cout<<TT->toString();
//	cout<<endl;
//	cout<<II->toString();
//	cout<<"======================================= hess_reverse_1"<<endl;
	assert(II->index==0);

	uint i =0;
	BOOST_FOREACH(Node* node, vnodes)
	{
		assert(node->n_in_arcs == 0);
		assert(node->getType() == VNode_Type);
		//node->index = 0 means this VNode is not in the tree
		if(node->index!=0)
		{
			double hess = TT->get(node->index -1);
			assert(!isnan(hess));
//			chess(i) = hess;
			if(!isnan(hess))
			{
				chess(i) =  hess;
			}
		}
		i++;
	}
	assert(i==vnodes.size());
	cout<<"-====================="<<chess<<endl;
	root->hess_reverse_clear_index(); //clear node index on tape
	TT->clear();
	II->clear();
	return val;
}


/*
 * Reverse Hessian matrix algorithm by computing Hessian-vector product column by column
 */
double hess_reverse(Node* root,vector<Node*>& vnodes,col_compress_matrix& hess)
{
	double val = NaN_Double;

	for(uint c = 0;c<vnodes.size();c++)
	{
		//initialize the weight for computing column c of Hessian m
		//this is required by Autodiff_library
		BOOST_FOREACH(AutoDiff::Node* v, vnodes)
		{
			static_cast<AutoDiff::VNode*>(v)->u = 0;
		}
		static_cast<AutoDiff::VNode*>(vnodes[c])->u = 1; //set weight = 1 for column c varible only, rest are 0

		//calculate the column c for Hessian m
		col_compress_matrix_col chess(hess,c);


		TT->clear();
		II->clear();
		assert(TT->empty());
		assert(II->empty());
		assert(TT->index==0);
		assert(II->index==0);

		root->hess_reverse_0();
		root->hess_reverse_get_x(TT->index,val);
	//	cout<<TT->toString();
	//	cout<<endl;
	//	cout<<II->toString();
	//	cout<<"======================================= hess_reverse_0"<<endl;
		root->hess_reverse_1_init_x_bar(TT->index);

		assert(root->n_in_arcs == 0);
		//reset node index and n_in_arcs - for the Tape location
		root->hess_reverse_0_init_n_in_arcs();
		assert(root->n_in_arcs == 1);
		root->hess_reverse_1(TT->index);
		assert(root->n_in_arcs == 0);
	//	cout<<TT->toString();
	//	cout<<endl;
	//	cout<<II->toString();
	//	cout<<"======================================= hess_reverse_1"<<endl;
		assert(II->index==0);

		uint i =0;
		BOOST_FOREACH(Node* node, vnodes)
		{
			assert(node->n_in_arcs == 0);
			assert(node->getType() == VNode_Type);
			//node->index = 0 means this VNode is not in the tree
			if(node->index!=0)
			{
				double hess = TT->get(node->index -1);
				assert(!isnan(hess));
	//			chess(i) = hess;
				if(!isnan(hess))
				{
					chess(i) =  hess;
				}
			}
			i++;
		}
		assert(i==vnodes.size());
		root->hess_reverse_clear_index(); //clear node index on tape
		TT->clear();
		II->clear();
	}
	return val;
}

/*
 * More efficient implementation of reverse Hessian algorithm using Edge Pushing
 *
 */
double hess_reverse_ep(Node* root, boost::unordered_map<Node*,uint>& colvMap, boost::unordered_map<Node*,uint>& rowvMap, col_compress_matrix& hess)
{
	double val = NaN_Double;
	TT->clear();
	II->clear();
	root->hess_reverse_full0(); //compute x, hl, hr, hlr

	root->hess_reverse_full1_init_x_bar(TT->index); //init x_bar = 1
	EdgeSet eset;  									// edge set
	root->hess_reverse_full1(TT->index,eset);		//

	//now recovery the Hessian matrix from EdgeSet
	if(eset.edges.size() == 0)
	{
		return val;
	}
	list<Edge>::iterator i = eset.edges.begin();
	for(;i!=eset.edges.end();i++)
	{
		Edge e =*i;
		Node* a = e.a;
		Node* b = e.b;

		//case 1.
		boost::unordered_map<Node*,uint>::iterator itcol = colvMap.find(a);
		boost::unordered_map<Node*,uint>::iterator itrow = rowvMap.find(b);
		if(itcol!=colvMap.end() && itrow!=rowvMap.end())
		{
			uint row = itrow->second;
			uint col = itcol->second;
			hess(row,col) = e.w;
		}
		//case 2
		itcol = colvMap.find(b);
		itrow = rowvMap.find(a);
		if(itcol!=colvMap.end() && itrow!=rowvMap.end())
		{
			uint row = itrow->second;
			uint col = itcol->second;
			hess(row,col) = e.w;
		}
		//otherwise edge is not connect colvMap and rowvMap.
	}

	root->hess_reverse_clear_index();
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
	BOOST_FOREACH(Node* n, vSet)
	{
		if(vnodes.find(n) != vnodes.end())
		{
			nzg++;
		}
		else{
			WARN(" "<<n->toString(0)<<" not in vSet");
			zg++;
		}
	}
	assert(nzg+zg == vSet.size());
	return nzg;
}

uint nzGrad(Node* root, std::vector<Node*>& vlist, col_compress_imatrix_row& rgrad)
{
	uint nzg = 0;
	uint zg = 0;
	uint nnodes = 0;
	boost::unordered_set<Node*> vnodes;
	root->collect_vnodes(vnodes,nnodes);
	uint c = 0;
	BOOST_FOREACH(Node* n, vlist)
	{
		if(vnodes.find(n)!=vnodes.end())
		{
			rgrad(c) = 1; //setting structure non-zero.
			nzg ++;
		}
		else
		{
			zg++;
		}
		c++;
	}
	assert(nzg+zg == vlist.size());
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
	uint nzh = 0;
	uint nzd = 0;
	list<Edge>::iterator i = eSet.edges.begin();
	for(;i!=eSet.edges.end();i++)
	{
		Edge e =*i;
		Node* a = e.a;
		Node* b = e.b;

		boost::unordered_set<Node*>::iterator itcol = set1.find(a);
		boost::unordered_set<Node*>::iterator itrow = set2.find(b);

		//case diagonal
		if(itcol!=set1.end()&& itrow!=set2.end())
		{
			if(a==b) nzd++; //only for diagnal block in Q matrix
		}

		//case 1;
		if(itcol!=set1.end() && itrow!=set2.end()){
			//e is connected between set1 and set2
			nzh ++;
		}

		//case 2;
		itcol = set1.find(b);
		itrow = set2.find(a);
		if(itcol!=set1.end() && itrow!=set2.end()){
			nzh ++;
		}
	}

	nzh -= nzd;
	return nzh;
}

uint nzHess(EdgeSet& eSet,boost::unordered_map<Node*,uint>& colvMap, boost::unordered_map<Node*,uint>& rowvMap,col_compress_imatrix& m)
{
	uint nzh = 0;
	uint nzd = 0;
	if(eSet.edges.size() == 0)
	{
		return 0;
	}
	list<Edge>::iterator i = eSet.edges.begin();
	for(;i!=eSet.edges.end();i++)
	{
		Edge e =*i;
		Node* a = e.a;
		Node* b = e.b;

		//case 1.
		boost::unordered_map<Node*,uint>::iterator itcol = colvMap.find(a);
		boost::unordered_map<Node*,uint>::iterator itrow = rowvMap.find(b);

		if(itcol!=colvMap.end()&& itrow!=rowvMap.end())
		{
			if(a==b) nzd++; //only for diagnal block in Q matrix
		}
		if(itcol!=colvMap.end() && itrow!=rowvMap.end())
		{
			uint row = itrow->second;
			uint col = itcol->second;
			m(row,col) = 1;
			nzh++;
		}
		//case 2
		itcol = colvMap.find(b);
		itrow = rowvMap.find(a);
		if(itcol!=colvMap.end() && itrow!=rowvMap.end())
		{
			uint row = itrow->second;
			uint col = itcol->second;
			m(row,col) = 1;
			nzh++;
		}
		//otherwise edge is not connect colvMap and rowvMap.
	}
	//diagnoal will be double counted!
	nzh -= nzd;
	assert(nzh == m.nnz());
	return nzh;
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
