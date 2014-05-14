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

#include "oops/OopsInterface.h"
#include "oops/parutil.h"
#include "oops/hopdm.h"
#include "oops/PDProblem.h"
#include "oops/Algebra.h"
#include "oops/Vector.h"
#include "oops/DenseVector.h"
#include "oops/CallBack.h"
#include "oops/MatrixSparseSimple.h"
#include "oops/DblBordDiagSimpleAlg.h"
#include "oops/BlockDenseAlg.h"
#include "oops/BlockDiagSimpleAlg.h"

#include "../context/ExpandedModel.h"
#include "../context/ColSparseMatrix.h"
#include "../model/AmplModel.h"
#include "../util/util.h"
#include "../sml/Sml.h"

#include "OOPSBlock.h"
#include "sml-oops.h"
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <cassert>
#include <vector>

using namespace std;


static PDProblem* prob_ptr = NULL;

static Algebra *createA(ExpandedModel *em);
static Algebra *createQ(ExpandedModel *em);

static void FillRhsVector(Vector *vb);
static void FillObjVector(Vector *vc);
static void FillUpBndVector(Vector *vu);
static void FillLowBndVector(Vector *vl);


void SML_OOPS_driver_LP_QP(ExpandedModel *root) {
	Algebra *AlgAug;
	Vector *vb, *vc, *vu, *vl;
	Vector *vx, *vy, *vz;

	Algebra *A = createA(root);
	Algebra *Q = createQ(root);
	if(GV(rank)==0)
		cout<<"Problem Size: row[ "<<A->nb_row<<" ]col[ "<<A->nb_col<<" ]"<<endl;

	TIMER_START("OOPS_PROBLEM_SETUP");
	AlgAug = OOPSSetup(A, Q);

	// FIXME: should the stuff below be included in OOPSSetup?
	//        that would require OOPSSetup to return vectors as well

	vb = new Vector(A->Trow, "vb");
	vc = new Vector(A->Tcol, "vc");
	vu = new Vector(A->Tcol, "vu");
	vl = new Vector(A->Tcol, "vl");

	vc->fillCallBack(FillObjVector);
	vb->fillCallBack(FillRhsVector);
	vu->fillCallBack(FillUpBndVector);
	vl->fillCallBack(FillLowBndVector);

	if (GV(writeMatlab)) {
		LOG("Writing matlab file:mat.m");
		string mfile = GV(logdir)+ "mat.m";
		FILE *mout = fopen(mfile.c_str(), "w");
		PrintMatrixMatlab(mout, A, "A");
		PrintMatrixMatlab(mout, Q, "Q");
		vb->printMatlab(mout, "b");
		vc->printMatlab(mout, "c");
		vu->printMatlab(mout, "bu");
		vl->printMatlab(mout, "bl");
		fclose(mout);
	}

	if (GV(writeMPS)) {
		LOG("Writing mps file:mat.m");
		string mfile = GV(logdir)+"test.mps";
		FILE *mps_file = fopen(mfile.c_str(), "w");
		Write_MpsFile(mps_file, AlgAug, vb, vc, vu, vl, 1, NULL, NULL);
		fclose(mps_file);
	}
	PrintOptions Prt(1);

	vx = new Vector(A->Tcol, "vx");
	vy = new Vector(A->Trow, "vy");
	vz = new Vector(A->Tcol, "vz");


	TIMER_STOP("OOPS_PROBLEM_SETUP");

	PDProblem Prob(AlgAug, vb, vc, vl, vu, vx, vy, vz);
	prob_ptr = &Prob;

	if (GV(solve)) {
		cout << "Calling OOPS..." << endl;
		Prob.solve(stdout);
	}

	//Feng the solution upload routine has some bug for parallel version
	//1. oops are reponsible for assignment of node on which part of the subproblem it has
	//2. therefore, a communication are necsseary to move the solution to root node only(?),
	//    or all of the sml ndoe.
	//3. comment out for now just for bench marking
	//SML_OOPS_upload_sol(root, Prob.x, Prob.y, Prob.z);
}

