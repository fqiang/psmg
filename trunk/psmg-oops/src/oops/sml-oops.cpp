/*
 * sml-oops.cpp
 *
 *  Created on: 18 Sep 2014
 *      Author: s0965328
 */


#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>

#include "sml-oops.h"

CallBackFunction cbf_a()
{
	if(ExpandedModel::ptype == LP || ExpandedModel::ptype == QP) {
		return (CallBackFunction)(psmg_callback_a_lp_qp);
	}
	else{
		assert(ExpandedModel::ptype == NLP);
		return (CallBackFunction)(psmg_callback_a_nlp);
	}
}

CallBackFunction cbf_q()
{
	if(ExpandedModel::ptype == LP || ExpandedModel::ptype == QP) {
		return (CallBackFunction)(psmg_callback_q_lp_qp);
	}
	else{
		assert(ExpandedModel::ptype == NLP);
		return (CallBackFunction)(psmg_callback_q_nlp);
	}
}
/* --------------------------------------------------------------------------
 createA
 --------------------------------------------------------------------------- */
/* This routine sets up the matrix A from the ExpandedModelInterface tree */
Algebra* createA(ExpandedModel *em) {
	TRACE("createA for em->getName ["<<em->name<<"]");
	Algebra *Alg = NULL;
	if (em->children.size() == 0) {
		TRACE("creatA leaf node: ");
		OOPSBlock *obl = new OOPSBlock(em, em);
		Alg = NewAlgebraSparse(em->numLocalCons, em->numLocalVars,
				(em->name + ":" + em->name).c_str(),
				*cbf_a(), obl);
	}
	else
	{
		/* this is a complex node, set up DblBordDiag with
		 - Diagonals from children (call createA again)
		 - Off-diagonals with *.nl file from children and col file from parent
		 - bottom from this *.nl file and col from the children              */

		TRACE("PSMG - Create complex node:["<< em->numLocalCons<<"] x ["<< em->numLocalVars << "] nchd = " <<em->children.size());

		/* every child is a diagonal block */
		Algebra **D, **B, **R;
		int nblk, i;

		nblk = (em->children).size();
		D = (Algebra **) calloc(nblk + 1, sizeof(Algebra *));
		B = (Algebra **) calloc(nblk, sizeof(Algebra *));
		R = (Algebra **) calloc(nblk, sizeof(Algebra *));

		for (i = 0; i < nblk; i++) {
			TRACE("--------------------------------------------- em["<<em->name<<"] ["<<i<<"]");
			D[i] = createA((em->children).at(i));
			B[i] = createBottomBord(em, (em->children).at(i));
			R[i] = createRHSBord((em->children).at(i),em);
			TRACE("--------------------------------------------- end em["<<em->name<<"] ["<<i<<"]");
		}

		/* The final D[nblk] block is defined by local constraints/variables */

		// this is final node: read in *.nl file to get dimensions
		// I suspect we can just copy in the code from the leaf node case above
		OOPSBlock *obl = new OOPSBlock(em, em);

		D[nblk] = NewAlgebraSparse(em->numLocalCons, em->numLocalVars,
				(em->name + ":" + em->name).c_str(),
				*cbf_a(), obl);
		Alg = NewAlgebraDblBordDiag(nblk, B, R, D,
				(em->name + ":" + em->name).c_str());
	}

	return Alg;
}

Algebra* createBottomBord(ExpandedModel *emrow, ExpandedModel *emcol) {
	TRACE("createBottom for emrow["<<emrow->name<<"] emcol["<<emcol->name<<"]");
	Algebra *Alg = NULL;
	if (emcol->children.size() == 0)
	{
		TRACE("createBottom leaf node ");
		OOPSBlock *obl = new OOPSBlock(emrow, emcol);

		Alg = NewAlgebraSparse(emrow->numLocalCons, emcol->numLocalVars,
			(emrow->name + ":" + emcol->name).c_str(),
			*cbf_a(), obl);
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

		B[nblk] = NewAlgebraSparse(emrow->numLocalCons,
				emcol->numLocalVars,
				(emrow->name + ":" + emcol->name).c_str(),
				*cbf_a(), obl);
		Alg = NewAlgebraBlockDense(1, nblk + 1, B,
				(emrow->name + ":" + emcol->name).c_str());
	}
	return Alg;
}

