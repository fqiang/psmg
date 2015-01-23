/* (c) 2008,2009 Jonathan Hogg and Andreas Grothey, University of Edinburgh
 *
 * This file is part of SML.
 *
 * SML is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, using version 3 of the License.
 *
 * SML is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see http://www.gnu.org/licenses/.
 */
/* This is the OOPS driver for the Structured Modelling Language (SML) */




#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>

#include "sml-oops_nlp.h"

using namespace std;


//static PDProblem* prob_ptr = NULL;


void SML_OOPS_driver_NLP(ExpandedModel *root) {
	ExpandedModel::itype = LOCAL;
	ExpandedModel::ptype = NLP;

	//starting point / default value for decision variable
	//TODO: implement functionality to take default value from model file and data file
	std::vector<double> x(ExpandedModel::n_col,0); //hard code the default X0 to 0s.
	ExpandedModel::X0 = &x[0];
	std::vector<double> y(ExpandedModel::n_row,0); //hard code the default X0 to 0s.
	ExpandedModel::Y0 = &y[0];


	assert(ExpandedModel::X==NULL && ExpandedModel::X0!=NULL);
	ExpandedModel::X = ExpandedModel::X0;
	assert(ExpandedModel::Y==NULL && ExpandedModel::Y0!=NULL);
	ExpandedModel::Y = ExpandedModel::Y0;

	OopsOpt Opt;
	Opt.readFile();
	PrintOptions Prt(Opt.prt);

	Algebra *AlgAug;
	Vector *vb, *vc, *vu, *vl;
	Vector *vt, *vs, *vw;
	Vector *vx, *vy, *vz;

	Algebra *A = createA(root);
	Algebra *Q = createQ(root);
	if(GV(rank)==0)
		cout<<"Problem Size: row[ "<<A->nb_row<<" ]col[ "<<A->nb_col<<" ]"<<endl;

	TIMER_START("OOPS_PROBLEM_SETUP_STR");
	//setting up problem in OOPS structure
	AlgAug = OOPSSetup(A, Q);

	// FIXME: should the stuff below be included in OOPSSetup?
	//        that would require OOPSSetup to return vectors as well

	vb = new Vector(A->Trow, "vb");
	vc = new Vector(A->Tcol, "vc"); //Objective gradient
	vu = new Vector(A->Tcol, "vu");
	vl = new Vector(A->Tcol, "vl");
	vu->setDoubleValue(1.e+31);
	vl->setDoubleValue(-1.e+31);

	vc->fillCallBack(fill_obj_grad_nlp); //Objective gradient         -- feng
	vb->fillCallBack(fill_cons_bounds);
	vu->fillCallBack(fill_var_ubound);
	vl->fillCallBack(fill_var_lbound);

	if (GV(writeMatlab)) {
		TRACE("Writing matlab file: "<<GV(matlabFile));
		string mfile = GV(logdir)+ GV(matlabFile);
		FILE *mout = fopen(mfile.c_str(), "w");
		PrintMatrixMatlab(mout, A, "A");
		PrintMatrixMatlab(mout, Q, "Q");
		PrintMatrixMatlab(mout, AlgAug, "AlgAug");
		vb->printMatlab(mout, "b");
		if(vc!=NULL) vc->printMatlab(mout, "c");
		vu->printMatlab(mout, "bu");
		vl->printMatlab(mout, "bl");
		fclose(mout);
	}

	if (GV(writeMPS)) {
		TRACE("Writing mps file:"<<GV(mpsFile));
		string mfile = GV(logdir)+GV(mpsFile);
		FILE *mps_file = fopen(mfile.c_str(), "w");
		Write_MpsFile(mps_file, AlgAug, vb, vc, vu, vl, 1, NULL, NULL);
		fclose(mps_file);
	}

	assert(A->Tcol->end-A->Tcol->begin == A->nb_col);
//	if (stx==NULL)
//		stx = (double*)malloc((A->Tcol->end-A->Tcol->begin)*sizeof(double));
////  if (stz==NULL)
////    stz = (double*)malloc((A->Tcol->end-A->Tcol->begin)*sizeof(double));
//	if (sty==NULL)
//		sty = (double*)malloc((A->Trow->end-A->Trow->begin)*sizeof(double));

	vx = new Vector(A->Tcol, "vx");
	vy = new Vector(A->Trow, "vy");
	vz = new Vector(A->Tcol, "vz");
	vt = new Vector(A->Tcol, "vt");
	vs = new Vector(A->Tcol, "vs");
	vw = new Vector(A->Tcol, "vw");

	TRACE("SolveProblem calls     OOPS.\n");

	TIMER_STOP("OOPS_PROBLEM_SETUP_STR");

	//Feng nlp to fill the matrix
	TIMER_START("OOPS_PROBLEM_SETUP_FILLMAT");
	OOPS_NLP_UPDATE_AQ(A,Q);
	TIMER_STOP("OOPS_PROBLEM_SETUP_FILLMAT");

	PDProblem Prob(AlgAug, vb, vc, vl, vu, vx, vy, vz);
	Prob.Opt = Opt;
	Prob.t = vt;
	Prob.s = vs;
	Prob.w = vw;
	Prob.PrtOpt= Prt;


//	prob_ptr = &Prob;
//	assert(prob_ptr!=NULL);
	if (GV(solve)) {
		TIMER_START("OOPS_SOLVE");
		cout << "Calling OOPS - ipopt_nlp..." << endl;
//		hopdm_ret* ret = ipopt_nlp(stdout,&Prob,&Opt, &Prt);
//		int rv = ret->ifail;
		TIMER_STOP("OOPS_SOLVE");
	}

	// Set up solution data
//	DenseVector *dvx = NewDenseVector( A->nb_col, "final_x" );
//	vx->copyToDense(dvx);
////	Pb->b = dvx; // pass back in Pb
//	DenseVector *dvy = NewDenseVector( A->nb_row, "final_y" );
//	vy->copyToDense(dvy);
//	Pb->c = dvy;

	//Feng the solution upload routine has some bug for parallel version
	//1. oops are reponsible for assignment of node on which part of the subproblem it has
	//2. therefore, a communication are necsseary to move the solution to root node only(?),
	//    or all of the sml ndoe.
	//3. comment out for now just for bench marking
	//SML_OOPS_upload_sol(root, Prob.x, Prob.y, Prob.z);
}