/* ==========================================================================
 Here comes the generation with all subroutines
 =========================================================================== */

static Algebra* createBottomBord(ExpandedModel *emrow, ExpandedModel *emcol);
static Algebra* createRHSBord(ExpandedModel *emrow, ExpandedModel *emcol);
static Algebra* createBottmBordQ(ExpandedModel *emrow, ExpandedModel *emcol);
static Algebra* createRHSBordQ(ExpandedModel *emrow, ExpandedModel *emcol);
static void SMLCallBack(CallBackInterfaceType *cbi);
static void SMLCallBackQ(CallBackInterfaceType *cbi);

/* --------------------------------------------------------------------------
 createA
 --------------------------------------------------------------------------- */
/* This routine sets up the matrix A from the ExpandedModelInterface tree */
Algebra* createA(ExpandedModel *em) {
	LOG("createA for em->getName ["<<em->name<<"]");
	Algebra *Alg = NULL;

	//if (!A->localVarInfoSet) A->setLocalVarInfo();
	if (em->children.size() == 0) {
		// this is final node: read in *.nl file to get dimensions
		LOG("creatA leaf node: ");
		OOPSBlock *obl = new OOPSBlock(em, em);
		obl->prob = prob_ptr; obl->i = 0;
		Alg = NewAlgebraSparse(em->numLocalCons, em->numLocalVars,
				(em->name + ":" + em->name).c_str(),
				(CallBackFunction) SMLCallBack, obl);
	}
	else
	{
		/* this is a complex node, set up DblBordDiag with
		 - Diagonals from children (call createA again)
		 - Off-diagonals with *.nl file from children and col file from parent
		 - bottom from this *.nl file and col from the children              */

		LOG("PSMG - Create complex node:["<< em->numLocalCons<<"] x ["<< em->numLocalVars << "] nchd = " <<em->children.size());

		/* every child is a diagonal block */
		Algebra **D, **B, **R;
		int nblk, i;

		nblk = (em->children).size();
		D = (Algebra **) calloc(nblk + 1, sizeof(Algebra *));
		B = (Algebra **) calloc(nblk, sizeof(Algebra *));
		R = (Algebra **) calloc(nblk, sizeof(Algebra *));

		for (i = 0; i < nblk; i++) {
			LOG("--------------------------------------------- em["<<em->name<<"] ["<<i<<"]");
			D[i] = createA((em->children).at(i));
			B[i] = createBottomBord(em, (em->children).at(i));
			R[i] = createRHSBord((em->children).at(i),em);
			LOG("--------------------------------------------- end em["<<em->name<<"] ["<<i<<"]");
		}

		/* The final D[nblk] block is defined by local constraints/variables */

		// this is final node: read in *.nl file to get dimensions
		// I suspect we can just copy in the code from the leaf node case above
		OOPSBlock *obl = new OOPSBlock(em, em);
		obl->prob = prob_ptr; obl->i = 0;
		D[nblk] = NewAlgebraSparse(em->numLocalCons, em->numLocalVars,
				(em->name + ":" + em->name).c_str(),
				(CallBackFunction) SMLCallBack, obl);

		Alg = NewAlgebraDblBordDiag(nblk, B, R, D,
				(em->name + ":" + em->name).c_str());
	}

	return Alg;
}

