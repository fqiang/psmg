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
#include "ModelContext.h"
#include "autodiff.h"
#include "ColSparseMatrix.h"
#include "../model/ConsComp.h"
#include "../model/VarComp.h"
#include "BlockCons.h"
#include "BlockObj.h"
#include "BlockHV.h"
#include "BlockLP.h"
#include "BlockObjLP.h"

using namespace std;

typedef boost::numeric::ublas::compressed_matrix<double,boost::numeric::ublas::column_major >  col_compress_matrix;
typedef boost::numeric::ublas::matrix_row<col_compress_matrix> col_compress_matrix_row;
typedef boost::numeric::ublas::matrix_column<col_compress_matrix> col_compress_matrix_col;
typedef boost::numeric::ublas::matrix_range<col_compress_matrix > col_compress_matrix_range;

//typedef boost::numeric::ublas::compressed_matrix<double>  compress_matrix;

class ExpandedModel
{
public:
	static ExpandedModel *root;

	AmplModel* model;   //the prototype model
	ModelContext* ctx;  //data is hold

	ExpandedModel *parent;

	uint numLocalVars;
	uint numLocalCons;
	string name;

	vector<ExpandedModel*> children;

	//! the map from dummy variable for constructing this instance of EM.
	//  block ID {a in ARCS} -- a --> ARCS
	boost::unordered_map<string,ModelComp*> dummySetMap;
	//! the map from dummy variable to the value of this variable for this local problem.
	//  a ---> "A1"
	boost::unordered_map<string,string> dummyValueMap;


//! Return Obj Block (objective in this) X (variables in emcol)
//	//! Creat one if this is not already created
//	BlockObj* getBlockObjPartial(ExpandedModel* emcol);
//	boost::unordered_map<ExpandedModel*, BlockObjLP*> lp_oblockMap; //constructed using partial attributes
//	BlockObj* getBlockObjFull();
//	BlockObj* oblockFull;		//constructed using full objective attributes
//	boost::unordered_map<ExpandedModel*, BlockObj*> nlp_oblockMap_lo;   //constructed using partial attributes
//	void obj_grad_lp(ExpandedModel*, double*);
//	uint nz_cons_hess_local(ExpandedModel* emcol);
//	void cons_hess_local(ExpandedModel* emcol, boost::numeric::ublas::compressed_matrix<double>&);
//	uint nz_obj_hess_local(ExpandedModel* emcol);
//	void obj_hess_local(ExpandedModel* emcol, boost::numeric::ublas::compressed_matrix<double>&);


	//! Return the block_lo
	//! Initialize the block_lo pointer if necessary
	BlockDep* getBlockDep();
	BlockDep* blockdep; 		//Block Local Dependencies for Local Interface Call

	//! Return Constraint Block defined by (constraints in this) X (variables in emcol)
	//! Creat one if this is not already created
	BlockCons* getBlockConsFull();
	BlockCons* cblockFull;	//constructed using full constraints attributes

	//! Hessian of Lagrangian block
	BlockHV* getBlockHVFull();
	BlockHV* hvblockFull;   //constructed using full constraints + objective attributes

	BlockObj* getBlockObj();
	BlockObj* blockobj;			//block objective is set 1. model->obj_comp is delcared and 2. after first call of function or derivative eval of obj function.

	//BlockLP for LP problem -- Jacobian block (this x emcol) evaluation
	//the reason for storing the BlockLP for each different emcol is that:
	// we will use the AutoDiff::Node* con saved in the BlockLP to compute nz_jacobian and jacobian of intersection
	BlockLP* getBlockLP(ExpandedModel* emcol);
	boost::unordered_map<ExpandedModel*, BlockLP*> lp_cblockMap;    //constructed using partial attributes

	//the constraint block created using partial attributes, this is only used for cons_jacobs_nlp evaluation
	BlockCons* getBlockConsPartial(ExpandedModel* emcol);
	boost::unordered_map<ExpandedModel*, BlockCons*> nlp_cblockMap_lo;  //constructed using partial attributes

	//the hv block created using *partial attributes*, this is different from attributes in getBlockConsPartial
	BlockHV* getBlockHVPartial(ExpandedModel* emcol);
	boost::unordered_map<ExpandedModel*, BlockHV*> nlp_hvblockMap_lo;   //constructed using *partial attributes*