/* ---------------------------------------------------------------------------
 CallBackFunction: FillObjVector
 ---------------------------------------------------------------------------- */
void fill_obj_grad_nlp(Vector *vc) {
	Tree *T = vc->node;
	DenseVector *dense = GetDenseVectorFromVector(vc);

	Algebra *A = (Algebra*) T->nodeOfAlg; // the diagonal node that spawned this tree
	OOPSBlock *obl = (OOPSBlock*) A->id;  // and its id structure

	assert(obl->emcol->numLocalVars == T->end - T->begin);

	assert(obl->emrow == obl->emcol); // this should be a diagonal block
	obl->emrow->obj_grad_nlp_local(dense->elts);
}

/* ==========================================================================
 Here comes the generation with all subroutines
 =========================================================================== */
//
///* --------------------------------------------------------------------------
// createA
// --------------------------------------------------------------------------- */
///* This routine sets up the matrix A from the ExpandedModelInterface tree */
//Algebra* createA(ExpandedModel *em) {
//	TRACE("createA for em->getName ["<<em->name<<"]");
//	Algebra *Alg = NULL;
//
//	//if (!A->localVarInfoSet) A->setLocalVarInfo();
//	if (em->children.size() == 0) {
//		// this is final node: read in *.nl file to get dimensions
//		TRACE("creatA leaf node: ");
//		OOPSBlock *obl = new OOPSBlock(em, em);
//
//		Alg = NewAlgebraSparse(em->numLocalCons, em->numLocalVars,
//				(em->name + ":" + em->name).c_str(),
//				(CallBackFunction) SMLCallBack, obl);
//	}
//	else
//	{
//		/* this is a complex node, set up DblBordDiag with
//		 - Diagonals from children (call createA again)
//		 - Off-diagonals with *.nl file from children and col file from parent
//		 - bottom from this *.nl file and col from the children              */
//
//		TRACE("PSMG - Create complex node:["<< em->numLocalCons<<"] x ["<< em->numLocalVars << "] nchd = " <<em->children.size());
//
//		/* every child is a diagonal block */
//		Algebra **D, **B, **R;
//		int nblk, i;
//
//		nblk = (em->children).size();
//		D = (Algebra **) calloc(nblk + 1, sizeof(Algebra *));
//		B = (Algebra **) calloc(nblk, sizeof(Algebra *));
//		R = (Algebra **) calloc(nblk, sizeof(Algebra *));
//
//		for (i = 0; i < nblk; i++) {
//			TRACE("--------------------------------------------- em["<<em->name<<"] ["<<i<<"]");
//			D[i] = createA((em->children).at(i));
//			B[i] = createBottomBord(em, (em->children).at(i));
//			R[i] = createRHSBord((em->children).at(i),em);
//			TRACE("--------------------------------------------- end em["<<em->name<<"] ["<<i<<"]");
//		}
//
//		/* The final D[nblk] block is defined by local constraints/variables */
//
//		// this is final node: read in *.nl file to get dimensions
//		// I suspect we can just copy in the code from the leaf node case above
//		OOPSBlock *obl = new OOPSBlock(em, em);
//
//		D[nblk] = NewAlgebraSparse(em->numLocalCons, em->numLocalVars,
//				(em->name + ":" + em->name).c_str(),
//				(CallBackFunction) SMLCallBack, obl);
//
//		Alg = NewAlgebraDblBordDiag(nblk, B, R, D,
//				(em->name + ":" + em->name).c_str());
//	}
//
//	return Alg;
//}
//
//Algebra* createBottomBord(ExpandedModel *emrow, ExpandedModel *emcol) {
//	TRACE("createBottom for emrow["<<emrow->name<<"] emcol["<<emcol->name<<"]");
//	Algebra *Alg = NULL;
//	if (emcol->children.size() == 0)
//	{
//		TRACE("createBottom leaf node ");
//		OOPSBlock *obl = new OOPSBlock(emrow, emcol);
//
//		Alg = NewAlgebraSparse(emrow->numLocalCons, emcol->numLocalVars,
//				(emrow->name + ":" + emcol->name).c_str(),
//				(CallBackFunction) SMLCallBack, obl);
//	}
//	else
//	{
//		// this is going to be a BlockDense Algebra
//		int nblk = emcol->children.size();
//		Algebra **B = (Algebra **) calloc(nblk + 1, sizeof(Algebra *));
//
//		for (int i = 0; i < nblk; i++) {
//			B[i] = createBottomBord(emrow, (emcol->children).at(i));
//		}
//
//		OOPSBlock *obl = new OOPSBlock(emrow, emcol);
//
//		B[nblk] = NewAlgebraSparse(emrow->numLocalCons,
//				emcol->numLocalVars,
//				(emrow->name + ":" + emcol->name).c_str(),
//				(CallBackFunction) SMLCallBack, obl);
//		Alg = NewAlgebraBlockDense(1, nblk + 1, B,
//				(emrow->name + ":" + emcol->name).c_str());
//	}
//	return Alg;
//}
//
//Algebra* createRHSBord(ExpandedModel *emrow, ExpandedModel *emcol) {
//	TRACE("createRhs for emrow["<<emrow->name<<"] emcol["<<emcol->name<<"]");
//	Algebra *Alg = NULL;
//	if (emrow->children.size() == 0) {
//		TRACE("createRhs leaf node ");
//		OOPSBlock *obl = new OOPSBlock(emrow, emcol);
//
//		Alg = NewAlgebraSparse(emrow->numLocalCons, emcol->numLocalVars,
//				(emrow->name + ":" + emcol->name).c_str(),
//				(CallBackFunction) SMLCallBack, obl);
//
//	}
//	else {
//		// this is going to be a BlockDense Algebra
//		int nblk = emrow->children.size();
//		Algebra **B = (Algebra **) calloc(nblk + 1, sizeof(Algebra *));
//
//		for (int i = 0; i < nblk; i++) {
//			B[i] = createRHSBord((emrow->children).at(i),emcol);
//		}
//
//		// The bottom node is made from this node's amplfile and the variables
//		// defined in diag
//		OOPSBlock *obl = new OOPSBlock(emrow, emcol);
//
//		B[nblk] = NewAlgebraSparse(emrow->numLocalCons,
//				emcol->numLocalVars,
//				(emrow->name + ":" + emcol->name).c_str(),
//				(CallBackFunction) SMLCallBack, obl);
//		Alg = NewAlgebraBlockDense(nblk + 1, 1, B,
//				(emrow->name + ":" + emcol->name).c_str());
//	}
//	return Alg;
//}
//
///* --------------------------------------------------------------------------
// createQ
// --------------------------------------------------------------------------- */
///* This routine sets up the matrix Q from the ExpandedModelInterface tree */
//Algebra *
//createQ(ExpandedModel *em) {
//	Algebra *Alg = NULL;
//
//	//if (!em->localVarInfoSet) em->setLocalVarInfo();
//	if (em->children.size() == 0) {
//		// this is final node: read in *.nl file to get dimensions
//
//		OOPSBlock *obl = new OOPSBlock(em, em);
//
//
//		Alg = NewAlgebraSparse(em->numLocalVars, em->numLocalVars,
//				("Q" + em->name + ":" + em->name).c_str(),
//				(CallBackFunction) SMLCallBackQ, obl);
//	} else {
//		/* TODO - Design;
//		 * assume the Q(x) is DblBordDiagMatrix for Nonlinear problem.
//		 * otherwise we will have problem to solve it in OOPS
//		 *
//		 */
//
//		/* this is a complex node, set up DblBordDiag with
//				 - Diagonals from children (call createA again)
//				 - Off-diagonals with *.nl file from children and col file from parent
//				 - bottom from this *.nl file and col from the children              */
//		TRACE("PSMG - Create complex node:["<< em->numLocalVars<<"] x ["<< em->numLocalVars << "] nchd = " <<em->children.size());
//
//		/* every child is a diagonal block */
//		Algebra **D, **B, **R;
//		int nblk, i;
//
//		nblk = (em->children).size();
//		D = (Algebra **) calloc(nblk + 1, sizeof(Algebra *));
//		B = (Algebra **) calloc(nblk, sizeof(Algebra *));
//		R = (Algebra **) calloc(nblk, sizeof(Algebra *));
//
//		for (i = 0; i < nblk; i++) {
//			TRACE("--------------------------------------------- em["<<em->name<<"] ["<<i<<"]");
//			D[i] = createQ(em->children.at(i));
//			B[i] = createBottmBordQ(em, (em->children).at(i));
//			R[i] = createRHSBordQ((em->children).at(i),em);
//			TRACE("--------------------------------------------- end em["<<em->name<<"] ["<<i<<"]");
//		}
//
//		OOPSBlock *obl = new OOPSBlock(em, em);
//
//
//		D[nblk] = NewAlgebraSparse(em->numLocalVars, em->numLocalVars,
//				("Q"+em->name + ":" + em->name).c_str(),
//				(CallBackFunction) SMLCallBackQ, obl);
//
//		Alg = NewAlgebraDblBordDiag(nblk, B, R, D,
//				("Q"+em->name + ":" + em->name).c_str());
//	}
//
//	return Alg;
//}
//
//Algebra* createBottmBordQ(ExpandedModel *emrow, ExpandedModel *emcol) {
//	TRACE("createBottmBorderQ for emrow["<<emrow->name<<"] emcol["<<emcol->name<<"]");
//	Algebra *Alg;
//	/* This is a bottom block:
//	 take the local constraints from the diag block and
//	 follow tree defined from the non-diag block */
//
//	if (emcol->children.size() == 0)
//	{
//		TRACE("createBottom leaf node ");
//		OOPSBlock *obl = new OOPSBlock(emrow, emcol);
//
//
//		Alg = NewAlgebraSparse(emrow->numLocalVars, emcol->numLocalVars,
//				("Q"+emrow->name + ":" + emcol->name).c_str(),
//				(CallBackFunction) SMLCallBackQ, obl);
//	}
//	else
//	{
//		// this is going to be a BlockDense Algebra
//		int nblk = emcol->children.size();
//		Algebra **B = (Algebra **) calloc(nblk + 1, sizeof(Algebra *));
//
//		for (int i = 0; i < nblk; i++) {
//			B[i] = createBottmBordQ(emrow, (emcol->children).at(i));
//		}
//
//		OOPSBlock *obl = new OOPSBlock(emrow, emcol);
//
//
//		B[nblk] = NewAlgebraSparse(emrow->numLocalVars,emcol->numLocalVars,
//				("Q"+emrow->name + ":" + emcol->name).c_str(),
//				(CallBackFunction) SMLCallBackQ, obl);
//		Alg = NewAlgebraBlockDense(1, nblk + 1, B,
//				("Q"+emrow->name + ":" + emcol->name).c_str());
//	}
//	return Alg;
//}
//
//Algebra* createRHSBordQ(ExpandedModel *emrow, ExpandedModel *emcol) {
//	TRACE("createRhs for emrow["<<emrow->name<<"] emcol["<<emcol->name<<"]");
//	Algebra *Alg = NULL;
//	if (emrow->children.size() == 0) {
//		TRACE("createRhs leaf node ");
//		OOPSBlock *obl = new OOPSBlock(emrow, emcol);
//
//
//		Alg = NewAlgebraSparse(emrow->numLocalVars, emcol->numLocalVars,
//				("Q"+emrow->name + ":" + emcol->name).c_str(),
//				(CallBackFunction) SMLCallBackQ, obl);
//	}
//	else {
//		// this is going to be a BlockDense Algebra
//		int nblk = emrow->children.size();
//		Algebra **B = (Algebra **) calloc(nblk + 1, sizeof(Algebra *));
//
//		for (int i = 0; i < nblk; i++) {
//			B[i] = createRHSBordQ((emrow->children).at(i),emcol);
//		}
//
//		OOPSBlock *obl = new OOPSBlock(emrow, emcol);
//
//		B[nblk] = NewAlgebraSparse(emrow->numLocalVars,
//				emcol->numLocalVars,
//				("Q"+emrow->name + ":" + emcol->name).c_str(),
//				(CallBackFunction) SMLCallBackQ, obl);
//		Alg = NewAlgebraBlockDense(nblk + 1, 1, B,
//				("Q"+emrow->name + ":" + emcol->name).c_str());
//	}
//	return Alg;
//}
//

