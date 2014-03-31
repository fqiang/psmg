/*
 * SetComp.cpp
 *
 *  Created on: 7 Jan 2014
 *      Author: s0965328
 */

#include "SetComp.h"
#include "SyntaxNode.h"

#include "ObjComp.h"
#include "../parser/sml.tab.h"

SetComp::SetComp(const string& id, SyntaxNode* index, SyntaxNode* attr):ModelComp(id, TSET, index,attr),
	setDim(0),setCard(0), isOrdered(false)
{


}

SetComp::~SetComp() {
}


/* ---------------------------------------------------------------------------
 SetComp::dump(ostream &fout)
 ---------------------------------------------------------------------------- */
void SetComp::dump(ostream& fout,int counter)
{
	fout << "SetComp:  ("<<counter<<"------------------------------------------------------\n";
	fout << "SetComp: " << name << " (" << (void *) this << ")\n";
	if (attributes) {
		fout << "    attr: " << attributes << '\n';
	}
	if (indexing) {
		fout << "    indexing: " << indexing << "\n";
	}
}

/***************************************************************************************
 Set related operation evaluation
 Feng
 ***************************************************************************************/
void SetComp::setSetDim() {
	assert(this->type == TSET);
	assert(this->indexing==NULL); //set has no indexing

	SyntaxNode* assign = NULL;
	SyntaxNode* within = NULL;
	if (this->attributes != NULL) {
		assert(this->attributes->opCode == COMMA);
		SyntaxNode::iterator  it = this->attributes->begin();
		for(;it!=this->attributes->end();it++)
		{
			if((*it)->opCode == ASSIGN)
			{
				assign = *it;
			}
			else if((*it)->opCode == ORDERED)
			{
				isOrdered = true;
			}
			else if((*it)->opCode == WITHIN)
			{
				within = *it;
			}
		}
	}

	if(within != NULL || assign != NULL)
	{
		setDim = within!=NULL? within->calculateSetDim(): assign->calculateSetDim();
	}
	else //no assign or with attribute, then it should just be a simple 1-index set
	{
		setDim = 1; //default set has 1-index vs. default for param is 0;
	}

	assert(setDim!=0);
}

void SetComp::calculateSetModelComp(ModelContext* context) {
	assert(this->type==TSET);
	assert(this->indexing == NULL);
	//set define may not have a indexing
	assert(this->attributes != NULL);

	assert(this->attributes->opCode == COMMA);
	Set* theSet = Set::createSet(this);
	SyntaxNode* within = NULL;
	SyntaxNode* ordered = NULL;
	for(SyntaxNode::iterator i=this->attributes->begin();i!=this->attributes->end();i++)
	{
		if((*i)->opCode == ASSIGN){
			assert((*i)->nchild() == 1);
			(*i)->values[0]->calculateSetValue(context,theSet);
		}
		else if((*i)->opCode == WITHIN){
			LOG("WITHIN ... "<<this->attributes);
			within = *i;
		}
		else if((*i)->opCode == ORDERED)
		{
			assert(this->isOrdered == false); //not support creating ordered set
			ordered = *i;
		}
	}

	LOG( "the calculated set is   -- ["<<theSet->name<<"]  -- "<<theSet->toString());
	this->setCard = theSet->card;
	assert(theSet->dim==this->setDim);
	theSet->name = this->name;
	theSet->dim = this->setDim;
	context->addCompValueMap(this, theSet);
}
