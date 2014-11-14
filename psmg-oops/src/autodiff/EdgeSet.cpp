/*
 * EdgeSet.cpp
 *
 *  Created on: 12 Nov 2013
 *      Author: s0965328
 */

#include "EdgeSet.h"
#include "Edge.h"
#include <sstream>

using namespace std;
namespace AutoDiff {

EdgeSet::EdgeSet() {
	// TODO Auto-generated constructor stub
	edges.clear();

}

EdgeSet::~EdgeSet() {
	edges.clear();
}

/*
 * create edge a-b if not already in edge set
 */
Edge& EdgeSet::createEdge(Node* a, Node* b)
{
	Edge e(a,b);
	list<Edge>::iterator it = edges.begin();
	for(;it!=edges.end();it++)
	{
		Edge& eit = *it;
		if(eit.isEqual(e)){
			return eit;
		}
	}
	edges.push_front(e);
	return edges.front();
}

uint EdgeSet::size(){
	return edges.size();
}

uint EdgeSet::num_self_edges(){
	uint diag = 0;
	list<Edge>::iterator it = edges.begin();
	for(;it!=edges.end();it++)
	{
		Edge eit = *it;
		if(eit.a == eit.b)
		{
			diag++;
		}
	}
	return diag;
}

void EdgeSet::clear() {
	edges.clear();
}

string EdgeSet::toString()
{
	ostringstream oss;
	list<Edge>::iterator it = edges.begin();
	for(;it!=edges.end();it++)
	{
		oss<<(*it).toString()<<endl;
	}
	return oss.str();
}


} /* namespace AutoDiff */
