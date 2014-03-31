/*
 * psmg.cpp
 *
 *  Created on: 23 Oct 2013
 *      Author: s0965328
 */

#include "psmg.h"
#include "../model/AmplModel.h"

void recursiveFillList(ExpandedModel* em,std::vector<ExpandedModel*>& list)
{
	list.push_back(em);
	if(em->parent!=NULL)
	{
		recursiveFillList(em->parent,list);
	}
}


namespace PSMG
{


ExpandedModel* getRootProblem()
{
	assert(ExpandedModel::root!=NULL);
	return ExpandedModel::root;
}

int getProblemNestedLevel()
{
	assert(AmplModel::root!=NULL);
	return AmplModel::MAX_LEVEL;
}


int getNumConstraintsLocal(ExpandedModel* em)
{
	assert(em!=NULL);
	return em->numLocalCons;
}

int getNumVariablesLocal(ExpandedModel* em)
{
	assert(em!=NULL);
	return em->numLocalVars;
}

int getAncesterExpandedModels(ExpandedModel* col, std::vector<ExpandedModel*>& list)
{
	assert(col!=NULL);
	if(col->parent!=NULL){
		recursiveFillList(col->parent,list);
	}
	return 0;
}
int evaluateFunctionDistribute(ExpandedModel* row, ExpandedModel* col, double* vals)
{
	vector<double> fvals;
	row->cons_feval_distribute(col,fvals);
	return 0;
}

int getNumNoneZeroJacobianDistribute(ExpandedModel* row, ExpandedModel* col, unsigned int* nz)
{

}

int evaluateJacobianDistribute(ExpandedModel* row, ExpandedModel* col, ColSparseMatrix**)
{

}

int getNumNoneZeroHessianDistribute(ExpandedModel* row, ExpandedModel* col, int**)
{

}

int evaluateHessianDistribute(ExpandedModel* row, ExpandedModel* col, ColSparseMatrix**)
{

}
int evaluateLagHessianDistribute(ExpandedModel* row, ExpandedModel* col, double** y, ColSparseMatrix**)
{

}

int getNumNoneZeroJacobianLocal(ExpandedModel* row, ExpandedModel* col, unsigned int* nz)
{
	*nz = row->nz_cons_jacobs_local(col);
	return 0;
}

}

