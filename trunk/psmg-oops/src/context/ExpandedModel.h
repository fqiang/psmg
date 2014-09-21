/*
 * ExpandedModel2.h
 *
 *  Created on: 17 Oct 2011
 *      Author: s0965328
 */

#ifndef EXPANDEDMODEL_H_
#define EXPANDEDMODEL_H_

#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/foreach.hpp>
#include "../autodiff/autodiff.h"

#include "ColSparseMatrix.h"
#include "ModelContext.h"

class ModelComp;
class AmplModel;
class ConsComp;
class VarComp;
class BlockA;
class BlockObj;
class BlockHV;


using namespace std;


//1. the model comp of the dummy var referred to. 2. the string value of this dummy var at current em.
typedef std::pair<ModelComp*, string> model_dummy_t;
typedef enum {LP=0, QP, NLP, UNDEFINED} ProbType;
typedef enum {LOCAL=0,DIST} InterfaceType;

class ExpandedModel
{
public:
	static ExpandedModel *root;
	static ProbType ptype;
	static InterfaceType itype;
	static uint n_col;
	static uint n_row;
	static double* X0; //if set the default value for X (decision variables)
	static double* Y0; //if set the default value for Y (decision variables)

	static double* X;
	static double* Y;
	static double ONE;


	AmplModel* model;   //the prototype model
	ExpandedModel *parent;
	ModelContext ctx;  //data is hold

	vector<ExpandedModel*> children;
	uint colbeg; //for access X, primal variable
	uint rowbeg; //for access Y, dual variable
	uint numLocalVars;
	uint numLocalCons;
	string name;

	bool isDepEmsInitialized;


	//! the map from dummy variable for constructing this instance of EM.
	//  block ID {a in ARCS} -- a --> ARCS
//	boost::unordered_map<string,ModelComp*> dummySetMap;
	//! the map from dummy variable to the value of this variable for this local problem.
	//  a ---> (Comp* "A1")
	boost::unordered_map<string,model_dummy_t> dummyMap;

	//Jacobian block (this x emcol) evaluation
	//used for computing Jacobian block and constrain values
	//Jacobian block is computed locally for LP QP problems as they are linear and separable
	//mpi reduce operation is need for computing Jacobian block of NLP problem.
	//Constraints values are computed always requiring of MPI reduce operation.
	BlockA* getBlockA_LP_QP(ExpandedModel* emcol);
	boost::unordered_map<ExpandedModel*, BlockA*> lp_qp_cblockMap;    //constructed using partial attributes

	//the Q matrix created using partial attributes - higher order expression only!
	BlockObj* getBlockQ_QP(ExpandedModel* emcol);
	boost::unordered_map<ExpandedModel*, BlockObj*> qp_oblockMap;    //constructed using partial attributes
																		// this is for Q matrix of QP problem.
	//! Return Constraint Block defined by (constraints in this) X (variables in emcol)
	//! Creat one if this is not already created
	BlockA* getBlockA_ConsFull();
	BlockA* consFull;	//constructed using full constraints attributes

	//used for computing objective value
	//objective value will be computed using mpi reduce operation. (same as constraint value)
	BlockObj* getBlockObj_Full();
	BlockObj* objFull;  //oblockFull is set when
						//1. model->obj_comp is declared and
						//2. after first call of function eval of obj function.

	//NLP below!
	//the constraint block created using partial attributes, this is only used for cons_jacobs_nlp evaluation
	BlockA* getBlockA_NLP_Local(ExpandedModel* emcol);
	boost::unordered_map<ExpandedModel*, BlockA*> nlp_cblockMap_local;  //constructed using partial attributes

	//! Hessian of Lagrangian block
	//the hv block created using blockA from lp_qp_nlp_cblockMap, and BlockObj from qp_nlp_oblockMap.
	BlockHV* getBlockHV_NLP_Full(ExpandedModel* emcol);
	boost::unordered_map<ExpandedModel*, BlockHV*> nlp_hvblockMap_local;    //constructed using full constraints + objective attributes

	BlockObj* getBlockObj_Grad();
	BlockObj* objGrad;

	ExpandedModel(AmplModel* _mod,ExpandedModel* par);
	virtual ~ExpandedModel();


	//LP and QP Interface - specific
	//these methods return the Jacobian block of LP QP problem. This is computed based on the linear constraints, therefore no
	//distributed communication is needed.

