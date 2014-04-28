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

VarComp::VarComp(const string& id, SyntaxNode* index, SyntaxNode* attr, AmplModel* owner) : ModelComp(id,TVAR,index,attr,owner),
	dim(0), isDet(false)
{

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
	if(this->indexing!=NULL)
	{
		IndexSet* iset = this->indexing->createIndexSet(ctx);
		assert(iset->dummyCompMap.size()<=2);
		if(iset->dummyCompMap.size()==1)
		{
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
				VarSingle* varsingle = new VarSingle(*it,upper,lower,v);
				var->varMultiMap.push_back(varsingle);
				ctx->removeDummySetValueMapTemp(dummy);
			}
		}
		else if(iset->dummyCompMap.size()==2)
		{
			boost::unordered_map<string,SetComp*>::iterator it1 = iset->dummyCompMap.begin();
			boost::unordered_map<string,Set*>::iterator it2 = iset->dummySetMap.begin();
			string dummy1 = it1->first;
			SetComp* comp1 = it1->second;it1++;
			string dummy2 = it1->first;
			SetComp* comp2 = it1->second;
			Set* set1 = it2->second; it2++;
			Set* set2 = it2->second;
			vector<string>::iterator i = set1->setValues_data_order.begin();
			for(;i!=set1->setValues_data_order.end();i++)
			{
				ctx->addDummyCompValueMapTemp(dummy1,comp1,*i);
				vector<string>::iterator j = set2->setValues_data_order.begin();
				for(;j!=set2->setValues_data_order.end();j++)
				{
					ctx->addDummyCompValueMapTemp(dummy2,comp2,*j);
					//compute variable bounds
					double upper = INFINITY_D;
					double lower = NEG_INFINITY_D;
					if(this->attributes != NULL){
						this->attributes->calculateVarBounds(ctx,upper,lower);
					}
					AutoDiff::Node* v =  AutoDiff::create_var_node(1.0);
					string varkey = *i + *j;
					VarSingle* varsingle = new VarSingle(varkey,upper,lower,v);
					var->varMultiMap.push_back(varsingle);
					ctx->removeDummySetValueMapTemp(dummy2);
				}
				ctx->removeDummySetValueMapTemp(dummy1);
			}

		}
		else
		{
			LOG("only support upto 2 indexed variables declaration!");
			assert(false);
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
		AutoDiff::Node* v =  AutoDiff::create_var_node(1.0);
		VarSingle* varsingle = new VarSingle(varkey,upper,lower,v);
		var->varMultiMap.push_back(varsingle);
	}

	ctx->addCompValueMap(this,var);
	LOG("calculateVarComp -- model["<<this->model->name<<"] comp["<<this->name<<"] card["<<var->varMultiMap.size()<<"] dim["<<this->dim<<"]");
}

/* ---------------------------------------------------------------------------
	 VarComp::dump(ostream &fout)
	 ---------------------------------------------------------------------------- */
void VarComp::dump(ostream& fout,int counter)
{
	fout << "VarComp:  counter ("<<counter<<")\n";
	fout << "\t" << name << " (" << (void *) this << ")\n";
	if (attributes) {
		fout << "\t"<<"attr: " << attributes << '\n';
	}
	if (indexing) {
		fout << "\t"<<"indexing: " << indexing << "\n";
	}
}
