/*
 * ConsComp.cpp
 *
 *  Created on: 7 Jan 2014
 *      Author: s0965328
 */

#include <boost/foreach.hpp>
#include "ConsComp.h"
#include "SyntaxNode.h"
#include "AmplModel.h"

//always include sml.tab.h at last
#include "../parser/sml.tab.h"

ConsComp::ConsComp(const string& id, SyntaxNode* index, SyntaxNode* attr,AmplModel* owner): ModelComp(id,TCON,index, attr,owner)
	,card(0)
{

}

ConsComp::~ConsComp() {

}

void ConsComp::calculateLocalCon(ModelContext* ctx)
{
	LOG("calculateLocalCon -- in model["<<this->model->name<<"] id["<<this->name<<"]");
	this->card = 1;
	if(this->indexing!=NULL){ //a single constraint
		this->indexing->calculateConCard(ctx,this->card);
	}
	LOG( "calculateLocalVar -- in model["<<this->model->name<<"] id["<<this->name<<"] -- card["<<card<<"]");
}

void ConsComp::moveConsToLeft()
{
	LOG("ConsComp::moveConsToLeft --name["<<name<< "] - indx["<<this->indexing->print()<<"]-- attr["<<this->attributes->print()<<"] - declared level["<<this->model->level<<"]");
	assert(this->attributes->opCode == COMMA);
	int i = 0;
	BOOST_FOREACH(SyntaxNode* attr, this->attributes->values)
	{
		if(attr->opCode == ASSIGN){
			SyntaxNode* ret = this->attributes->moveConsToLeft();
			this->attributes->values.at(i) = ret;
		}
		i++;
	}
}

void ConsComp::calculatePartialConstraints()
{
	LOG("ConsComp::calculatePartialConstraints --id["<<name<< "] - indx["<<this->indexing->print()<<"]-- attr["<<this->attributes->print()<<"] - declared level["<<this->model->level<<"]");
	this->attributes->calculatePartialConstraints(this->partial);

	boost::unordered_map<int,SyntaxNode*>::iterator it = this->partial.begin();
	for(;it!=this->partial.end();it++)
	{
		SyntaxNode* node = (*it).second;
		LOG("level ["<<(*it).first<<"] - ["<<node->print()<<"]");
	}
}


/* ---------------------------------------------------------------------------
 ConsComp::dump(ostream &fout)
 ---------------------------------------------------------------------------- */
void ConsComp::dump(ostream& fout,int counter)
{
	fout << "ConsComp:  ("<<counter<<")\n";
	fout << "\t" << name << " (" << (void *) this << ")\n";
	if (attributes) {
		fout <<"\t"<<"attr: " << attributes << '\n';
	}
	if (indexing) {
		fout<<"\t"<<"indexing: " << indexing << "\n";
	}
}
