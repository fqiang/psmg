/*
 * Edge.h
 *
 *  Created on: 12 Nov 2013
 *      Author: s0965328
 */

#ifndef EDGE_H_
#define EDGE_H_


#include "Node.h"

namespace AutoDiff {

class Edge {
public:
	Edge(Node* a, Node* b);
	Edge(const Edge& e);
	virtual ~Edge();

	bool isEqual(Edge*);
	bool isEqual(Edge&);
	std::string toString();
	void update_w(double v);

	Node* a;
	Node* b;

	double w;
};

} /* namespace AutoDiff */
#endif /* EDGE_H_ */
