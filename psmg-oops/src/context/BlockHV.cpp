/*
 * BlockHV.cpp
 *
 *  Created on: 2 Apr 2014
 *      Author: s0965328
 */

#include <iostream>
#include "BlockHV.h"
#include "BlockDep.h"
#include "BlockCons.h"
#include "BlockObj.h"

#include "ExpandedModel.h"
#include "../model/AmplModel.h"
#include "../model/VarComp.h"
#include "../model/ObjComp.h"
#include "Var.h"

BlockHV::BlockHV(BlockCons* bc,BlockObj* o):blkcons(bc), blkobj(o) {
	// TODO Auto-generated constructor stub

}

BlockHV::~BlockHV() {
	// TODO Auto-generated destructor stub
}



void BlockHV::logBlock(ExpandedModel* emrow, ExpandedModel* emcol,ostream& out)
{
	bool isFull = emcol==NULL? true:false;
	if(isFull==false)
	{
		out<<"Lagragian Hessian Block - Partial"<<endl;
		out<<"Variables Declared/Row Model: "<<emrow->name<<"\t\t Size:"<<emrow->numLocalVars<<endl;
		out<<"Variables Declared/Col Model: "<<emcol->name<<"\t\t Size:"<<emcol->numLocalVars<<endl;
		out<<endl<<"Column Variables:"<<endl;
		vector<VarComp*>::iterator varcomp =emcol->model->var_comps.begin();
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
		out<<"Lagragian Hessian Block - Full"<<endl;
		out<<"Variables Declared/Row Model: "<<emrow->name<<"\t\t Size:"<<emrow->numLocalVars<<endl;
	}
	out<<endl<<"Row Variables:"<<endl;
	std::vector<VarComp*>::iterator varcomp;
	varcomp=emrow->model->var_comps.begin();
	for(;varcomp!=emrow->model->var_comps.end();varcomp++)
	{
		Var* var = static_cast<Var*>(emrow->ctx->getCompValue(*varcomp));
		var_multi_map_by_order& var_by_order = var->varMultiMap.get<0>();
		var_multi_map_by_order::iterator ivar = var_by_order.begin();
		for(;ivar!=var_by_order.end();ivar++)
		{
			string name = emrow->ctx->getContextId()+"_"+var->name+"_"+(*ivar)->indicies;
			out<<"\t"<<name<<"\t\t"<<(*ivar)->toString()<<endl;
		}
	}

	out<<endl<<"Constraints used:"<<endl;
	vector<string> consNames;
	BOOST_FOREACH(ExpandedModel* em, blkcons->block->ems)
	{
		em->get_cons_names(consNames);
	}

	BOOST_FOREACH(AutoDiff::Node* cnode, blkcons->cons)
	{
		uint i =0;
		string node_expr = cnode==NULL?"NULL":visit_tree(cnode);
		out<<i<<"\t"<<consNames[i]<<"\t\t"<<node_expr<<endl;
		i++;
	}
	out<<endl<<"Objective used:"<<endl;
	string objname = emrow->model->obj_comp==NULL?"null":emrow->model->obj_comp->name;
	AutoDiff::Node* onode = blkobj==NULL? NULL:blkobj->objective;
	string onode_expr = onode==NULL?"NULL":visit_tree(onode);
	out<<"\t"<<objname<<"\t\t"<<onode_expr<<endl;

	//printing dependent block information
	out<<endl;
	this->blkcons->block->logBlock(out);
	out<<endl;
	out.flush();

}
