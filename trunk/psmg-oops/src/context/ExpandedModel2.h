/*
 * ExpandedModel2.h
 *
 *  Created on: 17 Oct 2011
 *      Author: s0965328
 */

#ifndef EXPANDEDMODEL2_H_
#define EXPANDEDMODEL2_H_

#include "ModelContext.h"
#include "../model/AmplModel.h"
#include "ExpandedModel2.h"
#include <ext/hash_map>
#include <map>
#include <set>

using namespace std;
using namespace __gnu_cxx;

class ExpandedModel2
{
public:
	vector<ExpandedModel2*> children;
	ExpandedModel2 *parent;

	AmplModel* model;
	ModelContext* context;
	vector<ModelComp*> localVarComps;
	vector<Var*> localVars; //key is ModelComp*
	vector<ModelComp*>  localCons;
	ModelComp* objComp;

	unsigned int numLocalCons;
	unsigned int numLocalVars;
	string name;

	//solutions
	vector<double> pvar, dvar;
	vector<double> prow, drow;


	//holders
	hash_map<int,vector<int> > rowIndicies;
	hash_map<int,vector<double> > values;
	vector<double> rhss;
	bool jcobReady;
	bool rhsReady;

	int atRank;
	bool localVarFilled;

	//end stat

	ExpandedModel2(AmplModel* model,ExpandedModel2* parent,ModelContext* context);
	virtual ~ExpandedModel2();

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
	bool isParent(ExpandedModel2* em2);

	//not used
	//! Returns the names of local variables
	const list<string>& getLocalVarNames() const;
	//! Returns the names of local constraints
	const list<string>& getLocalConNames() const;

	void calculateMemoryUsage(unsigned long& size_str,unsigned long& size_data);

private:
	void updateName();

};

#endif /* EXPANDEDMODEL2_H_ */
