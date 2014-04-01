/*
 * Context.h
 *
 *  Created on: 10 Oct 2011
 *      Author: s0965328
 */

#ifndef MODELCONTEXT_H_
#define MODELCONTEXT_H_

#include "../util/global_util_functions.h"
#include "VarSingle.h"
#include "Set.h"
#include "autodiff.h"
#include "IndexSet.h"
#include <boost/unordered_map.hpp>
#include <string>
#include <vector>

using namespace std;

class ExpandedModel;

class ModelContext {

public:
	ModelContext* parent;
	ExpandedModel* em;

	int moveUpLevel;

	//! data storage for this expanded model tree node. key is ModelComp*
	boost::unordered_map<ModelComp*,CompDescr*> compValueMap;
	boost::unordered_map<string,ModelComp*> dummyCompMapTemp;
	boost::unordered_map<string,string> dummyValueMapTemp;

	//! temporary IndexSet storage - mainly for sum{...} node
	boost::unordered_map<SyntaxNode*,IndexSet*> tempISetMap;

	ModelContext(ModelContext* par);
	virtual ~ModelContext();

	void addDummyCompValueMapTemp(string& dummyVar,ModelComp* comp,string& val);
	void removeDummySetValueMapTemp(string& dummyVar);
	string getDummyValue(string& dummyVar);
	string getDummyValueTemp(string& dummyVar);
	ModelComp* getDummyComp(string& dummyVar);
	CompDescr* getCompValue(ModelComp* comp);
	void addCompValueMap(ModelComp* comp,CompDescr* value);
	string getContextId();
	void fillDummyValue(vector<string>&);
	void fillDummyValue(ostringstream&);


	bool getCalcSumSet(SyntaxNode*,IndexSet** iset);
	void addCalcSumSet(SyntaxNode*,IndexSet* iset);

	void calculateMemoryUsage(unsigned long& size);
};

#endif /* MODELCONTEXT_H_ */



//	void removeCurrLevelCompValueMap(ModelComp* comp);
//	void addDummySetValueMap(string& dummyVar,ModelComp* comp,string val);
//	bool getCalcTempSet(string& hashKey,vector<ModelComp*>& comps,Set** aSet,vector<string>& dummyVars);
//	void addCalcTempSet(string& hashKey,vector<ModelComp*>& comps,Set* aSet,vector<string>& dummyVars);
//	void addDummySetValueMapCons(string& dummyVar,ModelComp* comp,string val);
//	void removeDummySetValueMapCons(string& dummyVar);
//	string getConsDummyValAsKey();
//	string getModelDummyValAsKey(int& num);
//	string getDummyValueCons(string& dummyVar);