/* ---------------------------------------------------------------------------
 CallBackFunction: SMLCallBack
 ---------------------------------------------------------------------------- */
void psmg_callback_a_nlp(CallBackInterfaceType *cbi) {
	TRACE("psmg_callback_a_nlp - called -- ");
	OOPSBlock *obl = (OOPSBlock*) cbi->id;
//	assert(obl->prob!=NULL);
	//update primal vriables x solution for bellow emrow
	//			and ancestor and parent of emrow

	if (cbi->row_nbs == NULL) {
		TRACE("get nz_cons_jacobs --- ");
		//
		//before update the current x in PSMG, the Block need to be initialized by calling getBlockLocal
		//first need to update the primal varibles value
		//then calling cons_jacobs_local to retrieve the jacob for this block
		//1. update primal variables

		//2. compute the non zero for jacob of intersection emrow x emcol
		cbi->nz = obl->emrow->nz_cons_jacobs_nlp_local(obl->emcol);
		Stat::nnzJacA_NLP+=cbi->nz;
	}
	else {
		TRACE("jacobs - want to fill in matrices");
		assert(cbi->nz>=0);
		uint max_nz = cbi->nz;
		if(cbi->nz!=0 && cbi->nlp_str == false) {
			//2. computing jacob of intersection emrow X emcol
			//   use the current point offered by previous call
			col_compress_matrix block(obl->emrow->numLocalCons,obl->emcol->numLocalVars,cbi->nz);
			obl->emrow->cons_jacobs_nlp_local(obl->emcol, block);
			assert(block.nnz()<=cbi->nz);
			ColSparseMatrix m(cbi->element,cbi->row_nbs,cbi->col_beg,cbi->col_len);
			ExpandedModel::convertToColSparseMatrix(block,m,max_nz);
			if(cbi->nz != block.nnz()) {
				WARN("psmg_callback_a_nlp,zero value possible - "<<obl->emrow->name<<" X "<<obl->emcol->name<<" cbi["<<cbi->nz<<"] nnz["<<block.nnz()<<"]");
			}
		}
		else if(cbi->nz!=0){
			assert(cbi->nlp_str==true);
			col_compress_imatrix block(obl->emrow->numLocalCons,obl->emcol->numLocalVars,cbi->nz);
			uint nnz = obl->emrow->nz_cons_jacobs_nlp_local(obl->emcol,block);
			assert(block.nnz()==cbi->nz);
			ColSparseMatrix m(cbi->element,cbi->row_nbs,cbi->col_beg,cbi->col_len);
			ExpandedModel::convertToColSparseMatrix(block,m,max_nz);
		}
	}
	TRACE("end psmg_callback_a_nlp --");
}

