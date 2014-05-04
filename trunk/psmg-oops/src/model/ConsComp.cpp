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


void ConsComp::calculatePartialConstraints()
{
	LOG("ConsComp::calculatePartialConstraints --id["<<name<< "] - indx["<<this->indexing->print()<<"]-- attr["<<this->attributes->print()<<"] - declared level["<<this->model->level<<"]");
	assert(attributes->opCode == COMMA);
	SyntaxNode* assign_expr = attributes->findChildNode(ASSIGN);
	assert(assign_expr->nchild()==2);
	assert(this->partial.size() == 0);

	assign_expr->values.at(0)->calculatePartialConstraints(this->partial);
	assert(partial.find(-1)==partial.end()); //TODO: for now - place all constant term on LHS
	assert(assign_expr->values.at(1)->isContainsIDREF_TVAR_in_child() == false);  //TODO: for now - place all variables term on RHS
//	boost::unordered_map<int,SyntaxNode*> left;
//	boost::unordered_map<int,SyntaxNode*> right;
//	this->attributes[1]->calculatePartialConstraints(right);
//	for(int i=-1;i<=AmplModel::MAX_LEVEL;i++)
//	{
//		boost::unordered_map<int,SyntaxNode*>::iterator it_left = left.find(i);
//		boost::unordered_map<int,SyntaxNode*>::iterator it_right = right.find(i);
//		if(it_left!=left.end() && it_right!=right.end())
//		{//left - right
//			SyntaxNode* newnode = new SyntaxNodeOP(MINUS, it_left->second,it_right->second);
//			this->partial.insert(pair<int,SyntaxNode*>(i,newnode));
//		}
//		else if(it_left==left.end() && it_right != right.end())
//		{//null - right
//			SyntaxNode* newnode = new SyntaxNodeOP(MINUS,it_right->second);
//			this->partial.insert(pair<int,SyntaxNode*>(i,newnode));
//		}
//		else if(it_left!=left.end() && it_right == right.end())
//		{//left - null
//			this->partial.insert(pair<int,SyntaxNode*>(i,it_left->second));
//		}
//		else //both are null
//		{
//			//do nothing
//		}
//	}
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
}