	//For LP QP, the local/distribute interface has same effect
	uint nz_cons_jacobs_lp_qp(ExpandedModel* emcol);
	void cons_jacobs_lp_qp(ExpandedModel* emcol,col_compress_matrix& m);
	//! Returns the objective gradient for this expanded model w.r.t. local vars
	//! assume objective only use variable declared in it's own expanded model
	void obj_grad_c_lp_qp(double* vals);

	//Request second order term only place in the expanded model where local variable is initialised
	//The cross-product term placed at the highest level possible. (ie. x_0*x_1 placed at the level where x_1 is declared, assume
	//x_0 is the root/complicating variable  , this allows hess_qp distribute/local has same effect. It is worth to note
	//computing Hessian of objective function do not require current x values.
	//if this == emcol, computing the Hessian block for this local variable only
	uint nz_obj_hess_qp(ExpandedModel* emcol);
	void obj_hess_qp(ExpandedModel* emcol, col_compress_matrix& m);


	//LP, QP and NLP
	//! evaluate the constraint function value using build constraint function entirely locally
	void cons_feval_local(double* fvals);
	//！ evaluate the objective function value by summing over all expanded model nodes below this
	double& obj_feval_local(double& oval);


	//NLP interface - specific
	//! Return the nonzeros in the Jacobian of a section of the model
	uint nz_cons_jacobs_nlp_local(ExpandedModel* emcol);
	void cons_jacobs_nlp_local(ExpandedModel *emcol, col_compress_matrix& m);
	uint nz_lag_hess_nlp_local(ExpandedModel* emcol);
	void lag_hess_nlp_local(ExpandedModel* emcol,col_compress_matrix&);
	void obj_grad_nlp_local(double* vals);

	//Distribute interface
	//NLP
	//！ Return the nonzeros in the Jacobian of a section of the model
	uint nz_cons_jacobs_nlp_dist(ExpandedModel* emcol);
	void cons_jacobs_nlp_dist(ExpandedModel *emcol, col_compress_matrix& m);
	uint nz_lag_hess_nlp_dist(ExpandedModel* emcol);
	void lag_hess_nlp_dist(ExpandedModel* emcol,col_compress_matrix&);


	//LP, QP and NLP - Distribute interface
	//Function evaluation for constraints declared at this exapaned model
	void cons_feval_dist(ExpandedModel* col, double* fvals);
	//Objective function evaluation for objective declared at this expanded model
	double& obj_feval_dist(ExpandedModel* emcol, double& oval);

	/*
	 * current point update
	 * used in NLP interface only
	 */
	//! Setting the primal variable soln / the values for the local variables
	void update_primal_x(double *elts);
	//! Setting the lagrangian mulitpler for local constraints
	void update_lag(double* elts);

	/*
	 * Common inteface methods for Both Local and Distributed Inteface calls
	 */
	//! Returns the vector of upper bounds for the local variables in this model
	void get_var_up_bounds(double *elts);
	//! Returns the vector of lower bounds for the local variables in this model
	void get_var_low_bounds(double *elts);
	//! Return the arrays of bounds for the constraints in this model
	void get_cons_bounds(double *lower, double *upper);
	//! Returns the names of local variables
	void get_vars_names(vector<string>&);
	//! Returns the names of local constraints
	void get_cons_names(vector<string>&);


	/*
	 * Helper methods
	 */

	void initFullDepEms(); //every node above and below include this
	void getAllEM(vector<ExpandedModel*>& ems);
	void getParentEM(vector<ExpandedModel*>& ems);
	void addModelDummy(string& dummyVar, ModelComp* comp, string value);
	void addChildren(ExpandedModel* em2);
	void levelTraversal(vector<ExpandedModel*>& em2List,int level);
	static void convertToColSparseMatrix(col_compress_matrix& m,ColSparseMatrix& sm);
	/*
	 * Utility methods
	 */
	void setIndex(int& rbeg, int& cbeg);
	void dropCtxRecKeepRoot();
	ModelContext* locateChildCtx(AmplModel* model, string& dummyval);
	void logEMRecursive(string& line,ostream&);
	void calculateMemoryUsage(unsigned long& size);
	string qualifiedName();
private:
//	ModelContext* recursiveInitContext();
	void copyVariables(boost::unordered_set<AutoDiff::Node*>&);
	void copyVariables(std::vector<AutoDiff::Node*>&);
};

#endif /* EXPANDEDMODEL_H_ */
