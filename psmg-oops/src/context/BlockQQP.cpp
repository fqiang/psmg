/*
 * BlockQQP.cpp
 *
 *  Created on: 14 May 2014
 *      Author: s0965328
 */

#include "BlockQQP.h"
#include "BlockDep.h"
#include "ExpandedModel.h"
#include "../model/AmplModel.h"
#include "../model/SyntaxNode.h"
#include "../model/ObjComp.h"
#include "../sml/Config.h"
#include <fstream>

BlockQQP::BlockQQP(BlockDep* b) : block(b) {

}

BlockQQP::~BlockQQP() {
	// TODO Auto-generated destructor stub
}

void BlockQQP::logBlock(ExpandedModel* emrow, ExpandedModel* emcol)
{
	ostringstream oss;
	oss<<GV(logdir)<<emrow->qualifiedName()<<"-"<<emcol->qualifiedName()<<".qblk";
	ofstream out(oss.str().c_str());

	out<<"Q (QP) Block "<<endl;
	out<<"Row Declared/Row Model: "<<emrow->name<<"\t\t Size:"<<emrow->numLocalVars<<endl;
	out<<"Col Declared/Col Model: "<<emcol->name<<"\t\t Size:"<<emcol->numLocalVars<<endl;

	out<<endl<<"Constraints Involved:"<<endl;
	vector<string> objnames;

	BOOST_FOREACH(ExpandedModel* em, block->ems)
	{
		SyntaxNode* attr = NULL;
		if(em->model->obj_comp!=NULL){
			boost::unordered_map<int,SyntaxNode*>::iterator it= em->model->obj_comp->higher_partial.find(emcol->model->level);
			attr = (it == em->model->obj_comp->higher_partial.end()? NULL: it->second);
		}
		if(attr!=NULL)
		{
			objnames.push_back(em->model->obj_comp->name);
		}
	}
	assert(objnames.size()==objs.size());

	for(unsigned int i=0;i<objnames.size();i++){
		AutoDiff::Node* cnode = this->objs[i];
		string node_expr = cnode==NULL?"NULL":tree_expr(cnode);
		out<<i<<"\t"<<objnames[i]<<"\t\t"<<node_expr<<endl;
	}
	//printing dependent block information
	out<<endl;
	this->block->logBlock(out);
	out<<endl;
	out.flush();
}
