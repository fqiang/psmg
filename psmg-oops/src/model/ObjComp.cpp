/*
 * ObjComp.cpp
 *
 *  Created on: 7 Jan 2014
 *      Author: s0965328
 */

#include "ObjComp.h"
#include "SyntaxNode.h"
#include "AmplModel.h"
#include "../util/global_util_functions.h"

ObjComp::ObjComp(const string& id, objType t, SyntaxNode* attr,AmplModel* owner): ModelComp(id,TOBJ,NULL,attr,owner),otype(t)
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
	fout << "ObjComp:  ("<<counter<<")\n";
	fout << "\t"<< name << " (" << (void *) this << ")"<<endl;
	fout << "\t"<< otype<<endl;
	if (attributes) {
		fout <<"\t"<<"attr: " << attributes << '\n';
	}
	assert(indexing== NULL);
}