Algebra* createRHSBord(ExpandedModel *emrow, ExpandedModel *emcol) {
	TRACE("createRhs for emrow["<<emrow->name<<"] emcol["<<emcol->name<<"]");
	Algebra *Alg = NULL;
	if (emrow->children.size() == 0) {
		TRACE("createRhs leaf node ");
		OOPSBlock *obl = new OOPSBlock(emrow, emcol);

		Alg = NewAlgebraSparse(emrow->numLocalCons, emcol->numLocalVars,
				(emrow->name + ":" + emcol->name).c_str(),
				*cbf_a(), obl);

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

		B[nblk] = NewAlgebraSparse(emrow->numLocalCons,
				emcol->numLocalVars,
				(emrow->name + ":" + emcol->name).c_str(),
				*cbf_a(), obl);
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

	if (em->children.size() == 0) {
		//diagnal node on the leaf of the problem structure
		OOPSBlock *obl = new OOPSBlock(em, em);


		Alg = NewAlgebraSparse(em->numLocalVars, em->numLocalVars,
				("Q" + em->name + ":" + em->name).c_str(),
				*cbf_q(), obl);
	} else {
		/* TODO - Design;
		 * assume the Q(x) is DblBordDiagMatrix for QP problem
		 * otherwise we will have problem to solve it in OOPS
		 *
		 */

		/* this is a complex node, set up DblBordDiag with
				 - Diagonals from children (call createA again)
				 - Off-diagonals with *.nl file from children and col file from parent
				 - bottom from this *.nl file and col from the children              */
		TRACE("PSMG - Create complex node:["<< em->numLocalVars<<"] x ["<< em->numLocalVars << "] nchd = " <<em->children.size());

		/* every child is a diagonal block */
		Algebra **D, **B, **R;
		int nblk, i;

		nblk = (em->children).size();
		D = (Algebra **) calloc(nblk + 1, sizeof(Algebra *));
		B = (Algebra **) calloc(nblk, sizeof(Algebra *));
		R = (Algebra **) calloc(nblk, sizeof(Algebra *));

		for (i = 0; i < nblk; i++) {
			TRACE("--------------------------------------------- em["<<em->name<<"] ["<<i<<"]");
			D[i] = createQ(em->children.at(i));
			B[i] = createBottmBordQ(em, (em->children).at(i)); //em's level < it's children's level
			R[i] = createRHSBordQ((em->children).at(i),em);  //B[i] and R[i] is transpose of each other.
			TRACE("--------------------------------------------- end em["<<em->name<<"] ["<<i<<"]");
		}

		OOPSBlock *obl = new OOPSBlock(em, em);


		//the parent right bottom conner block
		D[nblk] = NewAlgebraSparse(em->numLocalVars, em->numLocalVars,
				("Q"+em->name + ":" + em->name).c_str(),
				*cbf_q(), obl);

		Alg = NewAlgebraDblBordDiag(nblk, B, R, D,
				("Q"+em->name + ":" + em->name).c_str());
	}

	return Alg;
}

Algebra* createBottmBordQ(ExpandedModel *emrow, ExpandedModel *emcol) {
	TRACE("createBottmBorderQ for emrow["<<emrow->name<<"] emcol["<<emcol->name<<"]");
	Algebra *Alg;
	/* This is a bottom block:
	 take the local constraints from the diag block and
	 follow tree defined from the non-diag block */

	if (emcol->children.size() == 0)
	{
		TRACE("createBottom leaf node ");
		OOPSBlock *obl = new OOPSBlock(emrow, emcol);


		Alg = NewAlgebraSparse(emrow->numLocalVars, emcol->numLocalVars,
				("Q"+emrow->name + ":" + emcol->name).c_str(),
				*cbf_q(), obl);
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


		B[nblk] = NewAlgebraSparse(emrow->numLocalVars,emcol->numLocalVars,
				("Q"+emrow->name + ":" + emcol->name).c_str(),
				*cbf_q(), obl);
		Alg = NewAlgebraBlockDense(1, nblk + 1, B,
				("Q"+emrow->name + ":" + emcol->name).c_str());
	}
	return Alg;
}

Algebra* createRHSBordQ(ExpandedModel *emrow, ExpandedModel *emcol) {
	TRACE("createRhs for emrow["<<emrow->name<<"] emcol["<<emcol->name<<"]");
	Algebra *Alg = NULL;
	if (emrow->children.size() == 0) {
		TRACE("createRhs leaf node ");
		OOPSBlock *obl = new OOPSBlock(emrow, emcol);


		Alg = NewAlgebraSparse(emrow->numLocalVars, emcol->numLocalVars,
				("Q"+emrow->name + ":" + emcol->name).c_str(),
				*cbf_q(), obl);
	}
	else {
		// this is going to be a BlockDense Algebra
		int nblk = emrow->children.size();
		Algebra **B = (Algebra **) calloc(nblk + 1, sizeof(Algebra *));

		for (int i = 0; i < nblk; i++) {
			B[i] = createRHSBordQ((emrow->children).at(i),emcol);
		}

		OOPSBlock *obl = new OOPSBlock(emrow, emcol);


		B[nblk] = NewAlgebraSparse(emrow->numLocalVars,
				emcol->numLocalVars,
				("Q"+emrow->name + ":" + emcol->name).c_str(),
				*cbf_q(), obl);
		Alg = NewAlgebraBlockDense(nblk + 1, 1, B,
				("Q"+emrow->name + ":" + emcol->name).c_str());
	}
	return Alg;
}



/* ---------------------------------------------------------------------------
 CallBackFunction: FillRhsVector
 ---------------------------------------------------------------------------- */
void fill_cons_bounds(Vector *vb) {
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
void fill_var_lbound(Vector *vl) {
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
			TRACE("Found lower bound !=0 (=%f) in variable ["<<i<<"]"<<" in model ["<<emrow->name<<"] val["<<dense->elts[i]<<"]");
		}
	}
}


