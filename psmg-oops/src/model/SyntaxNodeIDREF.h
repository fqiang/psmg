/*
 * SyntaxNodeIDREF.h
 *
 *  Created on: 28 Sep 2011
 *      Author: s0965328
 */

#ifndef SYNTAXNODEIDREF_H_
#define SYNTAXNODEIDREF_H_

#include "SyntaxNode.h"


/* ----------------------------------------------------------------------------
SyntaxNodeIDREF
---------------------------------------------------------------------------- */
/** @class SyntaxNodeIDREF
 *  A node on the tree representing a reference to a ModelComp.
 *
 *  IDREF is a SyntaxNode that represents a reference to a ModelComponent.
 */
class SyntaxNodeIDREF : public SyntaxNode {

 public:

  /** Pointer to the ModelComp referred to by this node */
  ModelComp *ref;

  /* stochrecourse was for the same purpose as stochparent, just that the
     recourse level was given as a SyntaxNode (i.e. expression to be
     eveluated by AMPL rather than an explicit  INT_VAL) */
  //?SyntaxNode *stochrecourse; //!< resourse level in stoch programming

   private:

  /** Levels above this one for which the reference is.
   *
   *  This field is only meaningful if the node represents a component
   *  in a stochastic program. In that case stochparent gives the recourse
   *  level of the component. This is the first argument in expressions
   *  such as xh(-1,i) which refers to xh[i] in the parent stage.
   */
  int stochparent;

 public:

  /** Default constructor */
  SyntaxNodeIDREF(int opCode,  SyntaxNode *val1=NULL, ModelComp *r=NULL);

  /** Constructor */
  SyntaxNodeIDREF(int opCode, ModelComp *r);

  virtual ~SyntaxNodeIDREF();


  std::ostream& put(std::ostream& s);

  /** Retrieve the level of the parent stage */
  int getStochParent() const { return stochparent; }

  /** Set the level of the parent stage */
  void setStochParent(int parent) { stochparent = parent; }

  virtual void calculateMemoryUsage(unsigned long& size);
};

#endif /* SYNTAXNODEIDREF_H_ */


///** Creates a shallow copy: points to the same components as the original */
//SyntaxNodeIDREF *clone();
//
///** Creates a copy using all new datastructures (does not duplicate ref) */
//SyntaxNodeIDREF *deep_copy();

