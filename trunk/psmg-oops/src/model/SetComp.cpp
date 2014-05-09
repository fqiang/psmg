/*
 * SetComp.cpp
 *
 *  Created on: 7 Jan 2014
 *      Author: s0965328
 */

#include <typeinfo>
#include "SetComp.h"
#include "../util/util.h"
#include "../context/ModelContext.h"
#include "../context/SetOrdered.h"
#include "SyntaxNode.h"
#include "ObjComp.h"

#include "../parser/sml.tab.h"

SetComp::SetComp(const string& id, SyntaxNode* index, SyntaxNode* attr,AmplModel* owner):ModelComp(id, TSET, index,attr,owner),
	setDim(0)
{
	isOrdered = false;
	if(this->attributes!=NULL)
	{
		for(SyntaxNode::iterator it=this->attributes->begin();it!=this->attributes->end();it++)
		{
			if((*it)->opCode == ORDERED)
			{
				isOrdered = true;
				break;
			}
		}
	}
}

SetComp::~SetComp() {
}


/* ---------------------------------------------------------------------------
 SetComp::dump(ostream &fout)
 ---------------------------------------------------------------------------- */
void SetComp::dump(ostream& fout,int counter)
{
	fout << "SetComp:  counter ("<<counter<<")\n";
	fout << "\t"<< name << " (" << (void *) this << ")\n";
	if (attributes) {
		fout << "\t"<<"attr: " << attributes << '\n';
	}
	if (indexing) {
		fout <<"\t"<<"indexing: " << indexing << "\n";
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
	assert(this->isOrdered == false || typeid(*theSet) == typeid(SetOrdered));
	SyntaxNode* within = NULL;
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
	}

	LOG( "the calculated set is   -- ["<<theSet->name<<"]  -- "<<theSet->toString());
	assert(theSet->dim==this->setDim);
	theSet->name = this->name;
	theSet->dim = this->setDim;
	context->addCompValueMap(this, theSet);
}
