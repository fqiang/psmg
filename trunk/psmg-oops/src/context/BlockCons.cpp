/*
 * EMBlock.cpp
 *
 *  Created on: 16 May 2013
 *      Author: s0965328
 */
#include <iostream>
#include "BlockCons.h"

#include "ExpandedModel.h"
#include "../model/AmplModel.h"
#include "../model/VarComp.h"
#include "Var.h"

BlockCons::BlockCons(BlockDep* b): block(b){
}

BlockCons::~BlockCons() {

}


void BlockCons::logBlock(ExpandedModel* emrow, ExpandedModel* emcol,ostream& out)
{
	bool isFull = emcol==NULL?true:false;
	if(isFull==false){
		out<<"Constraint Block - Partial"<<endl;
		out<<"Constraints Declared/Row Model: "<<emrow->name<<"\t\t Size:"<<emrow->numLocalCons<<endl;
		out<<"Variables   Declared/Col Model: "<<emcol->name<<"\t\t Size:"<<emcol->numLocalVars<<endl;
		out<<endl<<"Local Variables:"<<endl;
		vector<string> varNames;
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
	}
	else
	{
		out<<"Constraint Block - Full"<<endl;
		out<<"Constraints Declared/Row Model: "<<emrow->name<<"\t\t Size:"<<emrow->numLocalCons<<endl;
	}

	out<<endl<<"Local Constraints"<<endl;
	vector<string> consNames;
	emrow->get_cons_names(consNames);
	for(unsigned int i=0;i<consNames.size();i++){
		AutoDiff::Node* cnode = this->cons[i];
		string node_expr = cnode==NULL?"NULL":visit_tree(cnode);
		out<<i<<"\t"<<consNames[i]<<"\t\t"<<node_expr<<endl;
	}
	//printing dependent block information
	out<<endl;
	this->block->logBlock(out);
	out<<endl;
	out.flush();

}
