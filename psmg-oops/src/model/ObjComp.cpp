/*
 * ObjComp.cpp
 *
 *  Created on: 7 Jan 2014
 *      Author: s0965328
 */

#include "ObjComp.h"
#include "SyntaxNode.h"
#include "AmplModel.h"
#include "../util/CompilerMarco.h"
#include "../sml/GlobalVariables.h"

ObjComp::ObjComp(const string& id, objType t, SyntaxNode* attr): ModelComp(id,TOBJ,NULL,attr),otype(t)
{
}

ObjComp::~ObjComp() {
}

void ObjComp::calculatePartialConstraints()
{
	LOG("ObjComp::calculatePartialConstraints --id["<<name<< "] - indx["<<this->indexing->print()<<"]-- attr["<<this->attributes->print()<<"] - declared level["<<this->model->level<<"] - ["<<this<<"]");
	this->attributes->calculatePartialConstraints(this->partial);

	boost::unordered_map<int,SyntaxNode*>::iterator it = this->partial.begin();
	for(;it!=this->partial.end();it++)
	{
		SyntaxNode* node = (*it).second;
		LOG("level ["<<(*it).first<<"] - ["<<node->print()<<"]");
	}
}

/* ---------------------------------------------------------------------------
 ObjComp::dump(ostream &fout)
 ---------------------------------------------------------------------------- */
void ObjComp::dump(ostream& fout,int counter)
{
	fout << "ObjComp:  ("<<counter<<"------------------------------------------------------\n";
	fout << "ObjComp: " << name << " (" << (void *) this << ")"<<endl;
	fout << "ObjComp: " << type<<endl;
	fout << "ObjComp:"	<< otype<<endl;
	if (attributes) {
		fout << "    attr: " << attributes << '\n';
	}
	assert(indexing== NULL);
}