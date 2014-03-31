///*
// * StageNodeNode.h
// *
// *  Created on: 28 Sep 2011
// *      Author: s0965328
// */
//
//#ifndef STAGENODENODE_H_
//#define STAGENODENODE_H_
//
//#include "../model/SyntaxNode.h"
//
///** @class StageNodeNode
// */
//class StageNodeNode : public SyntaxNode {
//
// private:
//  std::string value_;
//
// public:
//
//  /** Current replacement string for the 'node' keyword */
//  static std::string node;
//
//  /** Current replacement string for the 'stage' keyword */
//  static std::string stage;
//
//  /** Constructor */
//  StageNodeNode(int opCode_, const std::string& value = "");
//
//  std::ostream& put(std::ostream& s) const;
//  SyntaxNode *clone();
//  SyntaxNode *deep_copy();
//
//  void setValue(const std::string& value);
//
//  virtual void calculateMemoryUsage(unsigned long& size);
//
//};
//
//#endif /* STAGENODENODE_H_ */
