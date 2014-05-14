///*
// * OpNode.cpp
// *
// *  Created on: 28 Sep 2011
// *      Author: s0965328
// */
//
//#include "SyntaxNodeOP.h"
//
//#include "../util/util.h"
//#include "../model/ObjComp.h"
//#include "../parser/sml.tab.h"
//
//
//SyntaxNodeOP::SyntaxNodeOP(int opCode, SyntaxNode *op1, SyntaxNode *op2) :
//  SyntaxNode(opCode, op1, op2)
//{
//	LOG("OpNode constructor called -- opCode["<<opCode<<"]");
//}
//
//SyntaxNodeOP::~SyntaxNodeOP()
//{
//}
//
//SyntaxNodeOP::SyntaxNodeOP(const SyntaxNodeOP& src):SyntaxNode(src)
//{
//}
//
//SyntaxNodeOP* SyntaxNodeOP::clone()
//{
//	return new SyntaxNodeOP(*this);
//}
//
//ostream& SyntaxNodeOP::put(ostream &s)
//{
//	SyntaxNode* left = this->values[0];
//	SyntaxNode* right= this->nchild()==2? this->values[1]: NULL;
//	if(left)
//	  s <<"("<< left << " ";
//
//	switch(opCode) {
//	case PLUS:
//		s << "+";
//		break;
//	case MINUS:
//		s << "-";
//		break;
//	case TIMES:
//		s << "*";
//		break;
//	case DIVID:
//	   s << "/";
//	   break;
//	case GE:      s << ">=";  break;
//	case GT:      s << ">";   break;
//	case LE:      s << "<=";  break;
//	case LT:      s << "<";   break;
//	case EQ:      s << "==";  break;
//	case NE:      s << "!=";  break;
//	case POWER:   s << "^";  break;
//	case COS:	  s << "cos"; break;
//	case SIN:	s<<"sin"; break;
//	default:
//	  cerr << "Unknown opCode for OpNode: " << opCode << endl;
//	  assert(false);
//	}
//
//	if(right)
//	  s << " " << right <<")";
//
//	return s;
//}