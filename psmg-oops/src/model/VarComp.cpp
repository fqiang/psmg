/*
 * VarComp.cpp
 *
 *  Created on: 7 Jan 2014
 *      Author: s0965328
 */

#include <typeinfo>
#include "VarComp.h"
#include "AmplModel.h"
#include "SyntaxNode.h"
#include "SyntaxNodeValue.h"
#include "../context/Var.h"
#include "../context/PValueValue.h"

#include "ObjComp.h"
#include "../parser/sml.tab.h"

VarComp::VarComp(const string& id, SyntaxNode* index, SyntaxNode* attr) : ModelComp(id,TVAR,index,attr),
	dim(0)
{
	// TODO Auto-generated constructor stub

}

VarComp::~VarComp() {
	// TODO Auto-generated destructor stub
}



void VarComp::calculateVarDimCard(ModelContext* context, uint& card) {
	LOG( "calculateLocalVar -- in model["<<this->model->name<<"] id["<<this->name<<"]");
	assert(this->type==TVAR);
	card = 1;
	this->dim = 0;
	if (this->indexing!=NULL) {
		this->indexing->calculateVarDimCard(context,this->dim,card);
	}
	LOG( "calculateLocalVar -- in model["<<this->model->name<<"] id["<<this->name<<"] -- varCard["<<card<<"] varDim["<<dim<<"]");
}

void VarComp::calculateVarComp(ModelContext* ctx)
{
	assert(this->type==TVAR);
	LOG("calculateVarComp - model["<<this->model->name<<"] comp["<<this->name<<"]");
	Var* var = new Var(this->name);
	IndexSet* iset = NULL;
	if(this->indexing!=NULL)
	{
		iset = this->indexing->createIndexSet(ctx);
		assert(iset->dummyCompMap.size()==1); //supports one -dim variables for now.
		string dummy = iset->dummyCompMap.begin()->first;
		SetComp* comp = iset->dummyCompMap.begin()->second;
		Set*	set = iset->dummySetMap.begin()->second;

		vector<string>::iterator it=set->setValues_data_order.begin();
		for(;it!=set->setValues_data_order.end();it++)
		{
			ctx->addDummyCompValueMapTemp(dummy,comp,*it);
			//compute variable bounds
			double upper = INFINITY_D;
			double lower = NEG_INFINITY_D;
			if(this->attributes != NULL){
				this->attributes->calculateVarBounds(ctx,upper,lower);
			}
			AutoDiff::Node* v =  AutoDiff::create_var_node(1.0);
			var->varMultiMap.push_back(VarSingle(*it,upper,lower,v));
			ctx->removeDummySetValueMapTemp(dummy);
		}
		delete iset;
	}
	else
	{
		double upper = INFINITY_D;
		double lower = NEG_INFINITY_D;
		if(this->attributes != NULL){
			this->attributes->calculateVarBounds(ctx,upper,lower);
		}
		string varkey = "";
		AutoDiff::Node* v =  AutoDiff::create_var_node();
		var->varMultiMap.push_back(VarSingle(varkey,upper,lower,v));
	}

	ctx->addCompValueMap(this,var);
	LOG( "fillLocalVar -- model["<<this->model->name<<"] comp["<<this->name<<"] card["<<var->varMultiMap.size()<<"] dim["<<this->dim<<"]");
}

/* ---------------------------------------------------------------------------
	 VarComp::dump(ostream &fout)
	 ---------------------------------------------------------------------------- */
void VarComp::dump(ostream& fout,int counter)
{
	fout << "VarComp:  ("<<counter<<"------------------------------------------------------\n";
	fout << "VarComp: " << name << " (" << (void *) this << ")\n";
	if (attributes) {
		fout << "    attr: " << attributes << '\n';
	}
	if (indexing) {
		fout << "    indexing: " << indexing << "\n";
	}
}

//void VarComp::fillLocalVarRecurive(ModelContext* context,Var* aVar,vector<ModelComp*>::iterator it, ostringstream& oss){
////	if(it==this->indexing->sets_mc.end())
////	{
////		aVar->addVarValue(oss);
////	}
////	else
////	{
////		Set* aSet = static_cast<Set*>(context->getCompValue(*it));
////		for(vector<string>::iterator it_setval = aSet->setValues_data_order.begin();it_setval != aSet->setValues_data_order.end();it_setval++) {
////			ostringstream copy(oss.str());
////			oss<<*it_setval;
////			this->fillLocalVarRecurive(context, aVar, it + 1, oss);
////			oss.str("");
////			oss.clear();
////			oss<<copy.str();
////		}
////	}
//}
