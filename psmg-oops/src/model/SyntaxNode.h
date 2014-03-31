/*
 * SyntaxNode.h
 *
 *  Created on: 28 Sep 2011
 *      Author: s0965328
 */

#ifndef SYNTAXNODE_H_
#define SYNTAXNODE_H_

/** @class SyntaxNode
 *  This is a node in the operator tree.
 *
 *  All AMPL/SML expressions are broken down into a tree of operators.
 *  The nodes of this tree are objects of type SyntaxNode.
 *
 *  Note that values[] is a list of untyped pointers. Normally these
 *  would point to further SyntaxNode structure (children of the current
 *  node). For an ID it is simply a pointer to the string containing
 *  the name.
 *
 *  There are a few special meanings of the values array depending on the
 *  type of node (i.e. the value of opCode).
 *  @bug This should probably be implemented by deriving subclasses, however
 *       an attempt for ID nodes resulted in problems with dynamic_casts
 *
 *  @bug A comma separated list is currently represented by a SyntaxNode with
 *       opCode==COMMA and values[0/1] pointing to the first and last member
 *       in a linked list of SyntaxNodes. This linked list is implemented using
 *       _indexNode objects (which is a SyntaxNode plus a pointer to next).
 *       This is done to facilitate adding members to the list without
 *       knowing the dimension beforehand. Better implemented by replacing
 *       the SyntaxNode::values array with a C++ list.
 */

#include <vector>
#include <set>
#include <string>
#include <sstream>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <boost/unordered_map.hpp>

#include "ModelComp.h"
#include "autodiff.h"
#include "../context/ModelContext.h"
#include "../context/SetSimple.h"
#include "../context/ExpandedModel.h"
#include "../context/PValue.h"

class SyntaxNodeIDREF;
class AmplModel;
class SetComp;

using namespace std;

class SyntaxNode {

public:

	typedef std::vector<SyntaxNode*>::const_iterator iterator;

	iterator begin() const {
		return values.begin();
	}
	iterator end() const {
		return values.end();
	}

	SyntaxNode* front() const {
		return values.front();
	}
	SyntaxNode* back() const {
		return values.back();
	}

	SyntaxNode* operator[](int i) const {
		return values[i];
	}

	/** Clear the child list */
	virtual void clear() {
		opCode = 0;
		values.clear();
	}

public:

	/** ID CODE of this node (a list can be found in ampl.tab.h) */
	int opCode;

	/** List of arguments.
	 *
	 *  For opCode==ID, there is usually one argument, this is a (char*) to the
	 *  name of the entity. If there are two arguments the second argument is
	 *  an (int*) stating the ancestor information as set by ancestor(1).ID in
	 *  stochastic programming */
	vector<SyntaxNode*> values;

public:
	// ------------------------ methods -----------------------------------

	/** Constructor */
	SyntaxNode(int opCode, SyntaxNode *val1 = NULL, SyntaxNode *val2 = NULL, SyntaxNode *val3 = NULL);

	/** Copy constructor */
	SyntaxNode(const SyntaxNode &src);

	/** Destructor */
	virtual ~SyntaxNode();

	/** Retrieve the number of children */
	virtual int nchild() const {
		return values.size();
	}

	/** Recursive printing of expression */
	std::string print() ;

	/** Return the value of this node as a string */
	virtual std::string getName() const {
		throw std::exception();
		return "(fail)";
	}

	virtual std::ostream& put(std::ostream& s);
	virtual SyntaxNode *push_back(SyntaxNode *newitem);
	virtual SyntaxNode *push_front(SyntaxNode *newitem);

