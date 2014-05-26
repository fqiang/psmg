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

#ifndef AMPLMODEL_H
#define AMPLMODEL_H

#include <vector>
#include <string>

#include "ModelComp.h"

class SetComp;
class ObjComp;
class ParamComp;
class VarComp;
class ConsComp;
class ModelContext;
class ExpandedModel;

using namespace std;


extern void parse_data(ModelContext* rootContext);

/** @class AmplModel
 *  This class describes a model (block) in the flat model tree.
 *
 *  It should really be called FlatModelNode (or something like that).
 *  It keeps track of the components (vars/cons/sets/params/submodels) 
 *  associated with this model. 
 *  Each component is stored in *symbolic* form: i.e. a tree of AMPL
 *  expressions for the body of the component definition and a tree of AMPL
 *  expressions for the indexing expression. It does not know about the
 *  cardinality of each component (it does not expand indexing expressions).
 *  It keeps track of both the number of every type registered and a linked
 *  list of entries describing each of the entities in more detail.
 */
class AmplModel : public ModelComp{

public:
	/** The root model of the AmplModel tree */
	static AmplModel *root;
	static int MAX_LEVEL;
	/** The parent if this is a submodel of another model */
	AmplModel *parent;

	int n_vars;      //!< number of variable declarations
	int n_cons;      //!< number of constraint declarations
	int n_params;    //!< number of parameter declarations
	int n_sets;      //!< number of set declarations
	int n_objs;      //!< number of objective declarations
	int n_submodels; //!< number of submodel/block declarations
	int n_total;     //!< total number of declarations
	int level;       //!< level of this model on the flat model tree (root=0)

	/** The list of components of this model */
	vector<SetComp*> set_comps;
	vector<ParamComp*> param_comps;
	vector<VarComp*> var_comps;
	vector<ConsComp*> con_comps;
	vector<AmplModel*> subm_comps;
	ObjComp* obj_comp;

	bool split; //true if constraint and objective already splitted into partials.

	// -------------------------- methods ----------------------------------
	/** Constructor */
	AmplModel(const string& name, SyntaxNode* index, AmplModel *par);

	/** Destructor */
	virtual ~AmplModel();

	/** Add a model component to the model */
	void addComp(ModelComp *comp);

	/** Recursive detailed debugging output */
	void logModel(const char *filename);

	//find the ModelComp to fill the CompDescr.
	ModelComp* findModelComp(string& id);
	ParamComp* findParamComp(string& id);
	SetComp* findSetComp(string& id);
	void calculateModelComp(ModelContext& context);
	void calculateModelCompRecursive(ModelContext& context);

	void splitConstraints();
	virtual ExpandedModel* createExpandedModel(string dummyVar,SetComp* comp,string value,ExpandedModel* parent);

	void calculateLocalVar(ModelContext& context);
	void calculateMemoryUsage(unsigned long& size);


	void dump(std::ostream& fout, int);
};

#endif
