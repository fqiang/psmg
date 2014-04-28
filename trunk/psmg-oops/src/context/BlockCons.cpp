/*
 * EMBlock.cpp
 *
 *  Created on: 16 May 2013
 *      Author: s0965328
 */
#include <iostream>
#include "BlockCons.h"

#include "../model/AmplModel.h"
#include "../model/VarComp.h"
#include "Var.h"

BlockCons::BlockCons(Block* b): block(b){
}

BlockCons::~BlockCons() {

}


void BlockCons::logBlock(ExpandedModel* emrow, ExpandedModel* emcol,ostream& out)
{
	out<<"Constraint Block"<<endl;
	out<<"Constraints Declared/Row Model: "<<emrow->name<<"\t\t Size:"<<emrow->numLocalCons<<endl;
	out<<"Variables   Declared/Col Model: "<<emcol->name<<"\t\t Size:"<<emcol->numLocalVars<<endl;
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
			string name = emcol->ctx->getContextId()+"_"+var->name+"_"+(*ivar)->indicies;
			out<<"\t"<<name<<"\t\t"<<(*ivar)->toString()<<endl;
		}
	}

	out<<endl<<"Local Constraints"<<endl;
	emrow->get_local_cons_names(conNames);
	for(unsigned int i=0;i<conNames.size();i++){
		AutoDiff::Node* cnode = this->cons[i];
		string node_expr = cnode==NULL?"NULL":visit_tree(cnode);
		out<<i<<"\t"<<conNames[i]<<"\t\t"<<node_expr<<endl;
	}
	//printing dependent block information
	out<<endl;
	this->block->logBlock(out);
	out<<endl;
	out.flush();

}