	//Feng
	int calculateSetDim();
	int calculateVarDim();
	int calculateParamSetIndicies();
	bool isParamSymbolic();
	void calculateParamIndicies(vector<string>&, vector<SetComp*>&);
	void calculateSetValue(ModelContext* context, Set* rval);
	Set* calculateSetValue(ModelContext* context);
//	Set* calculateTempSet(ModelContext* context);
//	void calculateParamValue(ModelContext* context,double&);
//	void calculateParamValue(ModelContext* context,string&);
	void calculateVarDimCard(ModelContext* ctx, uint& dim, uint& card);
	void calculateVarBounds(ModelContext* ctx, double& upper, double& lower);
	void calculateConCard(ModelContext* ctx,int& card);
	double evalRhs(ModelContext* context);
//	double evalDiff(ModelContext* rowContext, ModelContext* colContext, ModelComp* varComp, string& varKey);
//	void evalDiff(ModelContext* rowContext, ModelContext* colContext, vector<double>& jcobs);
//	bool isZeroVector(vector<double>& v);
//	void multVectorScalar(vector<double>& v, double scalar, vector<double>& result);
//	void plusVector(vector<double>& left, vector<double>& right, vector<double>& result);
//	void minusVector(vector<double>& left, vector<double>& right, vector<double>& result);
//	void negateVector(vector<double>& v);
//	void calcSumSetComp(ModelContext* context, IndexSet** aSet);
//	void calculateCompsDummyNames(ModelContext* context, IndexSet* iset);
//	void calculateIndexSet(ModelContext* context, IndexSet* iset);
//	string setOrgSetComp(ModelComp** orgSetComp);
	bool evalBool(ModelContext* context);
	void evalTerm(ModelContext* context,PValue**);
	IndexSet* createIndexSet(ModelContext* context);
	string printVector(vector<double>&v);
//	void evalTerm(ModelContext* context,string&);

//	void calcVarDefinedLevels(set<int>& levels);
//	void calcSeparability(int level, set<int>& deps);
//	bool containsVarDefInLevel(int level);
//	Node* constructAutoDiffNode(ModelContext* ctx, Block* emb, ExpandedModel* emcol);
//	Node* constructAutoDiffNode(ModelContext* ctx, Block* emb);
//	bool isContainVariablesInEm2(ModelContext* ctx,ExpandedModel* emcol);

	void calculatePartialConstraints(boost::unordered_map<int,SyntaxNode*>&);
	SyntaxNode* moveConsToLeft();
	AutoDiff::Node* createAutoDiffConsDAG(ExpandedModel* emrow,ExpandedModel* emcol,int mode);

	void calculateBaseValueVector(unsigned long& size);
	virtual void calculateMemoryUsage(unsigned long& size);
	//end Feng


	static SyntaxNode *findKeywordinTree(SyntaxNode *root, int oc);
	static SyntaxNode* find_var_ref_in_context(AmplModel* context, SyntaxNode *ref);
	static SyntaxNode* find_var_ref_in_indexing(const std::string& name);

private:
	static SyntaxNode* findSyntaxNodeChild(SyntaxNode* node, int op);
	bool isDepend(vector<ModelComp*> varComps);
	void handleSum(ModelContext* rowContext, vector<double>& jcobs, ModelContext* colContext);
	void foreachSetValue(vector<ModelComp*> comps, vector<string>& dummyVars, SetSimple* aSet, ModelContext* rowContext, vector<double>& jcobs, ModelContext* colContext);
	void getIndiciesKey(ModelContext* ctx,string& varKey);
	static bool isContainsInEm2(string& varKey, ExpandedModel* em);
	AutoDiff::OPCODE opCodeTranslateToAutoDiffOp(int opCode);
	AutoDiff::Node* createAutoDiffConIDREF(int mode, ModelContext* ctx);
};


std::ostream& operator<<(std::ostream& s, SyntaxNode &node);
std::ostream& operator<<(std::ostream& s, SyntaxNode *node);

// Routines taken from ampl.h

#endif /* SYNTAXNODE1_H_ */



//legacy !!

//	void getIDREFIndiciesKey(ModelContext* ctx,string& varKey);

//	// node is a dummy var -> remove (..)
//	std::string printDummyVar() ;
//
//	/** Return comma separated list of arguments for IDREF nodes */
//	std::string getArgumentList() const;

//	/** Find all the IDREF nodes at or below the current node */
//	virtual void findIDREF(std::list<ModelComp*>& lmc) const;
//
//	/** Find all the IDREF nodes at or below the current node */
//	virtual void findIDREF(std::list<SyntaxNode*> *lnd);
//
//	/** Find all nodes of opCode @a oc at or below the current node */
//	virtual void findOpCode(int oc, std::list<SyntaxNode*> *lnd);

//	/** Find the ModelComp (if it exists) referred to by this SyntaxNode.
//	 *
//	 *  If the expression given by this SyntaxNode is an immediate reference to
//	 *  a ModelComp then return that, otherwise return NULL.
//	 */
//	ModelComp *findModelComp() const;

//	/** Creates a deep copy of the nodes: SyntaxNodes pointed to are recreated
//	 *  as well.
//	 *
//	 *  Non-SyntaxNode objects pointed to are not recreated, here just pointers
//	 *  are copied (->ref in the case of a SyntaxNodeIDREF object).
//	 *  The int/double entries pointed to by INT_VAL/FLOAT_VAL SyntaxNodes *are*
//	 *  recreated.
//	 */
//	virtual SyntaxNode *deep_copy();
//
//	/** Creates a copy of the node, reusing the pointer in the current node */
//	virtual SyntaxNode *clone();
