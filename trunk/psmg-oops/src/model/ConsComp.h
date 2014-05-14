/*
 * ConsComp.h
 *
 *  Created on: 7 Jan 2014
 *      Author: s0965328
 */

#ifndef CONSCOMP_H_
#define CONSCOMP_H_

#include <boost/unordered_map.hpp>

#include "ModelComp.h"
#include "../context/ModelContext.h"

class SyntaxNode;

class ConsComp : public ModelComp {
public:

	ConsComp(const string& id, SyntaxNode* index, SyntaxNode* attr,AmplModel* owner);
	virtual ~ConsComp();

//	int card;
	CPart cpart;
//	boost::unordered_map<int,SyntaxNode*> const_partial;
//	boost::unordered_map<int,SyntaxNode*> first_partial;
//	boost::unordered_map<int,SyntaxNode*> higher_partial;

	boost::unordered_map<int,SyntaxNode*> partial;
	void calculatePartialConstraints();

	void calculateLocalConCard(ModelContext* ctx, uint& card);
	void dump(ostream& fout,int counter);
};

#endif /* CONSCOMP_H_ */
