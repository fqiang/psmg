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

using namespace std;

class ConsComp;
class VarComp;
class BlockCons;
class BlockObj;
class Block;
class BlockHV;
class BlockLP;

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

	//BlockLP for LP problem --
	//the reason for storing the BlockLP for each different emcol is that:
	// we will use the AutoDiff::Node* con saved in the BlockLP to compute nz_jacobian and jacobian of intersection
	BlockLP* getBlockLP(ExpandedModel* emcol);
	boost::unordered_map<ExpandedModel*, BlockLP*> lpBlockMap;

	//Block Local Dependencies for Local Interface Calls
	Block* block_lo;
	boost::unordered_map<ExpandedModel*, BlockHV*> hvBlockMap_lo;
	boost::unordered_map<ExpandedModel*, BlockCons*> cblockMap_lo;
	boost::unordered_map<ExpandedModel*, BlockObj*> oblockMap_lo;
	//Block Distributed Dependencies for Distributed Interface Calls
	boost::unordered_map<ExpandedModel*, Block*> block_ds;
	boost::unordered_map<ExpandedModel*,BlockCons* > cblockMap_ds;
	boost::unordered_map<ExpandedModel*,BlockObj* >	oblockMap_ds;

	ExpandedModel(AmplModel* _mod,ModelContext* _context);
	virtual ~ExpandedModel();

	//LP Interface
	uint nz_cons_jacobs(ExpandedModel* emcol);
	void cons_jacobs(ExpandedModel* emcol,boost::numeric::ublas::compressed_matrix<double>& m);
	void obj_grad(ExpandedModel*, double*);

	//NLP interface
	/*
	 * Location Interface Methods
	 */
	//! Return the block_lo
	//! Initialize the block_lo pointer if necessary
	Block* getBlockLocal();
	//! Return Constraint Block defined by (constraints in this) X (variables in emcol)
	//! Creat one if this is not already created
	BlockCons* getConsBlockLocal(ExpandedModel* emcol);
	BlockHV* getHVBlockLocal(ExpandedModel* emcol);
	//! Return Obj Block (objective in this) X (variables in emcol)
	//! Creat one if this is not already created
	BlockObj* getObjBlockLocal(ExpandedModel* emcol);
	//
	//Function evaluation for constraints declared at this exapaned model
	void cons_feval_local(std::vector<double>& fvals);
	//! Return the nonzeros in the Jacobian of a section of the model
	uint nz_cons_jacobs_local(ExpandedModel* emcol);
	void cons_jacobs_local(ExpandedModel *emcol, boost::numeric::ublas::compressed_matrix<double>&);
	uint nz_cons_hess_local(ExpandedModel* emcol);
	void cons_hess_local(ExpandedModel* emcol, boost::numeric::ublas::compressed_matrix<double>&);
	uint nz_lag_hess_local(ExpandedModel* emcol);
	void lag_hess_local(ExpandedModel* emcol,boost::numeric::ublas::compressed_matrix<double>&);
	//
	double& obj_feval_local(double& oval);
	//! Returns the objective gradient for the local model w.r.t. local vars
	void obj_grad_local(ExpandedModel* emcol, std::vector<double>& ograd);
	uint nz_obj_hess_local(ExpandedModel* emcol);
	void obj_hess_local(ExpandedModel* emcol, boost::numeric::ublas::compressed_matrix<double>&);

	/*
	 * Distribute Interface Methods
	 */
	Block* getBlockDistributed(ExpandedModel* emcol);
	//! Creating BlockCons defined by (constraints in this) X (variables in emcol)
	BlockCons* createConsBlockDistributed(ExpandedModel * emcol);
	//! creating BlockObj defined by (objective in this) X (variables in emcol)
	BlockObj* constructObjBlockDistributed(ExpandedModel* emcol);
	//! Return the function values
	void cons_feval_distribute(ExpandedModel *emcol, std::vector<double>& fvals);
	//! Returns the number of blocks and nonzeros in the Jacobian of a section of the model
	int nz_jacobs_distribute(ExpandedModel* emcol, std::vector<unsigned int>& nzs);
	//! Returns the nonzeros in the Jacobian of a section of the model
	void cons_jacobs_distribute(ExpandedModel *emcol, std::vector<boost::numeric::ublas::compressed_matrix<double> >& blocks);
	//! Return the nonzeros in the Hessian of a section of the model
	int nz_hess_distribute(ExpandedModel* emcol, std::vector<unsigned int>& nzs);
	//! Returns the nonzeros in the Hessian of a section of the model
	void cons_hessian_distribute(ExpandedModel* emcol, std::vector<boost::numeric::ublas::compressed_matrix<double> >& blocks);


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
	void get_local_var_up_bounds(double *elts);
	//! Returns the vector of lower bounds for the local variables in this model
	void get_local_var_low_bounds(double *elts);
	//! Return the arrays of bounds for the constraints in this model
	void get_local_cons_bounds(double *lower, double *upper);
	//! Returns the names of local variables
	void get_local_vars_names(vector<string>&);
	//! Returns the names of local constraints
	void get_local_cons_names(vector<string>&);
	//! Returns the number of local variables
	uint getNLocalCons();
	//! Returns the names of local variables
	uint getNLocalVars();

	/*
	 * Helper methods
	 */
	void getAllEM(vector<ExpandedModel*>& ems);
	void getParentEM(vector<ExpandedModel*>& ems);
	void addModelDummy(string dummyVar, ModelComp* comp, string value);
	void addChildren(ExpandedModel* em2);
	void clearAllContextTreeKeepRoot();
	void levelTraversal(vector<ExpandedModel*>& em2List,int level);
	static void convertToColSparseMatrix(boost::numeric::ublas::compressed_matrix<double>& m,ColSparseMatrix* sm);

	/*
	 * Utility methods
	 */
	ModelContext* locateCtx(AmplModel* model, string& dummyval);
	void logEMRecursive(string& line,ostream&);
	void calculateMemoryUsage(unsigned long& size_str,unsigned long& size_data);
private:
	void getQaulifiedName(ostringstream& oss);
	ModelContext* recursiveInitContext();
	void copyVariables(boost::unordered_set<AutoDiff::Node*>&);
	void copyVariables(std::vector<AutoDiff::Node*>&);
};

#endif /* EXPANDEDMODEL_H_ */
