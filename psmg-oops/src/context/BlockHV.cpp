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
	oss<<GV(logdir)<<emrow->qualifiedName()<<"_"<<emcol->qualifiedName()<<".hvblk";
	ofstream out(oss.str().c_str());

	out<<"HV Block"<<endl;
	out<<"Variables   Declared/Row Model: "<<emrow->name<<"\t\t Size:"<<emrow->numLocalCons<<endl;
	out<<"Variables   Declared/Col Model: "<<emcol->name<<"\t\t Size:"<<emcol->numLocalVars<<endl;
	out<<endl<<"Local Variables:"<<endl;
	vector<string> conNames, varNames;
	std::vector<VarComp*>::iterator varcomp=emcol->model->var_comps.begin();
	for(;varcomp!=emcol->model->var_comps.end();varcomp++)
	{
		Var* var = static_cast<Var*>(emcol->ctx.getCompValue(*varcomp));
		var_multi_map_by_order& var_by_order = var->varMultiMap.get<0>();
		var_multi_map_by_order::iterator ivar = var_by_order.begin();
		for(;ivar!=var_by_order.end();ivar++)
		{
			string name = emcol->name+"_"+var->name+"_"+(*ivar)->indicies;
			out<<"\t"<<name<<"\t\t"<<(*ivar)->toString()<<endl;
		}
	}

	out<<endl<<"expression"<<endl;
//	emrow->get_cons_names(conNames);
//	assert(conNames.size()==emrow->numLocalCons);
//	uint i =0;
//	BOOST_FOREACH(AutoDiff::Node* cnode, this->blka->cons)
//	{
//		string node_expr = cnode==NULL?"NULL":tree_expr(cnode);
//		out<<i<<"\t"<<conNames[i]<<"\t\t"<<node_expr<<endl;
//		i++;
//	}

	//printing dependent block information
	out<<endl;
	out.flush();
}