/* ---------------------------------------------------------------------------
 CallBackFunction: SMLCallBackQ
 ---------------------------------------------------------------------------- */
void psmg_callback_q_nlp(CallBackInterfaceType *cbi) {
	/*
	 where CallBackInterface is a struct of the following form
	 int nz
	 int max_nz
	 int *row_nbs
	 int *col_beg
	 int *col_len
	 double *element
	 void *id
	 */

	/*
	 * compute the a submatrix of the lagrangian Hessian block defined by emrow X emcol in OOPSBlock
	 * This will consist few parts
	 * 1. the submatrix of hessian of emrow X emcol  -- variable
	 * 2. the submatrix of all the levels above (and has to be of its ancestor) of emrow X emcol -- to deal with variables used in above it's declared level
	 * 3. the submatrix of all the levels bellow of emrow x emcol -- to deal with variables used in bellow it's declared level
	 * 3. objective constrain in emrow x emcol
	 * Assumption: the varaible declared at this level do not appear in objective at other level. If this is the case
	 * additional variable can be introduced to resolve this constraint
	 *
	 * We also assume the solver will call the get function evluation before evaluating the lagrangian hessian, therefore
	 * the current point X is already update to date. We will need to update Y before evaluate lagrangian hessian.
	 */
	TRACE("psmg_callback_q_nlp - called");
	OOPSBlock *obl = (OOPSBlock*) cbi->id;
	int rowlev = obl->emrow->model->level;
	int collev = obl->emcol->model->level;
	assert(rowlev != collev || obl->emrow == obl->emcol);

	if (cbi->row_nbs == NULL) {
		TRACE("get nz laghess --- ");
		//before update the current x in PSMG, the Block need to be initialized by calling getBlockLocal
		//first need to update the primal varibles value
		//then calling cons_jacobs_local to retrieve the jacob for this block
		//1. update primal variables
		//2. compute the nonzero for lagrangian hessian of the block insection by (emrow X emcol)
		if(rowlev < collev) {
			cbi->nz = obl->emcol->nz_lag_hess_nlp_local(obl->emrow);  //compute the transpose
		}
		else {
			cbi->nz = obl->emrow->nz_lag_hess_nlp_local(obl->emcol);
		}
		Stat::nnzLagHess_NLP+=cbi->nz;
	}
	else
	{
		assert(cbi->nz>=0);
		uint max_nz = cbi->nz;
		if(cbi->nz!=0 && cbi->nlp_str==false) {
			TRACE("laghess ---  fill in matrices");
			//2. computing jacob of intersection emrow X emcol
			//   use the current point offered by previous call
			if(rowlev < collev) {  //computing Bottom border Q.
				col_compress_matrix block(obl->emcol->numLocalVars,obl->emrow->numLocalVars,cbi->nz);
				obl->emcol->lag_hess_nlp_local(obl->emrow, block);          //computing RHS Q instead, and taking transpose later.
				assert(block.nnz()<=cbi->nz);
				block = boost::numeric::ublas::trans(block);  				//taking transpose,
				ColSparseMatrix m(cbi->element,cbi->row_nbs,cbi->col_beg,cbi->col_len);
				ExpandedModel::convertToColSparseMatrix(block,m,max_nz);
				if(cbi->nz != block.nnz()) {
					WARN("psmg_callback_q_nlp,zero value possible - "<<obl->emrow->name<<" X "<<obl->emcol->name<<" cbi["<<cbi->nz<<"] nnz["<<block.nnz()<<"]");
				}
			}
			else {  //computing RHS Q.
				col_compress_matrix block(obl->emrow->numLocalVars,obl->emcol->numLocalVars,max_nz);
				obl->emrow->lag_hess_nlp_local(obl->emcol, block);
				assert(block.nnz()<=cbi->nz);
				ColSparseMatrix m(cbi->element,cbi->row_nbs,cbi->col_beg,cbi->col_len);
				ExpandedModel::convertToColSparseMatrix(block,m,max_nz);
				if(cbi->nz != block.nnz()) {
					WARN("psmg_callback_q_nlp,zero value possible - "<<obl->emrow->name<<" X "<<obl->emcol->name<<" cbi["<<cbi->nz<<"] nnz["<<block.nnz()<<"]");
				}
			}
		}
		else if(cbi->nz!=0 ) {
			TRACE("laghess ---  get nz matrix structure");
			assert(cbi->nlp_str == true);
			//2. computing jacob of intersection emrow X emcol
			//   use the current point offered by previous call
			if(rowlev < collev) {  //computing Bottom border Q.
				col_compress_imatrix block(obl->emcol->numLocalVars,obl->emrow->numLocalVars,cbi->nz);
				obl->emcol->nz_lag_hess_nlp_local(obl->emrow, block);          //computing RHS Q instead, and taking transpose later.
				block = boost::numeric::ublas::trans(block);  				//taking transpose,
				ColSparseMatrix m(cbi->element,cbi->row_nbs,cbi->col_beg,cbi->col_len);
				ExpandedModel::convertToColSparseMatrix(block,m,max_nz);
				assert(cbi->nz == block.nnz());
			}
			else {  //computing RHS Q.
				col_compress_imatrix block(obl->emrow->numLocalVars,obl->emcol->numLocalVars,max_nz);
				obl->emrow->nz_lag_hess_nlp_local(obl->emcol, block);
				ColSparseMatrix m(cbi->element,cbi->row_nbs,cbi->col_beg,cbi->col_len);
				ExpandedModel::convertToColSparseMatrix(block,m,max_nz);
				assert(cbi->nz == block.nnz());
			}
		}
	}
	TRACE("end psmg_callback_q_nlp - called");

}

