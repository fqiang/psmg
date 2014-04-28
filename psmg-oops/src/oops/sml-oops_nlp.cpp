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
#include "../util/global_util_functions.h"
#include "../sml/Sml.h"

#include "OOPSBlock.h"
#include "sml-oops.h"
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <cassert>
#include <vector>

using namespace std;

typedef boost::numeric::ublas::compressed_matrix<double>  compressed_matrix;


static PDProblem* prob_ptr = NULL;

static Algebra *createA(ExpandedModel *em);
static Algebra *createQ(ExpandedModel *em);

static Algebra* createBottomBord(ExpandedModel *emrow, ExpandedModel *emcol);
static Algebra* createRHSBord(ExpandedModel *emrow, ExpandedModel *emcol);
static Algebra* createBottmBordQ(ExpandedModel *emrow, ExpandedModel *emcol);
static Algebra* createRHSBordQ(ExpandedModel *emrow, ExpandedModel *emcol);
static void SMLCallBack(CallBackInterfaceType *cbi);
static void SMLCallBackQ(CallBackInterfaceType *cbi);

static void FillRhsVector(Vector *vb);
static void FillObjVector(Vector *vc);
static void FillUpBndVector(Vector *vu);
static void FillLowBndVector(Vector *vl);

static void OOPS_update_x(ExpandedModel* emrow);
static Vector* find_sub_vector(ExpandedModel* root, Vector* root_vx, ExpandedModel* lookfor, vector<int>&);
static void OOPS_update_sol_x_all_bellow(ExpandedModel* em, Vector *vx);
static void OOPS_update_x_parent_only(ExpandedModel* emrow, vector<int>& path);

static void OOPS_update_y(ExpandedModel* emrow);
static void OOPS_update_sol_y_all_bellow(ExpandedModel* em, Vector *vx);
static void OOPS_update_y_parent_only(ExpandedModel* emrow, vector<int>& path);



