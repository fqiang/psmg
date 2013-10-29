/*
 * Context.h
 *
 *  Created on: 10 Oct 2011
 *      Author: s0965328
 */

#ifndef CONTEXT_H_
#define CONTEXT_H_

#include "../util/global_util_functions.h"
#include "Var.h"
#include "Set.h"
#include "IndexSet.h"
#include <ext/hash_map>
#include <string>
#include <vector>
#include <list>

using namespace std;

class ModelComp;
class ExpandedModel;

class ModelContext {

public:
	ModelContext* parent;
	ExpandedModel* em2;
	int moveUpLevel;
	bool isCompsCalculated;

	//! data storage for this expanded model tree node. key is ModelComp*
	__gnu_cxx::hash_map<string,CompDescr*> compValueMap;

	__gnu_cxx::hash_map<string,ModelComp*> dummySetMapCons;
	__gnu_cxx::hash_map<string,string> dummyValueMapCons;

	__gnu_cxx::hash_map<string,ModelComp*> dummySetMapTemp;
	__gnu_cxx::hash_map<string,string> dummyValueMapTemp;


	__gnu_cxx::hash_map<int,string> cacheModelDummyVarKey; //-- used in the getModelDummyValAsKey(int& num);

	__gnu_cxx::hash_map<string,IndexSet*> tempISetMap;


	list<string> localConNames;
	list<string> localVarNames;
	bool conNameReady;
	bool varNameReady;

	ModelContext(ModelContext* par);
	virtual ~ModelContext();

	void addDummySetValueMapCons(string& dummyVar,ModelComp* comp,string val);
	void addDummySetValueMapTemp(string& dummyVar,ModelComp* comp,string val);
	void removeDummySetValueMapCons(string& dummyVar);
	void removeDummySetValueMapTemp(string& dummyVar);
	string& getDummyValue(string& dummyVar);
	string getDummyValueCons(string& dummyVar);
	string getDummyValueTemp(string& dummyVar);
	ModelComp* getDummySet(string& dummyVar);
	CompDescr* getCompValue(ModelComp* comp);
	string getModelDummyValAsKey(int& num);
	string getConsDummyValAsKey();
	void addCompValueMap(ModelComp* comp,CompDescr* value);
	void removeCurrLevelCompValueMap(ModelComp* comp);
	string getContextId();
	void fillDummyValue(vector<string>&);
	void fillDummyValue(ostringstream&);


	bool getCalcSumSet(string& hashKey,IndexSet** iset);
	void addCalcSumSet(string& hashKey,IndexSet* iset);
	bool getCalcTempSet(string& hashKey,vector<ModelComp*>& comps,Set** aSet,vector<string>& dummyVars);
	void addCalcTempSet(string& hashKey,vector<ModelComp*>& comps,Set* aSet,vector<string>& dummyVars);

	void addDummySetValueMap(string& dummyVar,ModelComp* comp,string val);
	void calculateMemoryUsage(unsigned long& size);
};

#endif /* CONTEXT_H_ */
