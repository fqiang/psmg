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
#include "../model/ObjComp.h"
#include "Var.h"
#include "../sml/Config.h"
#include <fstream>


BlockObj::BlockObj() : objective(NULL)
{

}

BlockObj::~BlockObj() {

}

void BlockObj::logBlock(ExpandedModel* emrow)
{
	ostringstream oss;
	oss<<GV(logdir)<<emrow->qualifiedName()<<".fullobjblk";
	ofstream out(oss.str().c_str());
	string objname = emrow->model->obj_comp==NULL? "null":emrow->model->obj_comp->name;
	out<<"Objective Block - (Assume to be Partial)"<<endl;
	out<<"Objective Declared/Row Model: "<<emrow->name<<"\t\t Name:"<<objname<<endl;
	out<<"Variables Declared/Col Model: "<<emrow->name<<"\t\t Size:"<<emrow->numLocalVars<<endl;
	out<<endl;
	out<<endl<<"Local Variables:"<<endl;
	vector<string> conNames, varNames;
	std::vector<VarComp*>::iterator varcomp=emrow->model->var_comps.begin();
	for(;varcomp!=emrow->model->var_comps.end();varcomp++)
	{
		Var* var = static_cast<Var*>(emrow->ctx.getCompValue(*varcomp));
		var_multi_map_by_order& var_by_order = var->varMultiMap.get<0>();
		var_multi_map_by_order::iterator ivar = var_by_order.begin();
		for(;ivar!=var_by_order.end();ivar++)
		{
			string name = emrow->name+"_"+var->name+"_"+(*ivar)->indicies;
			out<<"\t"<<name<<"\t\t"<<(*ivar)->toString()<<endl;
		}
	}

	out<<endl<<"Object Constraint"<<endl;
	string node_expr = this->objective==NULL?"NULL":texpr(this->objective);
	out<<"\t"<<objname<<"\t\t"<<node_expr<<endl;

	//printing dependent block information
	out<<endl;
	out.flush();
}


void BlockObj::logBlock(ExpandedModel* emrow,ExpandedModel* emcol)
{
	ostringstream oss;
	oss<<GV(logdir)<<emrow->qualifiedName()<<"_"<<emcol->qualifiedName()<<".objblk";
	ofstream out(oss.str().c_str());
	string objname = emrow->model->obj_comp==NULL? "null":emrow->model->obj_comp->name;
	out<<"Objective Block - (Assume to be Partial)"<<endl;
	out<<"Objective Declared/Row Model: "<<emrow->name<<"\t\t Name:"<<objname<<endl;
	out<<"Variables Declared/Col Model: "<<emrow->name<<"\t\t Size:"<<emrow->numLocalVars<<endl;
	out<<endl;
	out<<endl<<"Local Variables:"<<endl;
	vector<string> conNames, varNames;
	std::vector<VarComp*>::iterator varcomp=emrow->model->var_comps.begin();
	for(;varcomp!=emrow->model->var_comps.end();varcomp++)
	{
		Var* var = static_cast<Var*>(emrow->ctx.getCompValue(*varcomp));
		var_multi_map_by_order& var_by_order = var->varMultiMap.get<0>();
		var_multi_map_by_order::iterator ivar = var_by_order.begin();
		for(;ivar!=var_by_order.end();ivar++)
		{
			string name = emrow->name+"_"+var->name+"_"+(*ivar)->indicies;
			out<<"\t"<<name<<"\t\t"<<(*ivar)->toString()<<endl;
		}
	}

	out<<endl<<"Object Constraint"<<endl;
	string node_expr = this->objective==NULL?"NULL":texpr(this->objective);
	out<<"\t"<<objname<<"\t\t"<<node_expr<<endl;

	//printing dependent block information
	out<<endl;
	out.flush();
}
