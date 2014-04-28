/*
 * ParamComp.cpp
 *
 *  Created on: 7 Jan 2014
 *      Author: s0965328
 */

#include <typeinfo>
#include "ParamComp.h"
#include "SyntaxNode.h"
#include "SetComp.h"
#include "../context/ParamSingle.h"
#include "../context/ParamMult.h"
#include "../context/PValueSym.h"
#include "../context/PValueValue.h"
#include "../parser/sml.tab.h"


ParamComp::ParamComp(const string& id, SyntaxNode* index, SyntaxNode* attr,AmplModel* owner): ModelComp(id,TPARAM,index,attr,owner),numIndicies(-1)
{
	this->isSym = false;
	if(this->attributes != NULL)
	{
		this->isSym = this->attributes->isParamSymbolic();
	}
}

ParamComp::~ParamComp() {

}

// param related operations
void ParamComp::setParamIndicies() {
	assert(this->type==TPARAM);
	this->numIndicies = 0;
	if (this->indexing != NULL) {
		this->numIndicies = this->indexing->calculateParamSetIndicies();
	}
	assert(this->numIndicies>=0);
}

void ParamComp::calculateParamModelComp(ModelContext* context) {

	assert(this->type==TPARAM);
	vector<string> paramIndiciesDummy; //belong to param  dummay->Set*
	vector<SetComp*> paramIndiciesComp;
	CompDescr* param = NULL;
	if (this->indexing != NULL) {
		SyntaxNode* ind = this->indexing;
		ind->calculateParamIndicies(paramIndiciesDummy, paramIndiciesComp);
		assert(paramIndiciesDummy.size()==paramIndiciesComp.size());

		vector<Set*> sets;
		for(uint i=0;i<paramIndiciesComp.size();i++)
		{
			Set* aSet = static_cast<Set*>(context->getCompValue(paramIndiciesComp[i]));
			sets.push_back(aSet);
		}

		param = new ParamMult(this);
		this->calculateParamModelComp(context,paramIndiciesDummy,paramIndiciesComp,sets,0,static_cast<ParamMult*>(param));
	}
	else
	{
		param = new ParamSingle(this);
		PValue* pval = NULL;
		this->evalParamValue(context,&pval);
		static_cast<ParamSingle*>(param)->setParamValue(pval);
	}
	context->addCompValueMap(this, param);
}

void ParamComp::calculateParamModelComp(ModelContext* context,vector<string>& dumVars,vector<SetComp*>& comps,vector<Set*>& sets, int curr, ParamMult* param)
{
	if(curr == sets.size())
	{
		PValue* pval = NULL;
		this->evalParamValue(context,&pval);
		ostringstream oss;
		for(vector<string>::iterator it = dumVars.begin();it!=dumVars.end();it++)
		{
			oss<<context->getDummyValue(*it);
		}
		param->addParamValue(oss.str(),pval);
	}
	else
	{
		vector<string>::iterator val = sets.at(curr)->setValues_data_order.begin();
		for(;val!=sets.at(curr)->setValues_data_order.end();val++)
		{
			string value = *val;
			context->addDummyCompValueMapTemp(dumVars[curr],comps[curr],value);
			this->calculateParamModelComp(context,dumVars,comps,sets,curr+1,param);
			context->removeDummySetValueMapTemp(dumVars[curr]);
		}
	}
}

void ParamComp::evalParamValue(ModelContext* ctx,PValue** rval)
{
	assert(*rval == NULL);
	assert(this->attributes->opCode == COMMA);
	SyntaxNode* define = NULL;
	for(SyntaxNode::iterator i=this->attributes->begin();i!=this->attributes->end();i++)
	{
		if((*i)->opCode == DEFINED)
		{
			define = *i;
		}
	}
	assert(define->nchild()==1);
	define->values[0]->evalTerm(ctx,rval);

	//error type check!
	assert(*rval!=NULL);
	if(isSym)
	{
		assert(typeid(**rval)==typeid(PValueSym));
	}
	else
	{
		assert(typeid(**rval)==typeid(PValueValue));
	}
}


/* ---------------------------------------------------------------------------
 ParamComp::dump(ostream &fout)
 ---------------------------------------------------------------------------- */
void ParamComp::dump(ostream& fout,int counter)
{
	fout << "ParamComp:  counter ("<<counter<<")\n";
	fout << "\t" << name << " (" << (void *) this << ")\n";
	if (attributes) {
		fout <<"\t"<<"attr: " << attributes << "\n";
	}
	if (indexing) {
		fout <<"\t"<<"indexing: " << indexing << "\n";
	}
}
