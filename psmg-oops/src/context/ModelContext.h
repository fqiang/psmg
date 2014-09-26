/*
 * Context.h
 *
 *  Created on: 10 Oct 2011
 *      Author: s0965328
 */

#ifndef MODELCONTEXT_H_
#define MODELCONTEXT_H_

#include <boost/unordered_map.hpp>
#include <string>
#include <vector>
#include "../autodiff/autodiff.h"

using namespace std;

class Set;
class IndexSet;
class CompDescr;
class ModelComp;
class ExpandedModel;
class SyntaxNode;

//1. the ModelComp of dummy index referred to, 2. the value of this dummy index evaluated
typedef std::pair<ModelComp*, string> dummy_tmp_t;

class ModelContext {

public:
	ExpandedModel* em;
	ModelContext* parent;
	bool used;
	//! data storage for this expanded model tree node. key is ModelComp*
	boost::unordered_map<ModelComp*,CompDescr*> compValueMap;

	boost::unordered_map<string, dummy_tmp_t> dummyTempMap;
	boost::unordered_map<string,string> dummyEmcolTempMap;  //this is used for storing emcol model dummy for SUMEXP handling

	//! temporary IndexSet storage - mainly for sum{...} node
	boost::unordered_map<string,IndexSet*> tempISetMap;

	ModelContext(ExpandedModel* par);
	virtual ~ModelContext();
	void dropContent();

	void addDummyEmcolTempMap(string& dummyVar, string& value);
	void removeDummyEmcolTempMap(string& dummyVar);
	void addDummyCompValueMapTemp(string& dummyVar,ModelComp* comp,string& val);
	void removeDummySetValueMapTemp(string& dummyVar);
	string& getDummyValue(string& dummyVar);
	ModelComp* getDummyComp(string& dummyVar);
	CompDescr* getCompValue(ModelComp* comp);
	void addCompValueMap(ModelComp* comp,CompDescr* value);
//	string getContextId();
	bool operator==(const ModelContext& other) const;

	bool getCalcSumSet(string&,IndexSet** iset);
	void addCalcSumSet(string&,IndexSet* iset);

	void calculateMemoryUsage(unsigned long& size);
};

#endif /* MODELCONTEXT_H_ */
