/*
 * ParamComp.h
 *
 *  Created on: 7 Jan 2014
 *      Author: s0965328
 */

#ifndef PARAMCOMP_H_
#define PARAMCOMP_H_

#include "ModelComp.h"
#include "../context/ModelContext.h"
#include "../context/ParamMult.h"
#include "../context/ParamSingle.h"

class SetComp;

class ParamComp : public ModelComp {
public:
	ParamComp(const string& id, SyntaxNode* index, SyntaxNode* attr);
	virtual ~ParamComp();

	int numIndicies;
	bool isSym;
	void setParamIndicies();
	int getNumSetIndices();
	void calculateParamModelComp(ModelContext* context);
	void calculateParamModelComp(ModelContext* context,
				vector<string>& dumVars,vector<SetComp*>& comps,vector<Set*>& sets, int curr, ParamMult* param);
	void evalParamValue(ModelContext* ctx,PValue** rval);
	void dump(ostream& fout,int counter);
};

#endif /* PARAMCOMP_H_ */
