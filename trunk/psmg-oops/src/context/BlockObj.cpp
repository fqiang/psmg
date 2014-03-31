/*
 * CBlock.cpp
 *
 *  Created on: 24 Sep 2013
 *      Author: s0965328
 */

#include "BlockObj.h"
#include "../model/AmplModel.h"
#include "../model/VarComp.h"
#include "Var.h"

BlockObj::BlockObj(Block* b) : block(b), objective(NULL){

}

BlockObj::~BlockObj() {

}

void BlockObj::logBlock(ExpandedModel* emrow, ExpandedModel* emcol,ostream& out)
{
	out<<"Objective Block"<<endl;
	out<<"Objective Declared/Row Model: "<<emrow->name<<"\t\t Size:"<<(emrow->model->obj_comp==NULL?"NULL":"1")<<endl;
	out<<"Variables Declared/Col Model: "<<emcol->name<<"\t\t Size:"<<emcol->getNLocalVars()<<endl;
	out<<endl;
	out<<endl<<"Local Variables:"<<endl;
	vector<string> conNames, varNames;
	std::vector<VarComp*>::iterator varcomp=emcol->model->var_comps.begin();
	for(;varcomp!=emcol->model->var_comps.end();varcomp++)
	{
		Var* var = static_cast<Var*>(emcol->ctx->getCompValue(*varcomp));
		var_multi_map_by_order& var_by_order = var->varMultiMap.get<0>();
		var_multi_map_by_order::iterator ivar = var_by_order.begin();
		for(;ivar!=var_by_order.end();ivar++)
		{
			string name = emcol->ctx->getContextId()+"_"+var->name+"_"+ivar->indicies;
			out<<"\t"<<name<<"\t\t"<<ivar->toString()<<endl;
		}
	}

	out<<endl<<"Object Constraint"<<endl;
	if(this->objective == NULL)
	{
		out<<"";
	}
	else{
		string objname = emrow->model->obj_comp->name;
		AutoDiff::Node* onode = this->objective;
		string node_expr = onode==NULL?"NULL":visit_tree(onode);
		out<<"\t"<<objname<<"\t\t"<<node_expr<<endl;
	}
	//printing dependent block information
	out<<endl;
	this->block->logBlock(out);
	out<<endl;
	out.flush();
}
