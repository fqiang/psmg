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

#include "../context/CompDescr.h"
#include "../context/ModelContext.h"
#include <list>
#include <string>
#include <vector>
#include <map>

class AmplModel;
class StochModel;
class SyntaxNode;
class SyntaxNodeIx;
class ModelContext;
class ExpandedModel2;

enum { NOARG = 0,
       WITHARG = 1,
       ONLYARG = 2};

using namespace std;
/** Possible types of a model component */
typedef enum {TVAR=0, TCON, TPARAM, TSET, TMIN, TMAX, TMODEL, TNOTYPE} compType;

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
  static const std::string nameTypes[];
  static const std::string compTypes[];

  /** Type of the component */
  compType type;

  /** Name of the component */
  std::string id;

  /** A tree of specifications (which includes :=, within, default, >=) */
  SyntaxNode *attributes;   
			 
  /**  Indexing expression */
  SyntaxNodeIx *indexing;

  /** The model this component belongs to */
  AmplModel *model; //parent

  /** A pointer to an AmplModel structure for components of type MODEL
   *  @attention Better implemented as a subclass of ModelComp. */
  AmplModel *other; //corresponding AmplModel object

  int moveUpLevel;

  string hashKey;

 protected:

  /** List of all entities that this model component depends on.
   *
   *  This lists all model components used in the definition of this component.
   */
  std::list<ModelComp*> dependencies;

  /** Components can be tagged by the tagDependencies method which sets
   *  this tag for this components and everything that it depends on
   *  (i.e. everything listed in the dependencies list).                 */
  //bool tag;            //!< true if part of the current 'needed' set

  /** For sets and parameters, this points to an object that gives the
   *  values and further specific information (Set for sets)
   */
  //CompDescr *compDescr;

 public:

  /** Constructor */
  ModelComp(const std::string& id_, compType type_,
            SyntaxNode *indexing_, SyntaxNode *attrib,int moveUpLevel=0);

  /** Default constructor */
  ModelComp(const std::string& id);

  /** Destructor */
  virtual ~ModelComp();

  /** Set up list of dependencies for this component */
  void setUpDependencies();

  /** Detailed debugging output */
  void dump(std::ostream& fout) const;

  /** Recalculate dependency list and re-resolve IDREF nodes */
  void reassignDependencies();

  /** Move this model component up in the model tree */
  void moveUp(int level);

  /** Duplicate the object: shallow copy */
  virtual ModelComp *clone() const;

  /** Duplicate the object: deep copy */
  ModelComp *deep_copy() const;

  // Virtual methods implemented only for stochastic models

  virtual void setStochModel(StochModel *stoch) { throw; }
  virtual void setStageSetNode(SyntaxNode *stageSet) { throw; }
  virtual void setDeterministic(bool det) { throw; }
  virtual SyntaxNode* getStageSetNode() const { throw; }
  virtual void addStageName(const std::string& name) { throw; }
  virtual const std::vector<std::string>& getStageNames() const { throw; }
  virtual ModelComp* transcribeToModelComp(AmplModel *current_model,
                                           const std::string& nodedummy,
                                           const std::string& stagedummy,
                                           const int level) { throw; }

  //Feng
  int setDim; //belong to set
  int setCard; //belong to set
  int varIndicies; //belong to TVar
  int varCard; //belong to TVar

  vector<string> paramIndiciesDummy; //belong to param  dummay->Set*
  vector<ModelComp*> paramIndiciesComp;
  hash_map<string,ModelComp*> paramIndiciesMap;

  void setSetDim();
  void calculateSetModelComp(ModelContext* context);

  void setParamIndicies();
  int getNumParamIndicies();
  void calculateParamModelComp(ModelContext* context);

  void fillLocalVar(ExpandedModel2* em2);
  void calculateLocalVar(ModelContext* context);
  void fillLocalVarRecurive(ModelContext* context,Var* aVar,vector<ModelComp*>::iterator it,ostringstream& oss);

  string& getHashKey();
  void calculateMemoryUsage(unsigned long& size);
  //feng end

 private:

  /** Find dependencies for this component */
  void findDependencies(const SyntaxNode *nd);

};

#endif /* MODELCOMP_H_ */
