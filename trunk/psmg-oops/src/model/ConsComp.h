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

	int card;
	boost::unordered_map<int,SyntaxNode*> partial;
	void calculatePartialConstraints();
	void moveConsToLeft();

	void calculateLocalCon(ModelContext* ctx);
	void dump(ostream& fout,int counter);
};

#endif /* CONSCOMP_H_ */
