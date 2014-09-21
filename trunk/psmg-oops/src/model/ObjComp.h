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

using namespace boost;
class ObjComp : public ModelComp {
public:

	objType otype;

	//This will be used by QP problem. the first order term will be in first_partial, and corresponding c coefficient for either
	//LP or QP problem will need to specified in it's own(local) block.
	//The higher order term (quadratic term for QP) will be in higher_partial and store separately for each different column level block.
	CPart cpart;
	unordered_map<int,SyntaxNode*> const_partial;
	unordered_map<int,SyntaxNode*> first_partial;
	unordered_map<int,SyntaxNode*> higher_partial;
//	unordered_map<int,SyntaxNode*> full_partial;

//	unordered_map<int,SyntaxNode*> partial;

	ObjComp(const string& id, objType t, SyntaxNode* attr,AmplModel* owner);
	virtual ~ObjComp();

	void calculatePartialConstraints();

	void dump(ostream& fout,int counter);
	void calculateMemoryUsage(ulong& size);
};

#endif /* OBJCOMP_H_ */
