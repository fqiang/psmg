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

	/** A tree of specifications (which includes :=, within, default, >=) */
	SyntaxNode *attributes;

	/**  Indexing expression */
	SyntaxNode *indexing;

	/** The model this component belongs to */
	AmplModel *model; //owner of this comp

//	string hashKey;

	/** Constructor */
	ModelComp(const string& _id, compType type, SyntaxNode *_indexing, SyntaxNode *_attribute);

	/** Destructor */
	virtual ~ModelComp();


	/** Detailed debugging output */
	virtual void dump(std::ostream& fout,int) = 0;

//	string& getHashKey();
	void calculateMemoryUsage(unsigned long& size);


//legacy!!


//  /** Default constructor */
//  ModelComp(string id);
//
//  /** Duplicate the object: shallow copy */
//   virtual ModelComp *clone() const;
//
//   /** Duplicate the object: deep copy */
//   ModelComp *deep_copy() const;
//   // Virtual methods implemented only for stochastic models
//
//     virtual void setStochModel(StochModel *stoch) { throw; }
     virtual void setStageSetNode(SyntaxNode *stageSet) { throw; }
     virtual void setDeterministic(bool det) { throw; }
//     virtual SyntaxNode* getStageSetNode() const { throw; }
//     virtual void addStageName(const std::string& name) { throw; }
//     virtual const std::vector<std::string>& getStageNames() const { throw; }
//     virtual ModelComp* transcribeToModelComp(AmplModel *current_model,
//                                              const std::string& nodedummy,
//                                              const std::string& stagedummy,
//                                              const int level) { throw; }


  //for TCON
  //  hash_map<int,set<int> > varDeps; //belong to TCON -- map separability of variable declared in level -> set of levels
  //  void analyseVarDepLevelsInCons();
  //  Node* constructAutoDiffCons(ModelContext* ctx, Block* emb,ExpandedModel* emcol);

  //  /** Set up list of dependencies for this component */
  //  void setUpDependencies();
   //end TCON


 protected:

//  /** List of all entities that this model component depends on.
//   *
//   *  This lists all model components used in the definition of this component.
//   */
//  std::list<ModelComp*> dependencies;

  /** Components can be tagged by the tagDependencies method which sets
   *  this tag for this components and everything that it depends on
   *  (i.e. everything listed in the dependencies list).                 */
  //bool tag;            //!< true if part of the current 'needed' set

  /** For sets and parameters, this points to an object that gives the
   *  values and further specific information (Set for sets)
   */
  //CompDescr *compDescr;

  //  /** Recalculate dependency list and re-resolve IDREF nodes */
  //  void reassignDependencies();

 private:

//  /** Find dependencies for this component */
//  void findDependencies(const SyntaxNode *nd);

};

#endif /* MODELCOMP_H_ */
