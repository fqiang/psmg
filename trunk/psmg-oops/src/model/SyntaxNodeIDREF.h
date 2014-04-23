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

 public:

  SyntaxNodeIDREF(SyntaxNode *val1, ModelComp *r);
  SyntaxNodeIDREF(const SyntaxNodeIDREF& src);
  virtual SyntaxNodeIDREF* clone();
  virtual SyntaxNode* appendDOTNotation(StochCtx*);
  virtual ~SyntaxNodeIDREF();

  virtual std::ostream& put(std::ostream& s);
  virtual void calculateMemoryUsage(unsigned long& size);
};

#endif /* SYNTAXNODEIDREF_H_ */
