/*
 * Node.cpp
 *
 *  Created on: 8 Apr 2013
 *      Author: s0965328
 */

#include "Node.h"

namespace AutoDiff {

uint Node::DEFAULT_INDEX = 0;
Node::Node():index(Node::DEFAULT_INDEX),n_in_arcs(0){
}


Node::~Node() {
}

void Node::hess_reverse_0_init_n_in_arcs()
{
	n_in_arcs++;
//	cout<<this->toString(0)<<endl;
}

void Node::hess_reverse_clear_index()
{
	index = Node::DEFAULT_INDEX;
}

void Node::print()
{
	std::cout<<this->toString(0)<<endl;
}

void Node::ptree()
{
	ostringstream oss;
	this->inorder_visit(0,oss);
	std::cout<<oss.str()<<endl;
}

}

