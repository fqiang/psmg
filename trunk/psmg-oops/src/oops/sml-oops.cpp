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

#include "../sml/GlobalVariables.h"
#include "../sml/Sml.h"

#include "OOPSBlock.h"
#include "sml-oops.h"
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <cassert>

using namespace std;

// C++ static variables defined in an *. file need to be declared here as well
static Algebra *createA(ExpandedModel *em);
static Algebra *createQ(ExpandedModel *em);

FILE *globlog = NULL;
FILE *printout = stdout;

void FillRhsVector(Vector *vb);
void FillObjVector(Vector *vc);
void FillUpBndVector(Vector *vu);
void FillLowBndVector(Vector *vl);
void SML_OOPS_upload_sol(ExpandedModel *em, Vector *vx, Vector *vy,Vector *vz);

void SML_OOPS_driver(ExpandedModel *root)
{
	Algebra *AlgAug;
	Vector *vb, *vc, *vu, *vl;

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

	if (GlobalVariables::writeMatlab) {
		LOG("Writing matlab file:mat.m");
		FILE *mout = fopen("mat.m", "w");
		PrintMatrixMatlab(mout, A, "A");
		PrintMatrixMatlab(mout, Q, "Q");
		vb->printMatlab(mout, "b");
		vc->printMatlab(mout, "c");
		vu->printMatlab(mout, "bu");
		vl->printMatlab(mout, "bl");
		fclose(mout);
	}

	if (GlobalVariables::writeMPS) {
		FILE *mps_file;
		mps_file = fopen("test.mps", "w");
		Write_MpsFile(mps_file, AlgAug, vb, vc, vu, vl, 1, NULL, NULL);
		fclose(mps_file);
	}

	Vector *vx, *vy, *vz;
	PrintOptions Prt(1);

	vx = new Vector(A->Tcol, "vx");
	vy = new Vector(A->Trow, "vy");
	vz = new Vector(A->Tcol, "vz");

	PDProblem Prob(AlgAug, vb, vc, vl, vu, vx, vy, vz);
	TIMER_STOP("OOPS_PROBLEM_SETUP");

	if(GlobalVariables::solve)
	{
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

static Algebra* createBottom(ExpandedModel *diag,
                             ExpandedModel *offdiag);
static Algebra* createRhs(ExpandedModel *diag,
                          ExpandedModel *offdiag);
//static Algebra* createBottomQ(ExpandedModel2 *diag,
//                              ExpandedModel2 *offdiag);
//static Algebra* createRhsQ(ExpandedModel2 *diag,
//                           ExpandedModel2 *offdiag);
static void SMLCallBack(CallBackInterfaceType *cbi);
static void SMLCallBackQ(CallBackInterfaceType *cbi);

/* --------------------------------------------------------------------------
createA
--------------------------------------------------------------------------- */
/* This routine sets up the matrix A from the ExpandedModelInterface tree */
Algebra* createA(ExpandedModel *em)
{
	LOG("createA for em->getName ["<<em->getName()<<"]");
	Algebra *Alg;

	//if (!A->localVarInfoSet) A->setLocalVarInfo();
	if (em->children.size()==0)
	{
		// this is final node: read in *.nl file to get dimensions
		LOG( "creatA leaf node: ");
		OOPSBlock *obl = new OOPSBlock(em, em);

		if (em->getNLocalCons()<0){
			printf("CreateA: local block has %d constraints: Not initialised?\n",
				 em->getNLocalCons());
			exit(1);
		}
		Alg = NewAlgebraSparse(em->getNLocalCons(), em->getNLocalVars(),
									   (em->getName()+":"+em->getName()).c_str(),
									   (CallBackFunction)SMLCallBack, obl);
	}
	else
	{
		/* this is a complex node, set up DblBordDiag with
		   - Diagonals from children (call createA again)
		   - Off-diagonals with *.nl file from children and col file from parent
		   - bottom from this *.nl file and col from the children              */

		LOG("SMLOOPS: Create complex node: (" << em->getNLocalCons() << "x" <<
			em->getNLocalVars() << ") nchd = " <<em->children.size());

		/* every child is a diagonal block */
		Algebra **D, **B, **R;
		int nblk, i;

		nblk = (em->children).size();
		D = (Algebra **)calloc(nblk+1, sizeof(Algebra *));
		B = (Algebra **)calloc(nblk, sizeof(Algebra *));
		R = (Algebra **)calloc(nblk, sizeof(Algebra *));

		for(i=0; i<nblk; i++)
		{
			LOG("--------------------------------------------- em["<<em->getName()<<"] ["<<i<<"]");
			D[i] = createA((em->children).at(i));
			B[i] = createBottom(em, (em->children).at(i));
			R[i] = createRhs(em, (em->children).at(i));
			LOG("--------------------------------------------- end em["<<em->getName()<<"] ["<<i<<"]");
		}

		/* The final D[nblk] block is defined by local constraints/variables */

		// this is final node: read in *.nl file to get dimensions
		// I suspect we can just copy in the code from the leaf node case above

		OOPSBlock *obl = new OOPSBlock(em, em);
		D[nblk] = NewAlgebraSparse(em->getNLocalCons(), em->getNLocalVars(),
					(em->getName() + ":" + em->getName()).c_str(),
					(CallBackFunction) SMLCallBack, obl);

		Alg = NewAlgebraDblBordDiag(nblk, B, R, D,
									(em->getName()+":"+em->getName()).c_str());
	}

	return Alg;
}


Algebra* createBottom(ExpandedModel *diag, ExpandedModel *nondiag)
{
	LOG("createBottom for diag["<<diag->getName()<<"] nondiag["<<nondiag->getName()<<"]");
	Algebra *Alg;
	/* This is a bottom block:
	 take the local constraints from the diag block and
	 follow tree defined from the non-diag block */

	if (nondiag->children.size()==0)
	{
		LOG("createBottom leaf node ");
		OOPSBlock *obl = new OOPSBlock(diag, nondiag);
		Alg = NewAlgebraSparse(diag->getNLocalCons(), nondiag->getNLocalVars(),
				   (diag->getName()+":"+nondiag->getName()).c_str(),
				   (CallBackFunction)SMLCallBack, obl);
	}
	else
	{
		// this is going to be a BlockDense Algebra
		int nblk = nondiag->children.size();
		Algebra **B = (Algebra **)calloc(nblk+1, sizeof(Algebra *));

		for(int i=0;i<nblk;i++)
		{
			B[i] = createBottom(diag, (nondiag->children).at(i));
		}

		// The right most block is made up of the diag nodes amplfile
		// and the variables from this nondiag node

		//NodeId *id = new NodeId();

		OOPSBlock *obl = new OOPSBlock(diag, nondiag);

		B[nblk] = NewAlgebraSparse(diag->getNLocalCons(),nondiag->getNLocalVars(),
				   (diag->getName()+":"+nondiag->getName()).c_str(),
				   (CallBackFunction)SMLCallBack, obl);
		Alg = NewAlgebraBlockDense(1, nblk+1, B,
						  (diag->getName()+":"+nondiag->getName()).c_str());
	}
	return Alg;
}

Algebra* createRhs(ExpandedModel *diag, ExpandedModel *nondiag)
{
	LOG("createRhs for diag["<<diag->getName()<<"] nondiag["<<nondiag->getName()<<"]");
	Algebra *Alg;
	/* This is a bottom block:
	 take the local variables from the diag block and
	 follow tree defined from the non-diag block */

	if (nondiag->children.size()==0)
	{
		LOG("createRhs leaf node ");
		OOPSBlock *obl = new OOPSBlock(nondiag, diag);
		Alg = NewAlgebraSparse(nondiag->getNLocalCons(), diag->getNLocalVars(),
			   (nondiag->getName()+":"+diag->getName()).c_str(),
			   (CallBackFunction)SMLCallBack, obl);

	}
	else
	{
		// this is going to be a BlockDense Algebra
		int nblk = nondiag->children.size();
		Algebra **B = (Algebra **)calloc(nblk+1, sizeof(Algebra *));

		for(int i=0;i<nblk;i++)
		{
			B[i] = createRhs(diag, (nondiag->children).at(i));
		}

		// The bottom node is made from this node's amplfile and the variables
		// defined in diag
		OOPSBlock *obl = new OOPSBlock(nondiag, diag);

		B[nblk] = NewAlgebraSparse(nondiag->getNLocalCons(),diag->getNLocalVars(),
							   (nondiag->getName()+":"+diag->getName()).c_str(),
								   (CallBackFunction)SMLCallBack, obl);
		Alg = NewAlgebraBlockDense(nblk+1, 1, B,
						   (nondiag->getName()+":"+diag->getName()).c_str());
	}

	return Alg;
}

/* --------------------------------------------------------------------------
createQ
--------------------------------------------------------------------------- */
/* This routine sets up the matrix Q from the ExpandedModelInterface tree */
Algebra *
createQ(ExpandedModel *em)
{
  Algebra *Alg;

  //if (!em->localVarInfoSet) em->setLocalVarInfo();
  if (em->children.size()==0){
    // this is final node: read in *.nl file to get dimensions 

    Alg = NewAlgebraSparse(em->getNLocalVars(), em->getNLocalVars(), 
                           ("Q"+em->getName()+":"+em->getName()).c_str(),
                           (CallBackFunction)SMLCallBackQ, em);
      
      
  }
  else
  {
    // Ok, should really test whether there is a DblBordDiagMatrix needed
    // or if a BlockDiagMatrix will do:
    // - Are there any cross products in the objective bewteen the 
    //   variables local to this node and the variables defined in its 
    //   children?

    /* If there are cross-products then the variables of the children
       that are refered to are part of the AMPL model at this node, but
       they are not part of the nvar/lvar list of local variables 

       AMPL will give us 
         sputinfo->hcolstarts
         sputinfo->hrownos
       for the Hessian defined in this model file. This could be either
       upper triangle only or the full matrix

       We can scan this for cross products (i.e. go through the columns
       corresponding to the local variables are see if there are any entries
       in rows corresponding to children). 
       If so then set up a DblBordDiagMatrix and let the child work out
       which of these entries are his.
       The child gets passed both ExpandedModelInterfaces (for itself and the parent)
       The objective however MUST be included in the parent part

    */
    
    /* this is a complex node, set up DblBordDiag with
       - Diagonals from children (call createQ again)
       - Off-diagonals with *.nl file from children and col file from parent
       - bottom from this *.nl file and col from the children              */



      /* every child is a diagonal block */
      Algebra **D;
      int nblk, i;
      
      nblk = (em->children).size();
      D = (Algebra **)calloc(nblk+1, sizeof(Algebra *));
      
      for(i=0; i<nblk; i++){
        D[i] = createQ((em->children).at(i));
      }

      /* The final D[nblk] block is defined by local constraints/variables */
    
      // this is final node: read in *.nl file to get dimensions 
      // I suspect we can just copy in the code from the leaf node case above 


      D[nblk] = NewAlgebraSparse(em->getNLocalVars(), em->getNLocalVars(), 
                              ("Q"+em->getName()+":"+em->getName()).c_str(),
                                 (CallBackFunction)SMLCallBackQ, em);

      Alg = NewAlgebraBlockDiag(nblk+1, D, 
                        ("Q"+em->getName()+":"+em->getName()).c_str()); 
  }

  return Alg;
}

/* ---------------------------------------------------------------------------
CallBackFunction: SMLCallBack
---------------------------------------------------------------------------- */
void
SMLCallBack(CallBackInterfaceType *cbi)
{
  /* This needs to be able to fill in the local sparse nodes with the 
     information coming from the ampl file. It can be called either
     for the diagonal nodes or for the off-diagonal nodes

     It needs to know
     - name of the ampl file
     - list and number of variables to use

  */
  
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
  
  /* id is a pointer to NodeId with components
      string amplfile;
      int nvar;
      int *lvar;
  */
	LOG("SMLCallBack - called -- ");

	OOPSBlock *obl = (OOPSBlock*) cbi->id;

	//NodeId *id = (NodeId*)cbi->id;
	if (cbi->row_nbs == NULL)
	{
		LOG("only want number of nonzeros jacobians - ");
		cbi->nz = obl->emrow->getNzJacobianOfIntersection(obl->emcol);
		//assert(nz==cbi->nz);
	}
	else
	{
		LOG("want to fill in matrices");
		obl->emrow->getJacobianOfIntersection(obl->emcol, cbi->col_beg,
				cbi->col_len, cbi->row_nbs, cbi->element);
	}
	LOG("end SMLCallBack --");
}

/* ---------------------------------------------------------------------------
CallBackFunction: SMLCallBack
---------------------------------------------------------------------------- */
void
SMLCallBackQ(CallBackInterfaceType *cbi)
{
  /* This needs to be able to fill in the local sparse nodes with the 
	 information coming from the ampl file. It can be called either
	 for the diagonal nodes or for the off-diagonal nodes

	 It needs to know
	 - name of the ampl file
	 - list and number of variables to use

	 */

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

	/* id is a pointer to NodeId with components
	 string amplfile;
	 int nvar;
	 int *lvar;
	 */

	/* need to know:
	 - the amplfile (better the NlFile structure
	 (the one that has the info on the objective = the diagonal file)
	 - the variable list for the diagonal part
	 - the variable list for the nondiagonal part
	 */
	LOG("SMLCalBackQ - called");
	ExpandedModel *em = (ExpandedModel*) cbi->id;
	cbi->nz = 0;
	if (cbi->row_nbs)
	{
		cbi->col_beg[em->getNLocalVars()] = cbi->nz;
		for (int i = 0; i < em->getNLocalVars(); i++)
		{
			cbi->col_len[i] = cbi->col_beg[i + 1] - cbi->col_beg[i];
		}
	}
	LOG("end SMLCalBackQ - called");

}



/* ---------------------------------------------------------------------------
CallBackFunction: FillObjVector
---------------------------------------------------------------------------- */
void
FillObjVector(Vector *vc)
{
  Tree *T = vc->node;
  DenseVector *dense = GetDenseVectorFromVector(vc);

  Algebra *A = (Algebra*)T->nodeOfAlg; // the diagonal node that spawned this tree
  OOPSBlock *obl = (OOPSBlock*)A->id;        // and its id structure

  assert(obl->nvar==T->end-T->begin);

  obl->emrow->getObjGradient(dense->elts);
}

/* ---------------------------------------------------------------------------
CallBackFunction: FillUpBndVector
---------------------------------------------------------------------------- */
void
FillUpBndVector(Vector *vu)
{
  Tree *T = vu->node;
  DenseVector *dense = GetDenseVectorFromVector(vu);

  Algebra *A = (Algebra *)T->nodeOfAlg; // the diagonal node that spawned this tree
  //NodeId *id = (NodeId*)A->id;        // and its id structure
  OOPSBlock *obl = (OOPSBlock*)A->id;        // and its id structure
  assert(obl->emrow==obl->emcol); // this should be a diagonal block
  //NlFile *nlf = obl->emrow->nlfile;
  ExpandedModel *emrow = obl->emrow;

  assert(obl->nvar==T->end-T->begin);
  //nlf->getColUpBoundsAMPL(obl->nvar, obl->lvar, test);
  emrow->getColUpBounds(dense->elts);
}

/* ---------------------------------------------------------------------------
CallBackFunction: FillRhsVector
---------------------------------------------------------------------------- */
void
FillRhsVector(Vector *vb)
{
  Tree *T = vb->node;
  DenseVector *dense = GetDenseVectorFromVector(vb);

  double *checkub = new double[dense->dim];

  Algebra *A = (Algebra*)T->nodeOfAlg; // the diagonal node that spawned this tree
  OOPSBlock *obl = (OOPSBlock*)A->id; // and its id structure
  ExpandedModel *emrow = obl->emrow;

  // FIXME: should the id structure include information on the ExpandedModelInterface
  //        as well? That way we could do some more sanity checks

  emrow->getRowBounds(dense->elts, checkub);

  // check that lower and upper constraint bounds are the same due to the
  // OOPS restriction
  for(int i=0;i<dense->dim; i++){
    if (fabs(dense->elts[i]-checkub[i])>1e-6){
      cerr << "At the moment OOPS only supports equality constraints!\n";
      cerr << "Bounds for c/s " << i << " in " << emrow->getName() << ": " <<
             dense->elts[i] << " " <<  checkub[i] << endl;
      exit(1);
    }
  }

  delete [] checkub;
}

/* ---------------------------------------------------------------------------
CallBackFunction: FillLowBndVector
---------------------------------------------------------------------------- */
void
FillLowBndVector(Vector *vl)
{
  Tree *T = vl->node;
  DenseVector *dense = GetDenseVectorFromVector(vl);

  Algebra *A = (Algebra *)T->nodeOfAlg; // the diagonal node that spawned this tree
  OOPSBlock *obl = (OOPSBlock*)A->id;        // and its id structure
  assert(obl->emrow==obl->emcol); // this should be a diagonal block
  ExpandedModel *emrow = obl->emrow;


  assert(obl->nvar==T->end-T->begin);
  emrow->getColLowBounds(dense->elts);

  //for(int i=0;i<dense->dim;i++){
  //  if (fabs(dense->elts[i])>1e-6) {
  //    printf("Found lower bound !=0 (=%f) in variable %i in model %s",
  //           dense->elts[i], i, emrow->getName().c_str());
  //    printf("Currently OOPS can only cope with zero lower bounds\n");
  //    exit(1);
  //  }
  // }

}

/* ---------------------------------------------------------------------------
SML_OOPS_upload_sol
---------------------------------------------------------------------------- */
void
SML_OOPS_upload_sol(ExpandedModel *em,
		    Vector *vx, Vector *vy, Vector *vz)
{
	Tree *Tx = vx->node,*Ty = vy->node;
	int nchd = em->children.size();

	//printf("%d: %d %d\n",nchd, Tx->nb_sons, Ty->nb_sons);
	assert((nchd==0&&Tx->nb_sons==0)||Tx->nb_sons==nchd+1);
	assert((nchd==0&&Ty->nb_sons==0)||Ty->nb_sons==nchd+1);
	if (nchd>0)
	{
		/* The final child of the vector tree corresponds to the local variables/
		   constraints of this node */

		/* upload local contributions */
		Vector *vxs = SubVector(vx, nchd);
		Vector *vys = SubVector(vy, nchd);
		Vector *vzs = SubVector(vz, nchd);

		DenseVector *dx = GetDenseVectorFromVector(vxs);
		DenseVector *dy = GetDenseVectorFromVector(vys);
		DenseVector *dz = GetDenseVectorFromVector(vzs);

		assert(dx->dim == em->getNLocalVars());
		assert(dy->dim == em->getNLocalCons());

		em->setPrimalSolColumns(dx->elts);
		em->setDualSolColumns(dz->elts);
		em->setDualSolRows(dy->elts);

		/* and upload a vector of zeros for the constraint slacks
		   (OOPS can only deal with equality constraints)                    */
		double *elts = new double[dy->dim];
		for(int i=0;i<dy->dim;i++) elts[i] = 0;
		em->setPrimalSolRows(elts);
		delete[] elts;

		/* recurse down the rest of the tree */

		for (int i=0;i<nchd;i++)
		{
			ExpandedModel *model = em->children[i];
			SML_OOPS_upload_sol(model,
				  SubVector(vx, i), SubVector(vy, i), SubVector(vz,i));
		}
	}
	else
	{
		/* This is a root node of the model tree and the vector trees */
		DenseVector *dx = GetDenseVectorFromVector(vx);
		DenseVector *dy = GetDenseVectorFromVector(vy);
		DenseVector *dz = GetDenseVectorFromVector(vz);

		assert(dx->dim == em->getNLocalVars());
		assert(dy->dim == em->getNLocalCons());

		em->setPrimalSolColumns(dx->elts);
		em->setDualSolColumns(dz->elts);
		em->setDualSolRows(dy->elts);

		/* and upload a vector of zeros for the constraint slacks
		   (OOPS can only deal with equality constraints)                    */
		double *elts = new double[dy->dim];
		for(int i=0;i<dy->dim;i++) elts[i] = 0;
		em->setPrimalSolRows(elts);
		delete[] elts;
	}
}
