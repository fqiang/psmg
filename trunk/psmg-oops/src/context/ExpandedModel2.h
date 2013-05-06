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
#include "ExpandedModelAbstract.h"
#include <ext/hash_map>
#include <map>
#include <set>

using namespace std;
using namespace __gnu_cxx;

class ExpandedModel2 : public ExpandedModelAbstract
{
public:
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

	ExpandedModel2(AmplModel* model,ExpandedModelAbstract* parent,ModelContext* context);
	virtual ~ExpandedModel2();

	string getName() const;
	void addLocalCon(ModelComp*);
	void addLocalVar(ModelComp*);
	int getNLocalCons() const;
	int getNLocalVars() const;
	int getNzJacobianOfIntersection(ExpandedModelAbstract *emcol);
	void getJacobianOfIntersection(ExpandedModelAbstract *emcol, int *colbeg,int *collen, int *rownbs, double *el);
	void getObjGradient(double *elts);
	void getColUpBounds(double *elts);
	void getColLowBounds(double *elts);
	void getRowBounds(double *lower, double *upper);
	void setPrimalSolColumns(const double *elts);
	void setDualSolColumns(const double *elts);
	void setPrimalSolRows(const double *elts);
	void setDualSolRows(const double *elts);
	void outputSolution(ostream &out,int indent);
	void levelTraversal(vector<ExpandedModel2*>& em2List,int level);
	void fillLocalVar();
	ModelContext* recursiveInitContext();
	bool isParent(ExpandedModel2* em2);

	//not used
	const list<string>& getLocalVarNames() const;
	const list<string>& getLocalConNames() const;

	void calculateMemoryUsage(unsigned long& size_str,unsigned long& size_data);

private:
	void updateName();

};

#endif /* EXPANDEDMODEL2_H_ */
