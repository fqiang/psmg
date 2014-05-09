/*
 * Block.cpp
 *
 *  Created on: 25 Sep 2013
 *      Author: s0965328
 */

#include "BlockDep.h"
#include "ExpandedModel.h"
#include "ModelContext.h"

BlockDep::BlockDep(){

}

BlockDep::~BlockDep() {
}

uint BlockDep::getNumDepVars(){
	uint numVars = 0;
	for(vector<ExpandedModel*>::iterator it=ems.begin();it!=ems.end();it++)
	{
		numVars+=(*it)->numLocalVars;
	}
	return numVars;
}

void BlockDep::logBlock(ostream& out)
{
	out<<"------------------BEGIN Dependent Expanded Models----------------------------"<<endl;
	for(vector<ExpandedModel*>::iterator it=ems.begin();it!=ems.end();it++)
	{
		out<<"name["<<(*it)->name<<"["<<(*it)->ctx->getContextId()<<"] - nvar["<<(*it)->numLocalVars<<"] ncon["<<(*it)->numLocalCons<<"]"<<endl;
	}
	out<<"------------------END Dependent Expanded Models----------------------------"<<endl;
	out<<endl;
}
