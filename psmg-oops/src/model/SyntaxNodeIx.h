/*
 * SyntaxNodeIx.h
 *
 *  Created on: 28 Sep 2011
 *      Author: s0965328
 */

#ifndef SYNTAXNODEIX_H_
#define SYNTAXNODEIX_H_

#include "SyntaxNode.h"

using namespace std;

/** @class SyntaxNodeIx
 *  A node on the operator tree representing an indexing expression.
 *
 *  This is a node on the operator tree that represents an indexing expression.
 *  A general indexing expression can be of the form:
 *
 *  {(i,j) in ARCS, k in SET: i>k}
 *
 *  which will be broken down into a list of 'dummy IN set' expressions plus
 *  an optional qualifier (the condition to the right of COLON).
 */
class SyntaxNodeIx : public SyntaxNode {

 private:

  SyntaxNodeIx(const int opCode_);

  //! Number of 'dummy IN set'-type expressions
  int ncomp;

  //! List of the dummyVarExpressions
  vector<SyntaxNode*> dummyVarExpr;

  //! List of the set expressions
  vector<SyntaxNode*> sets;

  //! The expresson to the right of the ':' (if present)
  SyntaxNode *qualifier;

  //! List of ModelComp for the indexing sets

 public:
  vector<ModelComp*> sets_mc;
  // --------------------------- methods ----------------------------------

  //! Constructor
  SyntaxNodeIx(SyntaxNode *on);

  virtual ~SyntaxNodeIx();

  //! Find if the indexing expression defines the given dummy variable
  SyntaxNode *hasDummyVar(const std::string& name);

  //! Return the list of all dummy variables defined by this index's expression
  std::list<SyntaxNode *> getListDummyVars() const;

  //! Retrieve the number of indexing expressions
  int getNComp() const { return ncomp; }

  //! Retrieve the dummy variable for the specified indexing expression
  SyntaxNode* getDummyVarExpr(int i) const { return dummyVarExpr[i]; }

  //! Retrieve the set for the specified indexing expression
  SyntaxNode* getSet(int i) const { return sets[i]; }

  //! Retrieve the ModelComp for the specified indexing expression
  ModelComp* getModelComp(int i) const { return sets_mc[i]; }

  //! Set up the ->sets, ->dummyVarExpr, ->ncomp, ->qualifier components
  void splitExpression();
  void resetSplitExpression();
  //! Copy the node and all its subnodes into new data structures
  SyntaxNodeIx *deep_copy();

  //! Diagnostic printing of member variables
  void printDiagnostic(std::ostream& fout) const;

  //! for nodes that are indexing expressions, get the set that is indexed over
  const SyntaxNode* getIndexingSet() const;

  virtual void calculateMemoryUsage(unsigned long& size);

};

#endif /* SYNTAXNODEIX_H_ */
