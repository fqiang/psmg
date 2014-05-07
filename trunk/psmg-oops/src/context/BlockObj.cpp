/*
 * CBlock.cpp
 *
 *  Created on: 24 Sep 2013
 *      Author: s0965328
 */

#include "BlockObj.h"
#include "ExpandedModel.h"
#include "../model/AmplModel.h"
#include "../model/VarComp.h"
#include "Var.h"

//BlockObj::BlockObj(BlockDep* b) : block(b), objective(NULL){
//
//}

BlockObj::BlockObj(ExpandedModel* em) : emthis(em), objective(NULL)
{

}

BlockObj::~BlockObj() {

}

void BlockObj::logBlock(ExpandedModel* emrow, ostream& out)
{
	assert(emthis == emrow);
	string objname = emthis->model->obj_comp==NULL? "null":emthis->model->obj_comp->name;
	out<<"Objective Block - (Assume to be Partial)"<<endl;
	out<<"Objective Declared/Row Model: "<<emthis->name<<"\t\t Name:"<<objname<<endl;
	out<<"Variables Declared/Col Model: "<<emthis->name<<"\t\t Size:"<<emthis->numLocalVars<<endl;
	out<<endl;
	out<<endl<<"Local Variables:"<<endl;
	vector<string> conNames, varNames;
	std::vector<VarComp*>::iterator varcomp=emthis->model->var_comps.begin();
	for(;varcomp!=emthis->model->var_comps.end();varcomp++)
	{
		Var* var = static_cast<Var*>(emthis->ctx->getCompValue(*varcomp));
		var_multi_map_by_order& var_by_order = var->varMultiMap.get<0>();
		var_multi_map_by_order::iterator ivar = var_by_order.begin();
		for(;ivar!=var_by_order.end();ivar++)
		{
			string name = emthis->ctx->getContextId()+"_"+var->name+"_"+(*ivar)->indicies;
			out<<"\t"<<name<<"\t\t"<<(*ivar)->toString()<<endl;
		}
	}

	out<<endl<<"Object Constraint"<<endl;
	string node_expr = this->objective==NULL?"NULL":visit_tree(this->objective);
	out<<"\t"<<objname<<"\t\t"<<node_expr<<endl;

	//printing dependent block information
	out<<endl;
	out.flush();
}
