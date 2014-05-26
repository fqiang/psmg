/*
 * ParamComp.h
 *
 *  Created on: 7 Jan 2014
 *      Author: s0965328
 */

#ifndef PARAMCOMP_H_
#define PARAMCOMP_H_

#include "ModelComp.h"

class SetComp;

class ModelContext;
class ParamMult;
class Set;
class PValue;

class ParamComp : public ModelComp {
public:
	ParamComp(const string& id, SyntaxNode* index, SyntaxNode* attr, AmplModel* owner);
	virtual ~ParamComp();

	int numIndicies;
	bool isSym;
	void setParamIndicies();
	void calculateParamModelComp(ModelContext& context);
	void calculateParamModelComp(ModelContext& context,
				vector<string>& dumVars,vector<SetComp*>& comps,vector<Set*>& sets, uint curr, ParamMult* param);
	void evalParamValue(ModelContext& ctx,PValue** rval);
	void dump(ostream& fout,int counter);
	void calculateMemoryUsage(ulong& size);
};

#endif /* PARAMCOMP_H_ */