/* ---------------------------------------------------------------------------
 CallBackFunction: FillUpBndVector
 ---------------------------------------------------------------------------- */
void fill_var_ubound(Vector *vu) {
	Tree *T = vu->node;
	DenseVector *dense = GetDenseVectorFromVector(vu);

	Algebra *A = (Algebra *) T->nodeOfAlg; // the diagonal node that spawned this tree
	OOPSBlock *obl = (OOPSBlock*) A->id;        // and its id structure
	assert(obl->emrow == obl->emcol); // this should be a diagonal block
	ExpandedModel *emrow = obl->emrow;

	assert(obl->emcol->numLocalVars == T->end - T->begin);
	emrow->get_var_up_bounds(dense->elts);

	for (int i = 0; i < dense->dim; i++) {
		if (fabs(dense->elts[i]) > 1e-6) {
			TRACE("Found upper bound !=0 (=%f) in variable ["<<i<<"]"<<" in model ["<<emrow->name<<"] val["<<dense->elts[i]<<"]");
		}

	}
}



void fill_cons_val_local(Vector *vcons)
{
	Tree* T = vcons->node;
	DenseVector *dense = GetDenseVectorFromVector(vcons);
	Algebra *A = (Algebra*) T->nodeOfAlg;
	OOPSBlock *obl = (OOPSBlock*)A->id;
	assert(obl->emcol->numLocalCons == T->end - T->begin);
	assert(obl->emrow == obl->emcol);
	obl->emrow->cons_feval_local(dense->elts);
}