Algebra* createBottomBord(ExpandedModel *emrow, ExpandedModel *emcol) {
	LOG("createBottom for emrow["<<emrow->name<<"] emcol["<<emcol->name<<"]");
	Algebra *Alg = NULL;
	if (emcol->children.size() == 0)
	{
		LOG("createBottom leaf node ");
		OOPSBlock *obl = new OOPSBlock(emrow, emcol);
		obl->prob = prob_ptr; obl->i = 0;
		Alg = NewAlgebraSparse(emrow->numLocalCons, emcol->numLocalVars,
				(emrow->name + ":" + emcol->name).c_str(),
				(CallBackFunction) SMLCallBack, obl);
	}
	else
	{
		// this is going to be a BlockDense Algebra
		int nblk = emcol->children.size();
		Algebra **B = (Algebra **) calloc(nblk + 1, sizeof(Algebra *));

		for (int i = 0; i < nblk; i++) {
			B[i] = createBottomBord(emrow, (emcol->children).at(i));
		}

		OOPSBlock *obl = new OOPSBlock(emrow, emcol);
		obl->prob = prob_ptr; obl->i = 0;
		B[nblk] = NewAlgebraSparse(emrow->numLocalCons,
				emcol->numLocalVars,
				(emrow->name + ":" + emcol->name).c_str(),
				(CallBackFunction) SMLCallBack, obl);
		Alg = NewAlgebraBlockDense(1, nblk + 1, B,
				(emrow->name + ":" + emcol->name).c_str());
	}
	return Alg;
}

Algebra* createRHSBord(ExpandedModel *emrow, ExpandedModel *emcol) {
	LOG("createRhs for emrow["<<emrow->name<<"] emcol["<<emcol->name<<"]");
	Algebra *Alg = NULL;
	if (emrow->children.size() == 0) {
		LOG("createRhs leaf node ");
		OOPSBlock *obl = new OOPSBlock(emrow, emcol);
		obl->prob = prob_ptr; obl->i = 0;
		Alg = NewAlgebraSparse(emrow->numLocalCons, emcol->numLocalVars,
				(emrow->name + ":" + emcol->name).c_str(),
				(CallBackFunction) SMLCallBack, obl);

	}
	else {
		// this is going to be a BlockDense Algebra
		int nblk = emrow->children.size();
		Algebra **B = (Algebra **) calloc(nblk + 1, sizeof(Algebra *));

		for (int i = 0; i < nblk; i++) {
			B[i] = createRHSBord((emrow->children).at(i),emcol);
		}

		// The bottom node is made from this node's amplfile and the variables
		// defined in diag
		OOPSBlock *obl = new OOPSBlock(emrow, emcol);
		obl->prob = prob_ptr; obl->i = 0;
		B[nblk] = NewAlgebraSparse(emrow->numLocalCons,
				emcol->numLocalVars,
				(emrow->name + ":" + emcol->name).c_str(),
				(CallBackFunction) SMLCallBack, obl);
		Alg = NewAlgebraBlockDense(nblk + 1, 1, B,
				(emrow->name + ":" + emcol->name).c_str());
	}
	return Alg;
}

/* --------------------------------------------------------------------------
 createQ
 --------------------------------------------------------------------------- */
