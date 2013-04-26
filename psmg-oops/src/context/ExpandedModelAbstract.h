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

#ifndef ExpandedModelAbstract_H
#define ExpandedModelAbstract_H

#include <list>
#include <string>
#include <vector>

/**
 *  The representation of a a submodel (block) in the expanded model
 *  which is passed to the solver to allow it to calculate values.
 *
 *  The class offers two iterators to allow access to all nodes we may
 *  interact with:
 *  -# child_iterator: iterates over descendants in depth-first order
 *  -# ancestor_iterator: iterates back to root
 *  Descendants are submodels contained within this one, and ancestors are
 *  models in which this model is contained.
 */
using namespace std;
class ExpandedModelAbstract {
 //friend class AmplModel;

	public:
		vector<ExpandedModelAbstract*> children;
		ExpandedModelAbstract *parent;

		//! Returns the number of local variables
		virtual int getNLocalVars() const = 0;

		//! Returns the names of local variables
		virtual const std::list<std::string>& getLocalVarNames() const = 0;

		//! Returns the number of local constraints.
		virtual int getNLocalCons() const = 0;

		//! Returns the names of local constraints
		virtual const std::list<std::string>& getLocalConNames() const = 0;

		//! Returns the nonzeros in the Jacobian of a section of the model
		virtual int getNzJacobianOfIntersection(ExpandedModelAbstract *emcol) = 0;

		//! Returns the nonzeros in the Jacobian of a section of the model
		virtual void getJacobianOfIntersection(ExpandedModelAbstract *emcol, int *colbeg,
					 int *collen, int *rownbs, double *el) = 0;

		//! Return the arrays of bounds for the constraints in this model
		virtual void getRowBounds(double *lower, double *upper) = 0;

		//! Returns the vector of lower bounds for the local variables in this model
		virtual void getColLowBounds(double *elts) = 0;

		//! Returns the vector of upper bounds for the local variables in this model
		virtual void getColUpBounds(double *elts) = 0;

		//! Returns the objective gradient for the local model w.r.t. local vars
		virtual void getObjGradient(double *elts) = 0;

		//! Upload the local variable solutions
		virtual void setPrimalSolColumns(const double *elts) = 0;

		//! Upload the local variable duals (multipliers on bounds)
		virtual void setDualSolColumns(const double *elts) = 0;

		//! Upload the local constraints slacks
		virtual void setPrimalSolRows(const double *elts) = 0;

		//! Upload the local constraints duals (multipliers on constraints)
		virtual void setDualSolRows(const double *elts) = 0;

		//! Returns the unique name of this block.
		virtual std::string getName() const = 0;

		//! Outputs the solution to the supplied stream at given indent
		virtual void outputSolution(std::ostream &out, int indent=0) = 0;

//		virtual void allocateExpandedModel(int myIndex) = 0;
//
//		virtual int getAtRank() = 0;

		virtual void calculateMemoryUsage(unsigned long& size_str,unsigned long& size_data) = 0;

	public:
		virtual ~ExpandedModelAbstract() {}
};

#endif
