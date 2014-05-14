/*
 * ObjComp.h
 *
 *  Created on: 7 Jan 2014
 *      Author: s0965328
 */

#ifndef OBJCOMP_H_
#define OBJCOMP_H_

#include <boost/unordered_map.hpp>
#include "ModelComp.h"

class ObjComp : public ModelComp {
public:

	objType otype;
	CPart cpart;
	boost::unordered_map<int,SyntaxNode*> const_partial;
	boost::unordered_map<int,SyntaxNode*> first_partial;
	boost::unordered_map<int,SyntaxNode*> higher_partial;

//	boost::unordered_map<int,SyntaxNode*> partial;

	ObjComp(const string& id, objType t, SyntaxNode* attr,AmplModel* owner);
	virtual ~ObjComp();

	void calculatePartialConstraints();

	void dump(ostream& fout,int counter);
};

#endif /* OBJCOMP_H_ */