//Vector* find_sub_vector(ExpandedModel* root, Vector* root_vx, ExpandedModel* lookfor, vector<int>& path)
//{
//	Vector* rval = NULL;
//	if(root==lookfor)
//	{
//		return rval = root_vx;
//	}
//	else
//	{
//		for(int i=0;i<root->children.size();i++)
//		{
//			path.push_back(i);
//			Vector* sub_vx = SubVector(root_vx,i);
//			rval = find_sub_vector(root->children.at(i),sub_vx,lookfor,path);
//			if(rval != NULL)
//				break;
//			else
//			{
//				path.pop_back();
//			}
//		}
//	}
//	return rval;
//}
//
///*
// * Update the relate x vector
// */
//void OOPS_update_x(ExpandedModel* emrow)
//{
//	vector<int> path;
//	Vector* sub_vx = find_sub_vector(ExpandedModel::root,prob_ptr->x,emrow,path);
//	OOPS_update_sol_x_all_bellow(emrow,sub_vx);
//	OOPS_update_x_parent_only(emrow, path);
//}
//
///* update the primal solution x for all expanded model bellow
// * and include the em itself.
// */
//void OOPS_update_sol_x_all_bellow(ExpandedModel* em, Vector *vx)
//{
//	Tree * tx = vx->node;
//	if(!tx->local)
//	{
//		TRACE("OOPS_update_sol_x_all_bellow -- em["<<em->name<<"]  - solution is not on this proc");
//	}
//	int nchild = em->children.size();
//	assert((nchild == 0 && tx->nb_sons==0) || tx->nb_sons == nchild + 1);
//	if(nchild==0)
//	{
//		DenseVector *dx = GetDenseVectorFromVector(vx);
//		assert(dx->dim == em->numLocalVars);
//		em->update_primal_x(dx->elts);
//	}
//	else
//	{
//		// if there is child, the vx has size of nchild+1 node.
//		// the vxs is stored at index = nchild
//		Vector* vxs = SubVector(vx,nchild);
//		DenseVector* dx = GetDenseVectorFromVector(vxs);
//		assert(dx->dim == em->numLocalVars);
//		em->update_primal_x(dx->elts);
//		for(int i=0;i<nchild;i++){
//			ExpandedModel* child = em->children.at(i);
//			OOPS_update_sol_x_all_bellow(child,SubVector(vx,i));
//		}
//	}
//}
//
//void OOPS_update_x_parent_only(ExpandedModel* emrow, vector<int>& path)
//{
//	assert((emrow==ExpandedModel::root&&path.size()==0) && true);
//
//	ExpandedModel* currem = ExpandedModel::root;
//	Vector* currvx = prob_ptr->x;
//	for(int i=0;i<path.size();i++)
//	{
//		if(!currvx->node->local){
//			TRACE("OOPS_update_sol_x_parent_only -- em["<<currem->name<<"]  - solution is not on this proc");
//		}
//
//		int ci = path[i];
//		Vector* vxs = SubVector(currvx,currem->children.size());
//		DenseVector* dx = GetDenseVectorFromVector(vxs);
//		currem->update_primal_x(dx->elts);
//
//		currvx = SubVector(currvx,ci);
//		currem = currem->children.at(ci);
//	}
//}
//
///*
// * Update the related Y vector
// */
//
//void OOPS_update_y(ExpandedModel* emrow)
//{
//	vector<int> path;
//	Vector* sub_vy = find_sub_vector(ExpandedModel::root,prob_ptr->x,emrow,path);
//	OOPS_update_sol_y_all_bellow(emrow,sub_vy);
//	OOPS_update_y_parent_only(emrow, path);
//}
//
//void OOPS_update_sol_y_all_bellow(ExpandedModel* em, Vector *vy)
//{
//	Tree * tx = vy->node;
//	if(!tx->local)
//	{
//		TRACE("OOPS_update_sol_x_all_bellow -- em["<<em->name<<"]  - solution is not on this proc");
//	}
//	int nchild = em->children.size();
//	assert((nchild == 0 && tx->nb_sons==0) || tx->nb_sons == nchild + 1);
//	if(nchild==0)
//	{
//		DenseVector *dy = GetDenseVectorFromVector(vy);
//		assert(dy->dim == em->numLocalCons);
//		em->update_primal_x(dy->elts);
//	}
//	else
//	{
//		// if there is child, the vx has size of nchild+1 node.
//		// the vxs is stored at index = nchild
//		Vector* vys = SubVector(vy,nchild);
//		DenseVector* dy = GetDenseVectorFromVector(vys);
//		assert(dy->dim == em->numLocalCons);
//		em->update_lag(dy->elts);
//		for(int i=0;i<nchild;i++){
//			ExpandedModel* child = em->children.at(i);
//			OOPS_update_sol_y_all_bellow(child,SubVector(vy,i));
//		}
//	}
//}
//
//void OOPS_update_y_parent_only(ExpandedModel* emrow, vector<int>& path)
//{
//	assert((emrow==ExpandedModel::root&&path.size()==0) && true);
//
//	ExpandedModel* currem = ExpandedModel::root;
//	Vector* currvy = prob_ptr->y;
//	for(int i=0;i<path.size();i++)
//	{
//		if(!currvy->node->local){
//			TRACE("OOPS_update_sol_x_parent_only -- em["<<currem->name<<"]  - solution is not on this proc");
//		}
//
//		int ci = path[i];
//		Vector* vys = SubVector(currvy,currem->children.size());
//		DenseVector* dy = GetDenseVectorFromVector(vys);
//		currem->update_lag(dy->elts);
//
//		currvy = SubVector(currvy,ci);
//		currem = currem->children.at(ci);
//	}
//}
//
//
///* ---------------------------------------------------------------------------
// CallBackFunction: FillObjVector
// ---------------------------------------------------------------------------- */
//void FillObjVector(Vector *vc) {
//	assert(ExpandedModel::ptype == NLP);
//
//	Tree *T = vc->node;
//	DenseVector *dense = GetDenseVectorFromVector(vc);
//
//	Algebra *A = (Algebra*) T->nodeOfAlg; // the diagonal node that spawned this tree
//	OOPSBlock *obl = (OOPSBlock*) A->id;        // and its id structure
//
//	assert(obl->emcol->numLocalVars == T->end - T->begin);
//
//	assert(obl->emrow == obl->emcol);
//	obl->emrow->obj_grad_nlp_local(dense->elts);
//}

