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

//always include sml.tab.h at last
#include "../parser/sml.tab.h"

ObjComp::ObjComp(const string& id, objType t, SyntaxNode* attr,AmplModel* owner): ModelComp(id,TOBJ,NULL,attr,owner),otype(t)
{
}

ObjComp::~ObjComp() {
}

void ObjComp::calculatePartialConstraints()
{
	LOG("ObjComp::calculatePartialConstraints --id["<<name<< "] - indx["<<this->indexing<<"]-- attr["<<this->attributes->print()<<"] - declared level["<<this->model->level<<"] - ["<<this<<"]");
	assert(indexing==NULL);

	assert(this->partial.size() == 0);

	attributes->calculatePartialConstraints(this->partial);
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
	for(int i=-1;i<=AmplModel::MAX_LEVEL;i++)
	{
		boost::unordered_map<int,SyntaxNode*>::iterator it = partial.find(i);
		SyntaxNode* node = it==partial.end()? NULL:it->second;
		if(node==NULL) fout<<"\t"<<"level ["<<i<<"]\t"<<"null"<<"\n";
		else fout<<"\t"<<"level ["<<i<<"]\t"<<node<<"\n";
	}
}
