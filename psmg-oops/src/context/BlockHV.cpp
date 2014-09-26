/*
 * BlockHV.cpp
 *
 *  Created on: 2 Apr 2014
 *      Author: s0965328
 */

#include <iostream>
#include <fstream>

#include "../sml/Config.h"
#include "BlockHV.h"

#include "ExpandedModel.h"
#include "../model/AmplModel.h"
#include "../model/VarComp.h"
#include "../model/ObjComp.h"
#include "Var.h"

BlockHV::BlockHV():node(NULL){

}

BlockHV::~BlockHV() {
	delete node;
	node = NULL;
}



void BlockHV::logBlock(ExpandedModel* emrow, ExpandedModel* emcol)
{
	ostringstream oss;
	oss<<GV(logdir)<<emrow->qualifiedName()<<"-"<<emcol->qualifiedName()<<".hvblk";
	ofstream out(oss.str().c_str());

	out<<"HV Block"<<endl;
	out<<"Variables   Declared/Row Model: "<<emrow->name<<"\t\t Size:"<<emrow->numLocalVars<<endl;
	out<<"Variables   Declared/Col Model: "<<emcol->name<<"\t\t Size:"<<emcol->numLocalVars<<endl;
	out<<endl<<"Row Variables:"<<endl;

	vector<string> conNames, varNames;
	BOOST_FOREACH(VarComp* varcomp,emrow->model->var_comps)
	{
		Var* var = static_cast<Var*>(emrow->ctx.getCompValue(varcomp));
		var_multi_map_by_order& var_by_order = var->varMultiMap.get<0>();
		var_multi_map_by_order::iterator ivar = var_by_order.begin();
		for(;ivar!=var_by_order.end();ivar++)
		{
			string name = emrow->name+"_"+var->name+"_"+(*ivar)->indicies;
			out<<"\t"<<name<<"\t\t"<<(*ivar)->toString()<<endl;
		}
	}

	if(emcol!=emrow) {
		out<<endl<<"Col Variables:"<<endl;
		BOOST_FOREACH(VarComp* varcomp,emcol->model->var_comps)
		{
			Var* var = static_cast<Var*>(emcol->ctx.getCompValue(varcomp));
			var_multi_map_by_order& var_by_order = var->varMultiMap.get<0>();
			var_multi_map_by_order::iterator ivar = var_by_order.begin();
			for(;ivar!=var_by_order.end();ivar++)
			{
				string name = emcol->name+"_"+var->name+"_"+(*ivar)->indicies;
				out<<"\t"<<name<<"\t\t"<<(*ivar)->toString()<<endl;
			}
		}
	}

	out<<endl<<"expression"<<endl;
	string node_expr = this->node==NULL?"NULL":tree_expr(this->node);
	out<<node_expr<<endl;
	//printing dependent block information
	out<<endl;
	out.flush();
}
