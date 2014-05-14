/*
 * EMBlock.cpp
 *
 *  Created on: 16 May 2013
 *      Author: s0965328
 */
#include "BlockConsFull.h"

#include <fstream>

#include "BlockDep.h"
#include "../sml/Config.h"
#include "ExpandedModel.h"
#include "../model/AmplModel.h"
#include "../model/VarComp.h"
#include "Var.h"

BlockConsFull::BlockConsFull(BlockDep* b): block(b){
}

BlockConsFull::~BlockConsFull() {

}


void BlockConsFull::logBlock(ExpandedModel* emrow)
{
	ostringstream oss;
	oss<<GV(logdir)<<emrow->qualifiedName()<<".fullconsblk";
	ofstream out(oss.str().c_str());

	out<<"Constraint Block - Full"<<endl;
	out<<"Constraints Declared/Row Model: "<<emrow->name<<"\t\t Size:"<<emrow->numLocalCons<<endl;

	out<<endl<<"Local Constraints"<<endl;
	vector<string> consNames;
	emrow->get_cons_names(consNames);
	for(unsigned int i=0;i<consNames.size();i++){
		AutoDiff::Node* cnode = this->cons[i];
		string node_expr = cnode==NULL?"NULL":tree_expr(cnode);
		out<<i<<"\t"<<consNames[i]<<"\t\t"<<node_expr<<endl;
	}
	//printing dependent block information
	out<<endl;
	this->block->logBlock(out);
	out<<endl;
	out.flush();

}
