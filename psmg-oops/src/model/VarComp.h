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
	VarComp(const string& id, SyntaxNode* index, SyntaxNode* attr);
	virtual ~VarComp();

	uint dim; //number of declared indicies
	//bug - varcomp can't has card- because varcomp in difference instance(expanededmodel) can have
	//different size depending on the indexing set.
//	uint card; //number of variables for this declared varcomp
	void calculateVarComp(ModelContext* ctx);
	void calculateVarDimCard(ModelContext* context, uint& card);
//	void fillLocalVarRecurive(ModelContext* context,Var* aVar,vector<ModelComp*>::iterator it,ostringstream& oss);


	void dump(ostream& fout,int counter);

};

#endif /* VARCOMP_H_ */
