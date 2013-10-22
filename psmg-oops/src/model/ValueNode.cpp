/*
 * ValueNode.cpp
 *
 *  Created on: 28 Sep 2011
 *      Author: s0965328
 */

#include "ValueNode.h"

ValueNode::ValueNode(double new_value) : SyntaxNode(-99), value(new_value)
{
	LOG("ValueNode constructor --- opCode [-99] value["<<new_value<<"]");
}

ValueNode::~ValueNode()
{

}


string ValueNode::getValue() const
{
   std::ostringstream ost;
   ost << value;
   return ost.str();
}

void ValueNode::calculateMemoryUsage(unsigned long& size)
{
	size += sizeof(ValueNode);
	this->calculateBaseValueVector(size);
}