	ExpandedModel(AmplModel* _mod,ModelContext* _context);
	virtual ~ExpandedModel();

	//Both LP and NLP
	//Function evaluation for constraints declared at this exapaned model
	void cons_feval_local(double* fvals);
	//Objective function evaluation for objective declared at this expanded model
	double& obj_feval(double& oval);
	//! Returns the objective gradient for this expanded model w.r.t. local vars
	//! assume objective only use variable declared in it's own expanded model
	void obj_grad(double* vals);

	//LP Interface - specific
	uint nz_cons_jacobs_lp(ExpandedModel* emcol);
	void cons_jacobs_lp(ExpandedModel* emcol,col_compress_matrix& m);


	//NLP interface - specific
	/*
	 * Location Interface Methods
	 */

	//! Return the nonzeros in the Jacobian of a section of the model
	uint nz_cons_jacobs_nlp_local(ExpandedModel* emcol);
	void cons_jacobs_nlp_local(ExpandedModel *emcol, col_compress_matrix& m);
	uint nz_lag_hess_nlp_local(ExpandedModel* emcol);
	void lag_hess_nlp_local(ExpandedModel* emcol,col_compress_matrix&);

//	/*
//	 * Block Distributed Dependencies for Distributed Interface Calls
//	 */
//	boost::unordered_map<ExpandedModel*, Block*> block_ds;
//	boost::unordered_map<ExpandedModel*,BlockCons* > cblockMap_ds;
//	boost::unordered_map<ExpandedModel*,BlockObj* >	oblockMap_ds;

//	/*
//	 * Distribute Interface Methods
//	 */
//	Block* getBlockDistributed(ExpandedModel* emcol);
//	//! Creating BlockCons defined by (constraints in this) X (variables in emcol)
//	BlockCons* createConsBlockDistributed(ExpandedModel * emcol);
//	//! creating BlockObj defined by (objective in this) X (variables in emcol)
//	BlockObj* constructObjBlockDistributed(ExpandedModel* emcol);
//	//! Return the function values
//	void cons_feval_distribute(ExpandedModel *emcol, std::vector<double>& fvals);
//	//! Returns the number of blocks and nonzeros in the Jacobian of a section of the model
//	int nz_jacobs_distribute(ExpandedModel* emcol, std::vector<unsigned int>& nzs);
//	//! Returns the nonzeros in the Jacobian of a section of the model
//	void cons_jacobs_distribute(ExpandedModel *emcol, std::vector<boost::numeric::ublas::compressed_matrix<double> >& blocks);
//	//! Return the nonzeros in the Hessian of a section of the model
//	int nz_hess_distribute(ExpandedModel* emcol, std::vector<unsigned int>& nzs);
//	//! Returns the nonzeros in the Hessian of a section of the model
//	void cons_hessian_distribute(ExpandedModel* emcol, std::vector<boost::numeric::ublas::compressed_matrix<double> >& blocks);


	/*
	 * current point update
	 * used in NLP interface only
	 */
	//! Setting the primal variable soln / the values for the local variables
	void update_primal_x(double *elts);
	//! Setting the lagrangian mulitpler for local constraints
	void update_lag(double* elts);
	double* y;

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
	void getAllEM(vector<ExpandedModel*>& ems);
	void getParentEM(vector<ExpandedModel*>& ems);
	void addModelDummy(string dummyVar, ModelComp* comp, string value);
	void addChildren(ExpandedModel* em2);
	void clearAllContextTreeKeepRoot();
	void levelTraversal(vector<ExpandedModel*>& em2List,int level);
	static void convertToColSparseMatrix(col_compress_matrix& m,ColSparseMatrix* sm);
	static void convertToColSparseMatrix(boost::numeric::ublas::compressed_matrix<double>& m,ColSparseMatrix* sm);
	/*
	 * Utility methods
	 */
	ModelContext* locateCtx(AmplModel* model, string& dummyval);
	void logEMRecursive(string& line,ostream&);
	void calculateMemoryUsage(unsigned long& size_str,unsigned long& size_data);
private:
	string qualifiedName();
	ModelContext* recursiveInitContext();
	void copyVariables(boost::unordered_set<AutoDiff::Node*>&);
	void copyVariables(std::vector<AutoDiff::Node*>&);
};

#endif /* EXPANDEDMODEL_H_ */
