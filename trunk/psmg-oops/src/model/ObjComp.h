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
	boost::unordered_map<int,SyntaxNode*> partial;

	ObjComp(const string& id, objType t, SyntaxNode* attr);
	virtual ~ObjComp();

	void calculatePartialConstraints();

	void dump(ostream& fout,int counter);
};

#endif /* OBJCOMP_H_ */
