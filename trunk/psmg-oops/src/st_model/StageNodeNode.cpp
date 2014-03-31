///*
// * StageNodeNode.cpp
// *
// *  Created on: 28 Sep 2011
// *      Author: s0965328
// */
//
//#include "StageNodeNode.h"
//#include "../util/global_util_functions.h"
//#include "../parser/sml.tab.h"
//
//string StageNodeNode::node = "";
//string StageNodeNode::stage = "";
//
// /** Constructor */
//StageNodeNode::StageNodeNode(int opCode_, const std::string& value) :
//     SyntaxNode(opCode_), value_(value)
//{
//	LOG("StageNodeNode constructor called -- opCode["<<opCode_<<"]");
//}
//
//SyntaxNode* StageNodeNode::clone()
//{
//	return new StageNodeNode(opCode, value_);
//}
//
//SyntaxNode* StageNodeNode::deep_copy()
//{
//	return clone();
//}
//
//void StageNodeNode::setValue(const std::string& value)
//{
//	value_ = value;
//}
//
//
//ostream& StageNodeNode::put(ostream &s) const {
//   if(value_!="") return s << value_;
//
//   switch(opCode) {
//      case STAGE: s << StageNodeNode::stage;    break;
//      case NODE:  s << StageNodeNode::node;     break;
//      default:
//         cerr << "StageNodeNode::put called badly!" << endl;
//         throw exception();
//   }
//   return s;
//}
//
//void StageNodeNode::calculateMemoryUsage(unsigned long& size)
//{
//	size += sizeof(StageNodeNode);
//	size += this->value_.size() + 1;
//	this->calculateBaseValueVector(size);
//}