/* This routine sets up the matrix Q from the ExpandedModelInterface tree */
Algebra *
createQ(ExpandedModel *em) {
	Algebra *Alg = NULL;

	//if (!em->localVarInfoSet) em->setLocalVarInfo();
	if (em->children.size() == 0) {
		// this is final node: read in *.nl file to get dimensions

		OOPSBlock *obl = new OOPSBlock(em, em);
		obl->prob = prob_ptr; obl->i = 0;

		Alg = NewAlgebraSparse(em->numLocalVars, em->numLocalVars,
				("Q" + em->name + ":" + em->name).c_str(),
				(CallBackFunction) SMLCallBackQ, obl);
	} else {
		/* TODO - Design;
		 * assume the Q(x) is DblBordDiagMatrix for Nonlinear problem.
		 * otherwise we will have problem to solve it in OOPS
		 *
		 */

		/* this is a complex node, set up DblBordDiag with
				 - Diagonals from children (call createA again)
				 - Off-diagonals with *.nl file from children and col file from parent
				 - bottom from this *.nl file and col from the children              */
		LOG("PSMG - Create complex node:["<< em->numLocalVars<<"] x ["<< em->numLocalVars << "] nchd = " <<em->children.size());

		/* every child is a diagonal block */
		Algebra **D, **B, **R;
		int nblk, i;

		nblk = (em->children).size();
		D = (Algebra **) calloc(nblk + 1, sizeof(Algebra *));
		B = (Algebra **) calloc(nblk, sizeof(Algebra *));
		R = (Algebra **) calloc(nblk, sizeof(Algebra *));

		for (i = 0; i < nblk; i++) {
			LOG("--------------------------------------------- em["<<em->name<<"] ["<<i<<"]");
			D[i] = createQ(em->children.at(i));
			B[i] = createBottmBordQ(em, (em->children).at(i));
			R[i] = createRHSBordQ((em->children).at(i),em);
			LOG("--------------------------------------------- end em["<<em->name<<"] ["<<i<<"]");
		}

		OOPSBlock *obl = new OOPSBlock(em, em);
		obl->prob = prob_ptr; obl->i = 0;

		D[nblk] = NewAlgebraSparse(em->numLocalVars, em->numLocalVars,
				("Q"+em->name + ":" + em->name).c_str(),
				(CallBackFunction) SMLCallBackQ, obl);

		Alg = NewAlgebraDblBordDiag(nblk, B, R, D,
				("Q"+em->name + ":" + em->name).c_str());
	}

	return Alg;
}

Algebra* createBottmBordQ(ExpandedModel *emrow, ExpandedModel *emcol) {
	LOG("createBottmBorderQ for emrow["<<emrow->name<<"] emcol["<<emcol->name<<"]");
	Algebra *Alg;
	/* This is a bottom block:
	 take the local constraints from the diag block and
	 follow tree defined from the non-diag block */

	if (emcol->children.size() == 0)
	{
		LOG("createBottom leaf node ");
		OOPSBlock *obl = new OOPSBlock(emrow, emcol);
		obl->prob = prob_ptr; obl->i = 0;

		Alg = NewAlgebraSparse(emrow->numLocalVars, emcol->numLocalVars,
				("Q"+emrow->name + ":" + emcol->name).c_str(),
				(CallBackFunction) SMLCallBackQ, obl);
	}
	else
	{
		// this is going to be a BlockDense Algebra
		int nblk = emcol->children.size();
		Algebra **B = (Algebra **) calloc(nblk + 1, sizeof(Algebra *));

		for (int i = 0; i < nblk; i++) {
			B[i] = createBottmBordQ(emrow, (emcol->children).at(i));
		}

		OOPSBlock *obl = new OOPSBlock(emrow, emcol);
		obl->prob = prob_ptr; obl->i = 0;

		B[nblk] = NewAlgebraSparse(emrow->numLocalVars,emcol->numLocalVars,
				("Q"+emrow->name + ":" + emcol->name).c_str(),
				(CallBackFunction) SMLCallBackQ, obl);
		Alg = NewAlgebraBlockDense(1, nblk + 1, B,
				("Q"+emrow->name + ":" + emcol->name).c_str());
	}
	return Alg;
}

Algebra* createRHSBordQ(ExpandedModel *emrow, ExpandedModel *emcol) {
	LOG("createRhs for emrow["<<emrow->name<<"] emcol["<<emcol->name<<"]");
	Algebra *Alg = NULL;
	if (emrow->children.size() == 0) {
		LOG("createRhs leaf node ");
		OOPSBlock *obl = new OOPSBlock(emrow, emcol);
		obl->prob = prob_ptr; obl->i = 0;

		Alg = NewAlgebraSparse(emrow->numLocalVars, emcol->numLocalVars,
				("Q"+emrow->name + ":" + emcol->name).c_str(),
				(CallBackFunction) SMLCallBackQ, obl);
	}
	else {
		// this is going to be a BlockDense Algebra
		int nblk = emrow->children.size();
		Algebra **B = (Algebra **) calloc(nblk + 1, sizeof(Algebra *));

		for (int i = 0; i < nblk; i++) {
			B[i] = createRHSBordQ((emrow->children).at(i),emcol);
		}

		OOPSBlock *obl = new OOPSBlock(emrow, emcol);
		obl->prob = prob_ptr; obl->i = 0;

		B[nblk] = NewAlgebraSparse(emrow->numLocalVars,
				emcol->numLocalVars,
				("Q"+emrow->name + ":" + emcol->name).c_str(),
				(CallBackFunction) SMLCallBackQ, obl);
		Alg = NewAlgebraBlockDense(nblk + 1, 1, B,
				("Q"+emrow->name + ":" + emcol->name).c_str());
	}
	return Alg;
}


