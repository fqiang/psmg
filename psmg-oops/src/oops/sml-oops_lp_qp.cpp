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

#include "sml-oops_lp_qp.h"

using namespace std;


static PDProblem* prob_ptr = NULL;



void SML_OOPS_driver_LP_QP(ExpandedModel *root) {
	ExpandedModel::ptype = LP; //or QP will set later based on xml config file
	ExpandedModel::itype = LOCAL;

	Algebra *AlgAug;
	Vector *vb, *vc, *vu, *vl;
	Vector *vx, *vy, *vz;

	TIMER_START("OOPS_PROBLEM_SETUP");
	Algebra *A = createA(root);
	Algebra *Q = createQ(root);
	if(GV(rank)==0)
		cout<<"Problem Size: row[ "<<A->nb_row<<" ]col[ "<<A->nb_col<<" ]"<<endl;

	AlgAug = OOPSSetup(A, Q);

	// FIXME: should the stuff below be included in OOPSSetup?
	//        that would require OOPSSetup to return vectors as well

	vb = new Vector(A->Trow, "vb");
	vc = new Vector(A->Tcol, "vc");
	vu = new Vector(A->Tcol, "vu");
	vl = new Vector(A->Tcol, "vl");

	vc->fillCallBack(fill_obj_grad_lp_qp);
	vb->fillCallBack(fill_cons_bounds);
	vu->fillCallBack(fill_var_ubound);
	vl->fillCallBack(fill_var_lbound);

	if (GV(writeMatlab)) {
		TRACE("Writing matlab file:mat.m");
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
		TRACE("Writing mps file:mat.m");
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
		TIMER_START("OOPS_SOLVE");
		cout << "Calling OOPS..." << endl;
		Prob.solve(stdout);
		TIMER_STOP("OOPS_SOLVE");
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


/* ---------------------------------------------------------------------------
 CallBackFunction: FillObjVector
 ---------------------------------------------------------------------------- */
void fill_obj_grad_lp_qp(Vector *vc) {
	Tree *T = vc->node;
	DenseVector *dense = GetDenseVectorFromVector(vc);

	Algebra *A = (Algebra*) T->nodeOfAlg; // the diagonal node that spawned this tree
	OOPSBlock *obl = (OOPSBlock*) A->id;  // and its id structure

	assert(obl->emcol->numLocalVars == T->end - T->begin);

	assert(obl->emrow == obl->emcol); // this should be a diagonal block
	obl->emrow->obj_grad_c_lp_qp(dense->elts);
}


/* ---------------------------------------------------------------------------
 CallBackFunction: SMLCallBack
 ---------------------------------------------------------------------------- */
void psmg_callback_a_lp_qp(CallBackInterfaceType *cbi) {
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

	TRACE("psmg_callback_a_lp_qp - called -- ");
	OOPSBlock *obl = (OOPSBlock*) cbi->id;
	if (cbi->row_nbs == NULL) {
//		LOG_TIME(""<<Stat::numNZJacLPCall<<" row: "<<obl->emrow->name<<"  col: "<<obl->emcol->name<<" ");
//		TIMER_START("nz_cons_jacobs_lp");
		cbi->nz = obl->emrow->nz_cons_jacobs_lp_qp(obl->emcol);
//		TIMER_STOP("nz_cons_jacobs_lp");
		TRACE("SMLCallBack  nz - "<<cbi->nz);
	} else {
		TRACE("SMLCallBack  fill -"<<cbi->nz);
//		LOG_TIME(""<<Stat::numJacLPCall<<" row: "<<obl->emrow->name<<"  col: "<<obl->emcol->name<<" ");
//		TIMER_START("cons_jacobs_lp");
		assert(cbi->nz>=0);
		uint max_nz = cbi->nz;
		if(cbi->nz!=0) {
			col_compress_matrix block(obl->emrow->numLocalCons,obl->emcol->numLocalVars,cbi->nz);
			ColSparseMatrix m(cbi->element,cbi->row_nbs,cbi->col_beg,cbi->col_len);
			obl->emrow->cons_jacobs_lp_qp(obl->emcol, block);
			assert(block.nnz()<=cbi->nz);
//			TRACE(""<<block);
			ExpandedModel::convertToColSparseMatrix(block,m,max_nz);
			if(cbi->nz != block.nnz()) {
				WARN("psmg_callback_q_lp_qp,zero value possible - "<<obl->emrow->name<<" X "<<obl->emcol->name<<" cbi["<<cbi->nz<<"] nnz["<<block.nnz()<<"]");
			}
			Stat::nnzJacA_LP_QP+=cbi->nz;
		}
//		TIMER_STOP("cons_jacobs_lp");
	}
	TRACE("end psmg_callback_a_lp_qp --");
}

/* ---------------------------------------------------------------------------
 CallBackFunction: SMLCallBack
 ---------------------------------------------------------------------------- */
void psmg_callback_q_lp_qp(CallBackInterfaceType *cbi) {
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
	TRACE("psmg_callback_q_lp_qp - called --");
	if(ExpandedModel::ptype != QP) {
		assert(ExpandedModel::ptype == LP);
		cbi->nz = 0; //lp solver, therefore no Q matrix
		return;
	}
	else
	{
		OOPSBlock* obl = (OOPSBlock*)cbi->id;
		int rowlevel = obl->emrow->model->level;
		int collevel = obl->emcol->model->level;
		assert(rowlevel != collevel || obl->emrow == obl->emcol);
		if(cbi->row_nbs==NULL) {
			if(rowlevel<collevel){
				cbi->nz = obl->emcol->nz_obj_hess_qp(obl->emrow);
			}
			else{
				cbi->nz = obl->emrow->nz_obj_hess_qp(obl->emcol);
			}
		}
		else{
			assert(cbi->nz>=0);
			uint max_nz = cbi->nz;
			if(cbi->nz != 0)
			{
				if(rowlevel<collevel){ //computing the bottom border Q.
					col_compress_matrix block(obl->emcol->numLocalVars,obl->emrow->numLocalVars,cbi->nz);
					obl->emcol->obj_hess_qp(obl->emrow,block);    //computing RHS Q, then take transpose
					block = boost::numeric::ublas::trans(block);  //taking transpose,
					ColSparseMatrix m(cbi->element,cbi->row_nbs,cbi->col_beg,cbi->col_len);
					ExpandedModel::convertToColSparseMatrix(block,m,max_nz);
					assert(block.nnz()<=cbi->nz);
					if(cbi->nz != block.nnz()) {
						WARN("psmg_callback_q_lp_qp,zero value possible - "<<obl->emrow->name<<" X "<<obl->emcol->name<<" cbi["<<cbi->nz<<"] nnz["<<block.nnz()<<"]");
					}
				}
				else
				{
					col_compress_matrix block(obl->emrow->numLocalVars,obl->emcol->numLocalVars,cbi->nz);
					obl->emrow->obj_hess_qp(obl->emcol,block);
					ColSparseMatrix m(cbi->element,cbi->row_nbs,cbi->col_beg,cbi->col_len);
					ExpandedModel::convertToColSparseMatrix(block,m,max_nz);
					assert(block.nnz()<=cbi->nz);
					if(cbi->nz != block.nnz()) {
						WARN("psmg_callback_q_lp_qp,zero value possible - "<<obl->emrow->name<<" X "<<obl->emcol->name<<" cbi["<<cbi->nz<<"] nnz["<<block.nnz()<<"]");
					}
				}
				Stat::nnzHessQ_QP+=cbi->nz;
			}
		}
	}
	TRACE("end psmg_callback_q_lp_qp - called");
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
