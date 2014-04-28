/*
 * Block.cpp
 *
 *  Created on: 25 Sep 2013
 *      Author: s0965328
 */

#include "Block.h"

Block::Block() {
	// TODO Auto-generated constructor stub

}

Block::~Block() {
	// TODO Auto-generated destructor stub
}
void Block::logBlock(ostream& out)
{
	out<<"------------------BEGIN Dependent Expanded Models----------------------------"<<endl;
	for(vector<ExpandedModel*>::iterator it=ems.begin();it!=ems.end();it++)
	{
		out<<"name["<<(*it)->name<<"["<<(*it)->ctx->getContextId()<<"] - nvar["<<(*it)->numLocalVars<<"] ncon["<<(*it)->numLocalCons<<"]"<<endl;
	}
	out<<"------------------END Dependent Expanded Models----------------------------"<<endl;
	out<<endl;
}