/* ---------------------------------------------------------------------------
 CallBackFunction: SMLCallBack
 ---------------------------------------------------------------------------- */
void SMLCallBack(CallBackInterfaceType *cbi) {
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

	LOG("SMLCallBack - called -- ");
	OOPSBlock *obl = (OOPSBlock*) cbi->id;
	if (cbi->row_nbs == NULL) {
//		LOG_TIME(""<<Stat::numNZJacLPCall<<" row: "<<obl->emrow->name<<"  col: "<<obl->emcol->name<<" ");
//		TIMER_START("nz_cons_jacobs_lp");
		cbi->nz = obl->emrow->nz_cons_jacobs_lp_qp(obl->emcol);
//		TIMER_STOP("nz_cons_jacobs_lp");
		LOG("SMLCallBack  nz - "<<cbi->nz);
	} else {
		LOG("SMLCallBack  fill -"<<cbi->nz);
//		LOG_TIME(""<<Stat::numJacLPCall<<" row: "<<obl->emrow->name<<"  col: "<<obl->emcol->name<<" ");
//		TIMER_START("cons_jacobs_lp");
		if(cbi->nz!=0) {
			col_compress_matrix block(obl->emrow->numLocalCons,obl->emcol->numLocalVars,cbi->nz);
			ColSparseMatrix m(cbi->element,cbi->row_nbs,cbi->col_beg,cbi->col_len);
			obl->emrow->cons_jacobs_lp_qp(obl->emcol, block);
			LOG(""<<block);
			ExpandedModel::convertToColSparseMatrix(block,m);
		}
//		TIMER_STOP("cons_jacobs_lp");
	}
	LOG("end SMLCallBack --");
}

/* ---------------------------------------------------------------------------
 CallBackFunction: SMLCallBack
 ---------------------------------------------------------------------------- */
void SMLCallBackQ(CallBackInterfaceType *cbi) {
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
	if(GV(solvetype).compare("lp")==0) {
		cbi->nz = 0; //lp solver, therefore no Q matrix
		return;
	}
	LOG("SMLCalBackQ - called --");
	OOPSBlock* obl = (OOPSBlock*)cbi->id;
	int rowlevel = obl->emrow->model->level;
	int collevel = obl->emcol->model->level;
	if(rowlevel == collevel) assert(obl->emrow == obl->emcol);
	if(cbi->row_nbs==NULL) {
		if(rowlevel<collevel){
			cbi->nz = obl->emcol->nz_obj_hess_qp(obl->emrow);
		}
		else{
			cbi->nz = obl->emrow->nz_obj_hess_qp(obl->emcol);
		}
	}
	else{
		if(cbi->nz != 0)
		{
			if(rowlevel<collevel){ //compute the transpose
				col_compress_matrix block(obl->emcol->numLocalVars,obl->emrow->numLocalVars,cbi->nz);
				obl->emcol->obj_hess_qp(obl->emrow,block);
				ColSparseMatrix m(cbi->element,cbi->row_nbs,cbi->col_beg,cbi->col_len);
				block = boost::numeric::ublas::trans(block);
				ExpandedModel::convertToColSparseMatrix(block,m);
			}
			else
			{
				col_compress_matrix block(obl->emrow->numLocalVars,obl->emcol->numLocalVars,cbi->nz);
				obl->emrow->obj_hess_qp(obl->emcol,block);
				ColSparseMatrix m(cbi->element,cbi->row_nbs,cbi->col_beg,cbi->col_len);
				block = boost::numeric::ublas::trans(block);
				ExpandedModel::convertToColSparseMatrix(block,m);
			}
		}
	}
	LOG("end SMLCalBackQ - called");

}

