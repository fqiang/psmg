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
#ifndef SML_OOPS_H
#define SML_OOPS_H
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
#include "../model/AmplModel.h"
#include "../context/ColSparseMatrix.h"
#include "../util/util.h"
#include "../sml/Sml.h"


#include "OOPSBlock.h"

/*
 * Structure setting up
 */
Algebra *createA(ExpandedModel *em);
Algebra *createQ(ExpandedModel *em);


Algebra* createBottomBord(ExpandedModel *emrow, ExpandedModel *emcol);
Algebra* createRHSBord(ExpandedModel *emrow, ExpandedModel *emcol);
Algebra* createBottmBordQ(ExpandedModel *emrow, ExpandedModel *emcol);
Algebra* createRHSBordQ(ExpandedModel *emrow, ExpandedModel *emcol);



/*
 * cons bounds, var bounds are the same for both DIST and LOCAL.
 */
void fill_cons_bounds(Vector *vb);
void fill_var_ubound(Vector *vu);
void fill_var_lbound(Vector *vl);

/*
 * cons value fillup
 */
void fill_cons_val_local(Vector* vcon);



/*
 * Call back for Matrix A,Q  -- NLP
 */
void psmg_callback_a_nlp(CallBackInterfaceType *cbi);
void psmg_callback_q_nlp(CallBackInterfaceType *cbi);


/*
 * Call back for Matrix A,Q -- LP
 */
void psmg_callback_a_lp_qp(CallBackInterfaceType *cbi);
void psmg_callback_q_lp_qp(CallBackInterfaceType *cbi);

/*
 * main driver for NLP and LP QP
 */
void SML_OOPS_driver_NLP(ExpandedModel *root);
void SML_OOPS_driver_LP_QP(ExpandedModel *root);

#endif
