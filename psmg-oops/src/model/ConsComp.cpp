/*
 * ConsComp.cpp
 *
 *  Created on: 7 Jan 2014
 *      Author: s0965328
 */

#include "ConsComp.h"

#include <boost/foreach.hpp>
#include "../util/util.h"
#include "SyntaxNode.h"
#include "AmplModel.h"

//always include sml.tab.h at last
#include "../parser/sml.tab.h"

ConsComp::ConsComp(const string& id, SyntaxNode* index, SyntaxNode* attr,AmplModel* owner): ModelComp(id,TCON,index, attr,owner),cpart()
{

}

ConsComp::~ConsComp() {

}

void ConsComp::calculateLocalConCard(ModelContext& ctx,uint& card)
{
	TRACE("calculateLocalCon -- in model["<< (this->model->name) <<"] id["<<this->name<<"]");
	card = 1;
	if(this->indexing!=NULL){ //a single constraint
		this->indexing->calculateConCard(ctx,card);
	}
	TRACE( "calculateLocalVar -- in model["<<this->model->name<<"] id["<<this->name<<"] -- card["<<card<<"]");
}


void ConsComp::calculatePartialConstraints()
{
	TRACE("ConsComp::calculatePartialConstraints --id["<<name<< "] - indx["<<this->indexing<<"]-- attr["<<this->attributes<<"] - declared level["<<this->model->level<<"]");
	assert(attributes->opCode == COMMA);
	SyntaxNode* assign_expr = attributes->findDirectChild(ASSIGN);
	assert(assign_expr->nchild()==2); //TODO: for now- only equality constraint supported

	if(ExpandedModel::ptype == NLP) {
		assert(cpart.constant == NULL && cpart.first==NULL && cpart.higher==NULL);
		assign_expr->values[0]->calculateLinearNonLinearParts(cpart);
		assert(const_partial.size()==0 && first_partial.size() == 0 && higher_partial.size()==0);

		if(cpart.constant!=NULL) cpart.constant->calculatePartialConstraints(const_partial);
		if(cpart.first!=NULL) cpart.first->calculatePartialConstraints(first_partial);
		if(cpart.higher!=NULL) cpart.higher->calculatePartialConstraints(higher_partial);
	}

	assert(this->partial.size() == 0);
	assign_expr->values[0]->calculatePartialConstraints(this->partial);

	//sanity check! - for constraint formulation
	assert(partial.find(-1)==partial.end()); //TODO: for now - place all constant term on LHS
	assert(assign_expr->values.at(1)->isContainsIDREF_TVAR_in_child() == false);  //TODO: for now - place all variables term on RHS
}


/* ---------------------------------------------------------------------------
 ConsComp::dump(ostream &fout)
 ---------------------------------------------------------------------------- */
void ConsComp::dump(ostream& fout,int counter)
{
	fout << "ConsComp:  ("<<counter<<")\n";
	fout << "\t" << name << " (" << (void *) this << ")\n";
	if (indexing) {
		fout<<"\t"<<"indexing: " << indexing << "\n";
	}
	if (attributes) {
		fout <<"\t"<<"attr: " << attributes << '\n';
	}
	for(int i=-1;i<=AmplModel::MAX_LEVEL;i++)
	{
		boost::unordered_map<int,SyntaxNode*>::iterator it = partial.find(i);
		SyntaxNode* node = it==partial.end()? NULL:it->second;
		if(node==NULL) fout<<"\tlevel ["<<i<<"] \t"<<"null"<<"\n";
		else fout<<"\tlevel ["<<i<<"] \t"<<node<<"\n";
	}
	if(cpart.constant == NULL)	fout<<"\tconst : "<<"null"<<endl;
		else	fout<<"\tconst : "<<cpart.constant<<endl;
	if(cpart.first == NULL)	fout<<"\tfirst : "<<"null"<<endl;
		else	fout<<"\tfirst : "<<cpart.first<<endl;
	if(cpart.higher == NULL)	fout<<"\thigher : "<<"null"<<endl;
		else	fout<<"\thigher : "<<cpart.higher<<endl;
}

void ConsComp::calculateMemoryUsage(ulong& size)
{
	ModelComp::calculateMemoryUsage(size);
	size += sizeof(ConsComp);
	if(cpart.constant!=NULL)
		cpart.constant->calculateMemoryUsage(size);
	if(cpart.first!=NULL)
		cpart.first->calculateMemoryUsage(size);
	if(cpart.higher!=NULL)
		cpart.higher->calculateMemoryUsage(size);
}