/* ---------------------------------------------------------------------------
 CallBackFunction: FillObjVector
 ---------------------------------------------------------------------------- */
void FillObjVector(Vector *vc) {
	Tree *T = vc->node;
	DenseVector *dense = GetDenseVectorFromVector(vc);

	Algebra *A = (Algebra*) T->nodeOfAlg; // the diagonal node that spawned this tree
	OOPSBlock *obl = (OOPSBlock*) A->id;  // and its id structure

	assert(obl->emcol->numLocalVars == T->end - T->begin);

	assert(obl->emrow == obl->emcol); // this should be a diagonal block
	obl->emrow->obj_grad_c_lp_qp(dense->elts);
}

/* ---------------------------------------------------------------------------
 CallBackFunction: FillUpBndVector
 ---------------------------------------------------------------------------- */
void FillUpBndVector(Vector *vu) {
	Tree *T = vu->node;
	DenseVector *dense = GetDenseVectorFromVector(vu);

	Algebra *A = (Algebra *) T->nodeOfAlg; // the diagonal node that spawned this tree
	OOPSBlock *obl = (OOPSBlock*) A->id;        // and its id structure
	assert(obl->emrow == obl->emcol); // this should be a diagonal block
	ExpandedModel *emrow = obl->emrow;

	assert(obl->emcol->numLocalVars == T->end - T->begin);
	emrow->get_var_up_bounds(dense->elts);
}

/* ---------------------------------------------------------------------------
 CallBackFunction: FillRhsVector
 ---------------------------------------------------------------------------- */
void FillRhsVector(Vector *vb) {
	Tree *T = vb->node;
	DenseVector *dense = GetDenseVectorFromVector(vb);

	double *checkub = new double[dense->dim];

	Algebra *A = (Algebra*) T->nodeOfAlg; // the diagonal node that spawned this tree
	OOPSBlock *obl = (OOPSBlock*) A->id; // and its id structure
	ExpandedModel *emrow = obl->emrow;

	emrow->get_cons_bounds(dense->elts, checkub);

	// check that lower and upper constraint bounds are the same due to the
	// OOPS restriction
	for (int i = 0; i < dense->dim; i++) {
		if (fabs(dense->elts[i] - checkub[i]) > 1e-6) {
			cerr << "At the moment OOPS only supports equality constraints!\n";
			cerr << "Bounds for c/s " << i << " in " << emrow->name << ": "
					<< dense->elts[i] << " " << checkub[i] << endl;
			exit(1);
		}
	}

	delete[] checkub;
}

/* ---------------------------------------------------------------------------
 CallBackFunction: FillLowBndVector
 ---------------------------------------------------------------------------- */
void FillLowBndVector(Vector *vl) {
	Tree *T = vl->node;
	DenseVector *dense = GetDenseVectorFromVector(vl);

	Algebra *A = (Algebra *) T->nodeOfAlg; // the diagonal node that spawned this tree
	OOPSBlock *obl = (OOPSBlock*) A->id;        // and its id structure
	assert(obl->emrow == obl->emcol); // this should be a diagonal block
	ExpandedModel *emrow = obl->emrow;

	assert(obl->emcol->numLocalVars == T->end - T->begin);
	emrow->get_var_low_bounds(dense->elts);

	for (int i = 0; i < dense->dim; i++) {
		if (fabs(dense->elts[i]) > 1e-6) {
			printf("Found lower bound !=0 (=%f) in variable %i in model %s",
					dense->elts[i], i, emrow->name.c_str());
			printf("Currently OOPS can only cope with zero lower bounds\n");
			exit(1);
		}
	}
}


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