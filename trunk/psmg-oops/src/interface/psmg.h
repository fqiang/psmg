/*
 * psmg.h
 *
 *  Created on: 23 Oct 2013
 *      Author: s0965328
 */

#ifndef PSMG_H_
#define PSMG_H_

#include <vector>
#include <string>

#include "../util/compile_marco.h"

class ExpandedModel;
class ColSparseMatrix;

using namespace std;

//Maybe it is easier to just use ExpandedModel class as interface to access PSMG
namespace PSMG{
	//For both local and distribute interface call
	extern ExpandedModel* prob_root();
	extern int num_local_cons(ExpandedModel* em);
	extern int num_local_vars(ExpandedModel* em);
	extern int local_cons_names(ExpandedModel* em,std::vector<string>& names);

	//Local Interface Call --
	extern int depended_em_local(ExpandedModel* em, std::vector<ExpandedModel*>& list);
	extern int eval_cons_local(ExpandedModel* row, double*);
	extern int nz_jac_cons_local(ExpandedModel* row, ExpandedModel* col, uint&);
	extern int jac_cons_local(ExpandedModel* row, ExpandedModel* col, ColSparseMatrix&);
	extern int nz_hess_cons_local(ExpandedModel* row, ExpandedModel* col, uint&);
	extern int hess_cons_local(ExpandedModel* row, ExpandedModel* col, ColSparseMatrix&);
	extern int eval_obj_local(ExpandedModel* row, ExpandedModel* col, double&);
	extern int obj_jac_local(ExpandedModel* row, ExpandedModel* col, double*);
	extern int nz_obj_hess_local(ExpandedModel* row, ExpandedModel* col, uint&);
	extern int obj_hess_local(ExpandedModel* row, ExpandedModel* col, ColSparseMatrix&);



	//Distribute Interface Calls -- TODO unfinished
	extern int evaluateFunctionDistribute(ExpandedModel* row, ExpandedModel* col, double**);
	extern int getNumNoneZeroJacobianDistribute(ExpandedModel* row, ExpandedModel* col, unsigned int*);
	extern int evaluateJacobianDistribute(ExpandedModel* row, ExpandedModel* col, ColSparseMatrix**);
	extern int getNumNoneZeroHessianDistribute(ExpandedModel* row, ExpandedModel* col, int**);
	extern int evaluateHessianDistribute(ExpandedModel* row, ExpandedModel* col, ColSparseMatrix**);
	extern int evaluateLagHessianDistribute(ExpandedModel* row, ExpandedModel* col, double** y, ColSparseMatrix**);

};

#endif /* PSMG_H_ */
