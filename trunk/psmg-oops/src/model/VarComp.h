/*
 * VarComp.h
 *
 *  Created on: 7 Jan 2014
 *      Author: s0965328
 */

#ifndef VARCOMP_H_
#define VARCOMP_H_

#include "ModelComp.h"
#include "../context/ModelContext.h"

class VarComp : public ModelComp {
public:
	VarComp(const string& id, SyntaxNode* index, SyntaxNode* attr, AmplModel* owner);
	virtual ~VarComp();

	uint dim; //number of declared indicies

	//stochcastic only -- better implementation to use inheritance
	bool isDet;

	//bug - varcomp can't has card- because varcomp in difference instance(expanededmodel) can have
	//different size depending on the indexing set.
//	uint card; //number of variables for this declared varcomp

	void calculateVarComp(ModelContext& ctx);
	void calculateVarDimCard(ModelContext& context, uint& card);
	void dump(ostream& fout,int counter);
	void calculateMemoryUsage(ulong& size);
};

#endif /* VARCOMP_H_ */