void SML_OOPS_driver_NLP(ExpandedModel *root) {
	Algebra *AlgAug;
	Vector *vb, *vc, *vu, *vl;
	Vector *vx, *vy, *vz;

	Algebra *A = createA(root);
	Algebra *Q = createQ(root);

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
		LOG("Writing matlab file: "<<GV(matlabFile));
		string mfile = GV(logdir)+ GV(matlabFile);
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
		LOG("Writing mps file:"<<GV(mpsFile));
		string mfile = GV(logdir)+GV(mpsFile);
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
	assert(prob_ptr!=NULL);
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
//	assert(obl->prob!=NULL);
	//update primal vriables x solution for bellow emrow
	//			and ancestor and parent of emrow

	if (cbi->row_nbs == NULL) {
		LOG("get nz_cons_jacobs --- ");
		//
		//before update the current x in PSMG, the Block need to be initialized by calling getBlockLocal
		obl->emrow->getBlockLocal();
		//first need to update the primal varibles value
		//then calling cons_jacobs_local to retrieve the jacob for this block
		//1. update primal variables
		if(obl->i != 0){
			OOPS_update_x(obl->emrow);
		}
		obl->i++;
		//2. compute the non zero for jacob of intersection emrow x emcol
		cbi->nz = obl->emrow->nz_cons_jacobs_local(obl->emcol);
	} else {
		LOG("jacobs - want to fill in matrices");
		//2. computing jacob of intersection emrow X emcol
		//   use the current point offered by previous call
		compressed_matrix block;
		obl->emrow->cons_jacobs_local(obl->emcol, block);
		ColSparseMatrix m(cbi->element,cbi->row_nbs,cbi->col_beg,cbi->col_len);
		ExpandedModel::convertToColSparseMatrix(block,&m);
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
	LOG("SMLCalBackQ - called");
	OOPSBlock *obl = (OOPSBlock*) cbi->id;

	if (cbi->row_nbs == NULL) {
		LOG("get nz laghess --- ");
		//before update the current x in PSMG, the Block need to be initialized by calling getBlockLocal
		obl->emrow->getBlockLocal();
		//first need to update the primal varibles value
		//then calling cons_jacobs_local to retrieve the jacob for this block
		//1. update primal variables
		if(obl->i != 0){
			OOPS_update_y(obl->emrow);
		}
		obl->i++;
		//2. compute the nonzero for lagrangian hessian of the block insection by (emrow X emcol)
		cbi->nz = obl->emrow->nz_lag_hess_local(obl->emcol);
	}
	else
	{
		LOG("laghess ---  fill in matrices");
		//2. computing jacob of intersection emrow X emcol
		//   use the current point offered by previous call
		compressed_matrix block;
		obl->emrow->lag_hess_local(obl->emcol, block);
		ColSparseMatrix m(cbi->element,cbi->row_nbs,cbi->col_beg,cbi->col_len);
		ExpandedModel::convertToColSparseMatrix(block,&m);
	}
	LOG("end SMLCalBackQ - called");

}

Vector* find_sub_vector(ExpandedModel* root, Vector* root_vx, ExpandedModel* lookfor, vector<int>& path)
{
	Vector* rval = NULL;
	if(root==lookfor)
	{
		return rval = root_vx;
	}
	else
	{
		for(int i=0;i<root->children.size();i++)
		{
			path.push_back(i);
			Vector* sub_vx = SubVector(root_vx,i);
			rval = find_sub_vector(root->children.at(i),sub_vx,lookfor,path);
			if(rval != NULL)
				break;
			else
			{
				path.pop_back();
			}
		}
	}
	return rval;
}

/*
 * Update the relate x vector
 */
void OOPS_update_x(ExpandedModel* emrow)
{
	vector<int> path;
	Vector* sub_vx = find_sub_vector(ExpandedModel::root,prob_ptr->x,emrow,path);
	OOPS_update_sol_x_all_bellow(emrow,sub_vx);
	OOPS_update_x_parent_only(emrow, path);
}

/* update the primal solution x for all expanded model bellow
 * and include the em itself.
 */
void OOPS_update_sol_x_all_bellow(ExpandedModel* em, Vector *vx)
{
	Tree * tx = vx->node;
	if(!tx->local)
	{
		LOG("OOPS_update_sol_x_all_bellow -- em["<<em->name<<"]  - solution is not on this proc");
	}
	int nchild = em->children.size();
	assert((nchild == 0 && tx->nb_sons==0) || tx->nb_sons == nchild + 1);
	if(nchild==0)
	{
		DenseVector *dx = GetDenseVectorFromVector(vx);
		assert(dx->dim == em->numLocalVars);
		em->update_primal_x(dx->elts);
	}
	else
	{
		// if there is child, the vx has size of nchild+1 node.
		// the vxs is stored at index = nchild
		Vector* vxs = SubVector(vx,nchild);
		DenseVector* dx = GetDenseVectorFromVector(vxs);
		assert(dx->dim == em->numLocalVars);
		em->update_primal_x(dx->elts);
		for(int i=0;i<nchild;i++){
			ExpandedModel* child = em->children.at(i);
			OOPS_update_sol_x_all_bellow(child,SubVector(vx,i));
		}
	}
}

void OOPS_update_x_parent_only(ExpandedModel* emrow, vector<int>& path)
{
	assert((emrow==ExpandedModel::root&&path.size()==0) && true);

	ExpandedModel* currem = ExpandedModel::root;
	Vector* currvx = prob_ptr->x;
	for(int i=0;i<path.size();i++)
	{
		if(!currvx->node->local){
			LOG("OOPS_update_sol_x_parent_only -- em["<<currem->name<<"]  - solution is not on this proc");
		}

		int ci = path[i];
		Vector* vxs = SubVector(currvx,currem->children.size());
		DenseVector* dx = GetDenseVectorFromVector(vxs);
		currem->update_primal_x(dx->elts);

		currvx = SubVector(currvx,ci);
		currem = currem->children.at(ci);
	}
}

/*
 * Update the related Y vector
 */

void OOPS_update_y(ExpandedModel* emrow)
{
	vector<int> path;
	Vector* sub_vy = find_sub_vector(ExpandedModel::root,prob_ptr->x,emrow,path);
	OOPS_update_sol_y_all_bellow(emrow,sub_vy);
	OOPS_update_y_parent_only(emrow, path);
}

void OOPS_update_sol_y_all_bellow(ExpandedModel* em, Vector *vy)
{
	Tree * tx = vy->node;
	if(!tx->local)
	{
		LOG("OOPS_update_sol_x_all_bellow -- em["<<em->name<<"]  - solution is not on this proc");
	}
	int nchild = em->children.size();
	assert((nchild == 0 && tx->nb_sons==0) || tx->nb_sons == nchild + 1);
	if(nchild==0)
	{
		DenseVector *dy = GetDenseVectorFromVector(vy);
		assert(dy->dim == em->numLocalCons);
		em->update_primal_x(dy->elts);
	}
	else
	{
		// if there is child, the vx has size of nchild+1 node.
		// the vxs is stored at index = nchild
		Vector* vys = SubVector(vy,nchild);
		DenseVector* dy = GetDenseVectorFromVector(vys);
		assert(dy->dim == em->numLocalCons);
		em->update_lag(dy->elts);
		for(int i=0;i<nchild;i++){
			ExpandedModel* child = em->children.at(i);
			OOPS_update_sol_y_all_bellow(child,SubVector(vy,i));
		}
	}
}

void OOPS_update_y_parent_only(ExpandedModel* emrow, vector<int>& path)
{
	assert((emrow==ExpandedModel::root&&path.size()==0) && true);

	ExpandedModel* currem = ExpandedModel::root;
	Vector* currvy = prob_ptr->y;
	for(int i=0;i<path.size();i++)
	{
		if(!currvy->node->local){
			LOG("OOPS_update_sol_x_parent_only -- em["<<currem->name<<"]  - solution is not on this proc");
		}

		int ci = path[i];
		Vector* vys = SubVector(currvy,currem->children.size());
		DenseVector* dy = GetDenseVectorFromVector(vys);
		currem->update_lag(dy->elts);

		currvy = SubVector(currvy,ci);
		currem = currem->children.at(ci);
	}
}


/* ---------------------------------------------------------------------------
 CallBackFunction: FillObjVector
 ---------------------------------------------------------------------------- */
void FillObjVector(Vector *vc) {
	Tree *T = vc->node;
	DenseVector *dense = GetDenseVectorFromVector(vc);

	Algebra *A = (Algebra*) T->nodeOfAlg; // the diagonal node that spawned this tree
	OOPSBlock *obl = (OOPSBlock*) A->id;        // and its id structure

	assert(obl->emcol->numLocalVars == T->end - T->begin);

	vector<double> vals;
	obl->emrow->obj_grad_local(obl->emrow, vals);
	for (uint i = 0; i < vals.size(); i++)
		dense->elts[i] = vals[i];
}

/* ---------------------------------------------------------------------------
 CallBackFunction: FillUpBndVector
 ---------------------------------------------------------------------------- */
void FillUpBndVector(Vector *vu) {
	Tree *T = vu->node;
	DenseVector *dense = GetDenseVectorFromVector(vu);

	Algebra *A = (Algebra *) T->nodeOfAlg; // the diagonal node that spawned this tree
	//NodeId *id = (NodeId*)A->id;        // and its id structure
	OOPSBlock *obl = (OOPSBlock*) A->id;        // and its id structure
	assert(obl->emrow == obl->emcol); // this should be a diagonal block
	//NlFile *nlf = obl->emrow->nlfile;
	ExpandedModel *emrow = obl->emrow;

	assert(obl->emcol->numLocalVars == T->end - T->begin);
	emrow->get_local_var_up_bounds(dense->elts);
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

	// FIXME: should the id structure include information on the ExpandedModelInterface
	//        as well? That way we could do some more sanity checks

	emrow->get_local_cons_bounds(dense->elts, checkub);

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
	emrow->get_local_var_low_bounds(dense->elts);

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
