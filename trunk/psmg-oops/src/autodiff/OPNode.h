/*
 * OpNode.h
 *
 *  Created on: 8 Apr 2013
 *      Author: s0965328
 */

#ifndef OPNODE_H_
#define OPNODE_H_

#include "Node.h"
#include "ActNode.h"

namespace AutoDiff {

using namespace std;

class OPNode: public ActNode {
public:
	OPNode(OPCODE op,Node* left);
	virtual ~OPNode();


	OPCODE op;
	Node* left;
	double val;

	TYPE getType();

private:

};

}

#endif /* OPNODE_H_ */
