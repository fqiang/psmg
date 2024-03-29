/*
 * SetComp.h
 *
 *  Created on: 7 Jan 2014
 *      Author: s0965328
 */

#ifndef SETCOMP_H_
#define SETCOMP_H_

#include "ModelComp.h"

class ModelContext;

class SetComp : public ModelComp {
public:
	SetComp(const string& id, SyntaxNode* index, SyntaxNode* attr, AmplModel* owner);
	virtual ~SetComp();

	int setDim; //belong to set
	bool isOrdered;

	void setSetDim();
	void calculateSetModelComp(ModelContext& context);
	void dump(ostream& fout,int counter);
	void calculateMemoryUsage(ulong& size);
};

#endif /* SETCOMP_H_ */