///* ---------------------------------------------------------------------------
// CallBackFunction: FillUpBndVector
// ---------------------------------------------------------------------------- */
//void fill_var_ubound_nlp(Vector *vu) {
//	Tree *T = vu->node;
//	DenseVector *dense = GetDenseVectorFromVector(vu);
//
//	Algebra *A = (Algebra *) T->nodeOfAlg; // the diagonal node that spawned this tree
//	//NodeId *id = (NodeId*)A->id;        // and its id structure
//	OOPSBlock *obl = (OOPSBlock*) A->id;        // and its id structure
//	assert(obl->emrow == obl->emcol); // this should be a diagonal block
//	//NlFile *nlf = obl->emrow->nlfile;
//	ExpandedModel *emrow = obl->emrow;
//
//	assert(obl->emcol->numLocalVars == T->end - T->begin);
//	emrow->get_var_up_bounds(dense->elts);
//}
//
///* ---------------------------------------------------------------------------
// CallBackFunction: FillRhsVector
// ---------------------------------------------------------------------------- */
//void fill_cons_bounds_nlp(Vector *vb) {
//	Tree *T = vb->node;
//	DenseVector *dense = GetDenseVectorFromVector(vb);
//
//	double *checkub = new double[dense->dim];
//
//	Algebra *A = (Algebra*) T->nodeOfAlg; // the diagonal node that spawned this tree
//	OOPSBlock *obl = (OOPSBlock*) A->id; // and its id structure
//	ExpandedModel *emrow = obl->emrow;
//
//	// FIXME: should the id structure include information on the ExpandedModelInterface
//	//        as well? That way we could do some more sanity checks
//
//	emrow->get_cons_bounds(dense->elts, checkub);
//
//	// check that lower and upper constraint bounds are the same due to the
//	// OOPS restriction
//	for (int i = 0; i < dense->dim; i++) {
//		if (fabs(dense->elts[i] - checkub[i]) > 1e-6) {
//			cerr << "At the moment OOPS only supports equality constraints!\n";
//			cerr << "Bounds for c/s " << i << " in " << emrow->name << ": "
//					<< dense->elts[i] << " " << checkub[i] << endl;
//			exit(1);
//		}
//	}
//
//	delete[] checkub;
//}
//
///* ---------------------------------------------------------------------------
// CallBackFunction: FillLowBndVector
// ---------------------------------------------------------------------------- */
//void fill_var_lbound_nlp(Vector *vl) {
//	Tree *T = vl->node;
//	DenseVector *dense = GetDenseVectorFromVector(vl);
//
//	Algebra *A = (Algebra *) T->nodeOfAlg; // the diagonal node that spawned this tree
//	OOPSBlock *obl = (OOPSBlock*) A->id;        // and its id structure
//	assert(obl->emrow == obl->emcol); // this should be a diagonal block
//	ExpandedModel *emrow = obl->emrow;
//
//	assert(obl->emcol->numLocalVars == T->end - T->begin);
//	emrow->get_var_low_bounds(dense->elts);
//
//	for (int i = 0; i < dense->dim; i++) {
//		if (fabs(dense->elts[i]) > 1e-6) {
//			printf("Found lower bound !=0 (=%f) in variable %i in model %s",
//					dense->elts[i], i, emrow->name.c_str());
//			printf("Currently OOPS can only cope with zero lower bounds\n");
//			exit(1);
//		}
//	}
//}


