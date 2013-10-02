/*
 * ExpandedModel2.h
 *
 *  Created on: 17 Oct 2011
 *      Author: s0965328
 */

#ifndef EXPANDEDMODEL2_H_
#define EXPANDEDMODEL2_H_

#include <ext/hash_map>
#include <set>
#include "ModelContext.h"
#include "autodiff.h"
#include "ColSparseMatrix.h"

using namespace std;
using namespace __gnu_cxx;
using namespace AutoDiff;

class EMBlock;
class CBlock;
class Block;

class ExpandedModel2
{
public:
	static ExpandedModel2 *root;

	AmplModel* model;   //the prototype model
	ModelContext* ctx;  //data is hold

	ExpandedModel2 *parent;

	unsigned int numLocalVars;
	unsigned int numLocalCons;
	bool localVarFilled;
	bool jcobReady;
	bool rhsReady;
	ModelComp* objComp;
	string name;

	vector<ExpandedModel2*> children;

	__gnu_cxx::hash_map<string,ModelComp*> dummySetMap;
	__gnu_cxx::hash_map<string,string> dummyValueMap;

	//for local variables
	vector<ModelComp*> localVarComps;  //quick access for variables
	vector<Var*> localVars;

	//for local constraints and objective
	vector<ModelComp*>  localConComps;

	//emBlock
	hash_map<ExpandedModel2*,EMBlock* > emBlocksMap;
	hash_map<ExpandedModel2*,CBlock* >	cBlockMap;


	//primary and dual variables
	vector<double> pvar, dvar;
	vector<double> prow, drow;


	//holders
	//col index -> row indices list
	hash_map<int,vector<int> > rowIndicies;
	//col index -> values list (non zero)
	hash_map<int,vector<double> > values;
	vector<double> rhss;

	vector<Node*> variables;
	hash_map<string,Node*> varToIndMap; //var name into index in variables vector<Node*>

	//end stat

	ExpandedModel2(AmplModel* _mod,ModelContext* _context, string dummyVar, ModelComp* comp, string value);
	virtual ~ExpandedModel2();

	void addChildren(ExpandedModel2* em2);
	void clearAllContextTreeKeepRoot();

	//! Returns the unique name of this block.
	string getName() const;
	//! add local constraints model comp
	void addLocalCon(ModelComp*);
	//! add local variables model comp.
	void addLocalVar(ModelComp*);
	//! Returns the number of local variables
	int getNLocalCons() const;
	//! Returns the names of local variables
	int getNLocalVars() const;
	//! Returns the nonzeros in the Jacobian of a section of the model
	int getNzJacobianOfIntersection(ExpandedModel2 *emcol);
	//! Returns the nonzeros in the Jacobian of a section of the model
	void getJacobianOfIntersection(ExpandedModel2 *emcol, int *colbeg,int *collen, int *rownbs, double *el);
	//! Returns the objective gradient for the local model w.r.t. local vars
	void getObjGradient(double *elts);
	void getObjGradient(ExpandedModel2* emcol, CBlock* cb, double* elts);
	//! Returns the vector of upper bounds for the local variables in this model
	void getColUpBounds(double *elts);
	//! Returns the vector of lower bounds for the local variables in this model
	void getColLowBounds(double *elts);
	//! Return the arrays of bounds for the constraints in this model
	void getRowBounds(double *lower, double *upper);
	//! Upload the local variable solutions
	void setPrimalSolColumns(const double *elts);
	//! Upload the local variable duals (multipliers on bounds)
	void setDualSolColumns(const double *elts);
	//! Upload the local constraints slacks
	void setPrimalSolRows(const double *elts);
	//! Upload the local constraints duals (multipliers on constraints)
	void setDualSolRows(const double *elts);
	//! Outputs the solution to the supplied stream at given indent
	void outputSolution(ostream &out,int indent);


	void levelTraversal(vector<ExpandedModel2*>& em2List,int level);
	void fillLocalVar();
	ModelContext* recursiveInitContext();

	//not used
	//! Returns the names of local variables
	const list<string>& getLocalVarNames() const;
	//! Returns the names of local constraints
	const list<string>& getLocalConNames() const;


	void calcReqLevelsVariableforBlock(ExpandedModel2 *emcol_,set<int>& reqLevels);
	EMBlock* constructEMBlock(ExpandedModel2 * emcol);
	void initRecursive(int level, vector<ExpandedModel2*>& em2s);
	void initAutoDiffVariables(Block* emb);

	void convertToColSparseMatrix(ColSparseMatrix* sm);

	//for objective function declared in this expanded model
	CBlock* getCBlock(ExpandedModel2* emcol);
	CBlock* constructCBlock(ExpandedModel2* emcol);

	void calculateMemoryUsage(unsigned long& size_str,unsigned long& size_data);

private:
	void updateName();

};

#endif /* EXPANDEDMODEL2_H_ */
