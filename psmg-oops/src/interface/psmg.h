/*
 * psmg.h
 *
 *  Created on: 23 Oct 2013
 *      Author: s0965328
 */

#ifndef PSMG_H_
#define PSMG_H_

#include <vector>

#include "../context/ExpandedModel.h"
#include "../context/ColSparseMatrix.h"

namespace PSMG{
	extern ExpandedModel* getRootProblem();
	extern int getProblemNestedLevel();
	extern int getNumConstraintsLocal(ExpandedModel* em);
	extern int getNumVariablesLocal(ExpandedModel* em);

	//getting the ancester expanded models till root starting from col node.
	extern int getAncesterExpandedModels(ExpandedModel* col, std::vector<ExpandedModel*>& list);
	extern int evaluateFunctionDistribute(ExpandedModel* row, ExpandedModel* col, double**);
	extern int getNumNoneZeroJacobianDistribute(ExpandedModel* row, ExpandedModel* col, unsigned int*);
	extern int evaluateJacobianDistribute(ExpandedModel* row, ExpandedModel* col, ColSparseMatrix**);
	extern int getNumNoneZeroHessianDistribute(ExpandedModel* row, ExpandedModel* col, int**);
	extern int evaluateHessianDistribute(ExpandedModel* row, ExpandedModel* col, ColSparseMatrix**);
	extern int evaluateLagHessianDistribute(ExpandedModel* row, ExpandedModel* col, double** y, ColSparseMatrix**);

	//getting all the possible depended expanded models of row node.
	//list will contains list of expanded model node of all em's ancestors and descendants.
	extern int getAllDependedExpandedModels(ExpandedModel* em, std::vector<ExpandedModel*>& list);
	extern int evaluateFunctionLocal(ExpandedModel* row, double&);
	extern int getNumNoneZeroJacobianLocal(ExpandedModel* row, ExpandedModel* col, unsigned int*);
	extern int evaluateJacobianLocal(ExpandedModel* row, ExpandedModel* col, ColSparseMatrix*);
	extern int getNumNoneZeroHessianLocal(ExpandedModel* row, ExpandedModel* col, int&);
	extern int evaluateHessianLocal(ExpandedModel* row, ExpandedModel* col, ColSparseMatrix*);
	extern int evaluateLagHessianLocal(ExpandedModel* row, ExpandedModel* col, double** y, ColSparseMatrix*);
};

#endif /* PSMG_H_ */
