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

#ifndef MODELCOMP_H_
#define MODELCOMP_H_

#include <list>
#include <string>
#include <vector>

class SyntaxNode;
class AmplModel;

using namespace std;

/** Possible types of a model component */
typedef enum {TVAR=0, TCON, TPARAM, TSET, TOBJ, TMODEL, TNOTYPE} compType;
typedef enum {TMAX=0,TMIN} objType;

/** @class ModelComp
 *  Object to represent a component of an AMPL/SML model/block.
 *
 *  It usually represents one line of AMPL/SML which is a definition of a
 *  variable/parameter/set/constraint/objective/block.
 *
 *  A model component is broken down into the following parts:
 *  - type: the type of the component
 *  - id: the name of the component
 *  - indexing: the indexing expression stored as a tree
 *  - attributes: a list of attributes (this includes the actual constraint
 *                definition for "subject to" components)
 */
class ModelComp{
 public:

	const static string nameTypes[6];

	/** Name of the component */
	std::string name;

	/** Type of the component */
	compType type;

	/**  Indexing expression */
	SyntaxNode *indexing;

	/** A tree of specifications (which includes :=, within, default, >=) */
	SyntaxNode *attributes;

	/** The model this component belongs to */
	AmplModel *model; //owner of this comp

	/** The stage information for Stochastic Model
	 *  Note: this break the OO-Design a better implementation
	 *  will be inheritance for each model comp type
	 *  for stage!=NULL the model has to be a Stochastic Model
	 *  */
	SyntaxNode* stage;

	/** Constructor */
	ModelComp(const string& id, compType type, SyntaxNode *indexing, SyntaxNode *attribute, AmplModel* owner);

	/** Destructor */
	virtual ~ModelComp();


	/** Detailed debugging output */
	virtual void dump(std::ostream& fout,int) = 0;

//	string& getHashKey();
	void calculateMemoryUsage(unsigned long& size);

};

#endif /* MODELCOMP_H_ */
