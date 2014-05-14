/*
 * ObjComp.cpp
 *
 *  Created on: 7 Jan 2014
 *      Author: s0965328
 */

#include "ObjComp.h"
#include "SyntaxNode.h"
#include "AmplModel.h"
#include "../util/util.h"

//always include sml.tab.h at last
#include "../parser/sml.tab.h"

ObjComp::ObjComp(const string& id, objType t, SyntaxNode* attr,AmplModel* owner): ModelComp(id,TOBJ,NULL,attr,owner),otype(t),cpart()
{
}

ObjComp::~ObjComp() {
}

void ObjComp::calculatePartialConstraints()
{
	LOG("ObjComp::calculatePartialConstraints --id["<<name<< "] - indx["<<this->indexing<<"]-- attr["<<this->attributes->print()<<"] - declared level["<<this->model->level<<"] - ["<<this<<"]");
	assert(indexing==NULL);

	assert(cpart.constant == NULL && cpart.first==NULL && cpart.higher==NULL);
	attributes->calculateLinearNonLinearParts(cpart);

	assert(const_partial.size()==0 && first_partial.size() == 0 && higher_partial.size()==0);
	if(cpart.constant!=NULL) cpart.constant->calculatePartialConstraints(const_partial);
	if(cpart.first!=NULL) cpart.first->calculatePartialConstraints(first_partial);
	if(cpart.higher!=NULL) cpart.higher->calculatePartialConstraints(higher_partial);

	//validation of the objective
	// this is for LP and QP problem
	// LP and QP , we will need to compute the c vector (the coefficient of the linear part of the objective)
	// for NLP, this part doesn't contribute to HV.
	assert(first_partial.size() == 1 && first_partial.begin()->first==model->level); //assuming first order term of the objective only contains the varaible declared in it's own level

//	assert(this->partial.size() == 0);
//	attributes->calculatePartialConstraints(this->partial);
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
//	for(int i=-1;i<=AmplModel::MAX_LEVEL;i++)
//	{
//		boost::unordered_map<int,SyntaxNode*>::iterator it = partial.find(i);
//		SyntaxNode* node = it==partial.end()? NULL:it->second;
//		if(node==NULL) fout<<"\t"<<"level ["<<i<<"]\t"<<"null"<<"\n";
//		else fout<<"\t"<<"level ["<<i<<"]\t"<<node<<"\n";
//	}
	if(cpart.constant == NULL)	fout<<"\tconst : "<<"null"<<endl;
		else	fout<<"\tconst : "<<cpart.constant<<endl;
	if(cpart.first == NULL)	fout<<"\tfirst : "<<"null"<<endl;
		else	fout<<"\tfirst : "<<cpart.first<<endl;
	if(cpart.higher == NULL)	fout<<"\thigher : "<<"null"<<endl;
		else	fout<<"\thigher : "<<cpart.higher<<endl;
}