/* ---------------------------------------------------------------------------
SML_OOPS_upload_sol
---------------------------------------------------------------------------- */
//void SML_OOPS_upload_sol(ExpandedModel *em,Vector *vx, Vector *vy, Vector *vz)
//{
//	Tree *Tx = vx->node,*Ty = vy->node;
//	int nchd = em->children.size();
//
//	//printf("%d: %d %d\n",nchd, Tx->nb_sons, Ty->nb_sons);
//	assert((nchd==0&&Tx->nb_sons==0)||Tx->nb_sons==nchd+1);
//	assert((nchd==0&&Ty->nb_sons==0)||Ty->nb_sons==nchd+1);
//	if (nchd>0)
//	{
//		/* The final child of the vector tree corresponds to the local variables/
//		   constraints of this node */
//
//		/* upload local contributions */
//		Vector *vxs = SubVector(vx, nchd);
//		Vector *vys = SubVector(vy, nchd);
//		Vector *vzs = SubVector(vz, nchd);
//
//		DenseVector *dx = GetDenseVectorFromVector(vxs);
//		DenseVector *dy = GetDenseVectorFromVector(vys);
//		DenseVector *dz = GetDenseVectorFromVector(vzs);
//
//		assert(dx->dim == em->numLocalVars);
//		assert(dy->dim == em->numLocalCons);
//
//		em->setPrimalSolColumns(dx->elts);
//		em->setDualSolColumns(dz->elts);
//		em->setDualSolRows(dy->elts);
//
//		/* and upload a vector of zeros for the constraint slacks
//		   (OOPS can only deal with equality constraints)                    */
//		double *elts = new double[dy->dim];
//		for(int i=0;i<dy->dim;i++) elts[i] = 0;
//		em->setPrimalSolRows(elts);
//		delete[] elts;
//
//		/* recurse down the rest of the tree */
//
//		for (int i=0;i<nchd;i++)
//		{
//			ExpandedModel *model = em->children[i];
//			SML_OOPS_upload_sol(model,
//				  SubVector(vx, i), SubVector(vy, i), SubVector(vz,i));
//		}
//	}
//	else
//	{
//		/* This is a root node of the model tree and the vector trees */
//		DenseVector *dx = GetDenseVectorFromVector(vx);
//		DenseVector *dy = GetDenseVectorFromVector(vy);
//		DenseVector *dz = GetDenseVectorFromVector(vz);
//
//		assert(dx->dim == em->numLocalVars);
//		assert(dy->dim == em->numLocalCons);
//
//		em->setPrimalSolColumns(dx->elts);
//		em->setDualSolColumns(dz->elts);
//		em->setDualSolRows(dy->elts);
//
//		/* and upload a vector of zeros for the constraint slacks
//		   (OOPS can only deal with equality constraints)                    */
//		double *elts = new double[dy->dim];
//		for(int i=0;i<dy->dim;i++) elts[i] = 0;
//		em->setPrimalSolRows(elts);
//		delete[] elts;
//	}
//}
