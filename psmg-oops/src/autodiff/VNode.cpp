/*
 * VNode.cpp
 *
 *  Created on: 8 Apr 2013
 *      Author: s0965328
 */

#include "VNode.h"
#include "Tape.h"
#include "Stack.h"
#include "../context/ExpandedModel.h"
#include "../util/util.h"

using namespace std;

namespace AutoDiff {

#if FORWARD_ENABLED
int VNode::DEFAULT_ID = -1;
#endif

//VNode::VNode(double v) : ActNode(), val(v),u(NaN_Double)
//#if FORWARD_ENABLED
//,id(DEFAULT_ID)
//#endif
//{
//}

VNode::VNode(uint idx) : ActNode(), idx(idx),u(NaN_Double)
#if FORWARD_ENABLED
,id(DEFAULT_ID)
#endif
{
	AD_TRACE("create -- "<<toString(0));
}


VNode::~VNode() {
	AD_TRACE("delete varaible "<<this->toString(0));
}

void VNode::collect_vnodes(boost::unordered_set<Node*>& nodes,uint& total)
{
	total++;
	boost::unordered_set<Node*>::iterator it = nodes.find(this);
	if(it==nodes.end())
		nodes.insert(this);
}

void VNode::inorder_visit(int level,ostream& oss)
{
	oss<<this->toString(level)<<endl;
}

void VNode::eval_function()
{
	SV->push_back(val());
}

string VNode::toString(int level)
{
	ostringstream oss;
	string s(level,'\t');
	oss<<s<<"[VNode](index:"<<index<<",idx:"<<idx<<",val:"<<val()<<",u:"<<u<<") - "<<this;
	return oss.str();
}

void VNode::grad_reverse_0()
{
	this->adj = 0;
	SV->push_back(val());
}

void VNode::grad_reverse_1()
{
	//do nothing
	//this is a leaf node
}

#if FORWARD_ENABLED
void VNode::hess_forward(uint len, double** ret_vec)
{
	assert(id!=DEFAULT_ID);
	(*ret_vec) = new double[len];
	std::fill_n(*ret_vec,len,0);
	(*ret_vec)[id]=1;
	SV->push_back(this->val);
}
#endif



void VNode::nonlinear_edges(EdgeSet& edges)
{
	//leaf node , do nothing
}

uint VNode::hess_reverse_0()
{
	if(index==0)
	{//this node is not on tape
		double nan = NaN_Double;
		TT->set(val());  	//x_i
		TT->set(nan);		//x_bar_i
		TT->set(u);    	//w_i
		TT->set(nan);    	//w_bar_i
		index = TT->index;
	}
//	cout<<toString(0)<<"  -- "<<index<<endl;
	return index;
}

void VNode::hess_reverse_0_get_values(uint i,double& x, double& x_bar, double& w, double& w_bar)
{
	w_bar = TT->get(--i);
	w = TT->get(--i);
	x_bar = TT->get(--i);
	x = TT->get(--i);
}

void VNode::hess_reverse_1(uint i)
{
	n_in_arcs--;
	//leaf node do nothing
}

void VNode::hess_reverse_1_init_x_bar(uint i)
{
	TT->at(i-3) = 1;
}

void VNode::update_x_bar(uint i ,double v)
{
//	cout<<toString(0)<<"  --- "<<__FUNCTION__<<"  v="<<TT->at(i-3)<<"+"<<v<<endl;
	TT->at(i-3) = isnan(TT->get(i-3))? v: TT->get(i-3) + v;
}

void VNode::update_w_bar(uint i,double v)
{
//	cout<<toString(0)<<"  --- "<<__FUNCTION__<<"  v="<<TT->at(i-1)<<"+"<<v<<endl;
	TT->at(i-1) = isnan(TT->get(i-1))? v: TT->get(i-1) + v;
}
void VNode::hess_reverse_1_get_xw(uint i, double& w,double& x)
{
	//cout<<toString(0)<<"  --- "<<__FUNCTION__<<"  w="<<TT->get(i-2)<<"-- "<<"x="<<TT->get(i-4)<<endl;
	w = TT->get(i-2);
	x = TT->get(i-4);
}
void VNode::hess_reverse_get_x(uint i ,double& x)
{
	x = TT->get(i-4);
}


uint VNode::hess_reverse_full0()
{
	if(index == Node::DEFAULT_INDEX)
	{
		double nan = NaN_Double;
		TT->set(val());
		TT->set(nan);
		index = TT->index;
	}
	return index;
}

void VNode::hess_reverse_full1(uint i,EdgeSet& eset)
{
	//leaf node , do nothing
}

void VNode::hess_reverse_full0_get_x(uint i, double& v)
{
	v = TT->at(i-2);
}

void VNode::hess_reverse_full1_init_x_bar(uint i)
{
	TT->at(i-1) = 1;
}
void VNode::hess_reverse_full1_update_x_bar(uint i,double& v)
{
	TT->at(i-1) += v;
}

TYPE VNode::getType()
{
	return VNode_Type;
}

double& VNode::val()
{
	assert(idx < ExpandedModel::n_col);
	assert(ExpandedModel::X!=NULL);
	return ExpandedModel::X[idx];
}

}
