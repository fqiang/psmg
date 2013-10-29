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

#include "ModelComp.h"
#include "changeitem.h"
#include <vector>
#include <string>

class ExpandedModel;
class IDNode;
class SyntaxNode;
class SyntaxNodeIx;
class SyntaxNodeIDREF;
struct changeitem;

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
class AmplModel{
 public:


  /** The root model of the AmplModel tree */
  static AmplModel *root;
  /** The parent if this is a submodel of another model */
  AmplModel *parent;


  /** The ModelComp node corresponding to this model (defined if this is not
   *  root) */
  ModelComp *node;

  /** Name of the block defining this (sub)model */
  string name;

  int n_vars;      //!< number of variable declarations 
  int n_cons;      //!< number of constraint declarations 
  int n_params;    //!< number of parameter declarations 
  int n_sets;      //!< number of set declarations 
  int n_objs;      //!< number of objective declarations 
  int n_submodels; //!< number of submodel/block declarations
  int n_total;     //!< total number of declarations
  int level;       //!< level of this model on the flat model tree (root=0)



  /** The list of components of this model */
  vector<ModelComp*> all_comps;
  vector<ModelComp*> set_comps;
  vector<ModelComp*> param_comps;
  vector<ModelComp*> var_comps;
  vector<ModelComp*> con_comps;
  vector<ModelComp*> subm_comps;
  ModelComp* obj_comp;



  /** List of changes that should be applied to the models */
  static std::list<changeitem> changes;


  // -------------------------- methods ----------------------------------
  void deleteModelCompVector(vector<ModelComp*> comps);
  /** Constructor */
  AmplModel(const std::string& orig_name, AmplModel *par);
  
  /** Destructor */
  virtual ~AmplModel();

  /** Set the global name by concatenating ancestor names */
  void setGlobalName();      

  /** Set the global name recursively for this and all submodels */
  void setGlobalNameRecursive();      

  /** Recursively write out all tagged model components in this model and 
      submodels to file */
  void writeTaggedComponents(std::ostream& fout);
                                
  /** Recursively create an ExpandedModel tree from the flat AmplModel */
  ExpandedModel* createExpandedModel(const std::string& smodelname,
                                     const std::string& sinstanceStub);

  /** Add a model component to the model */
  virtual void addComp(ModelComp *comp);

  /** Remove a model component from the model */
  void removeComp(ModelComp *comp);
  static void removeComp(vector<ModelComp*> comps, ModelComp* comp);
  void removeAllComps();

  /** Recursively recalculate dependency list and re-resolve IDREF nodes */
  void reassignDependencies();

  /** Print debugging output recursively */
  void print() const;

  /** Recursive detailed debugging output */
  void dump(const char *filename) const;

  /** Recursive detailed debugging output */
  void dump(std::ostream& fout) const;

  static void applyChanges(); //< apply the model changes stored in Q

  virtual SyntaxNodeIDREF* createIdrefNode(IDNode *ref);

  bool isCompInThisModel(ModelComp* comp);

  // Virtual methods implemented only for stochastic models
  virtual AmplModel* expandToFlatModel() { throw; }

  //Feng start

  //find the ModelComp to fill the CompDescr.
  ModelComp* findModelComp(string id,compType type);
  void calculateCurrLevelModelComp(ModelContext* context);
  void calculateModelCompRecursive(ModelContext* context);
  ExpandedModel* createExpandedModel(string dummyVar,ModelComp* comp,string value,ModelContext* parent);
  void reassignModelIndexDependencies();
  void settingUpLevels(int);

  void analyseConstraints();
  void calculateMemoryUsage(unsigned long& size);
  //Feng end

};

#endif