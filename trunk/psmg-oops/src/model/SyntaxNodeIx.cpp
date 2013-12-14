/*
 * SyntaxNodeIx.cpp
 *
 *  Created on: 28 Sep 2011
 *      Author: s0965328
 */

#include "SyntaxNodeIx.h"
#include "IDNode.h"
#include "../parser/sml.tab.h"
#include "../util/global_util_functions.h"

SyntaxNodeIx::SyntaxNodeIx(const int opCode_):
	SyntaxNode(opCode_), qualifier(NULL), ncomp(0)
{
	LOG("Created SyntaxNodeIx -- opCode["<<opCode_<<"]");
}


/* ==========================================================================
SyntaxNodeix Methods to follow
============================================================================*/
SyntaxNodeIx::SyntaxNodeIx(SyntaxNode *on) :
   SyntaxNode(*on)
{
  qualifier = NULL;
  ncomp = 0;
  splitExpression();
  LOG("Created SyntaxNodeIx .... ");
}

SyntaxNodeIx::~SyntaxNodeIx()
{
	sets.clear();
	sets_mc.clear();
	for(vector<ModelComp*>::iterator it=sets_mc.begin();it!=sets_mc.end();it++)
	{
		ModelComp* mc = *it;
		if((mc->id).compare("dummy")==0)
		{
			delete mc;
		}
	}
}

/* --------------------------------------------------------------------------
SyntaxNodeIx::getIndexingSet()
---------------------------------------------------------------------------- */
const SyntaxNode* SyntaxNodeIx::getIndexingSet() const {
  const SyntaxNode *ix = this;
  const SyntaxNode *set;
  SyntaxNode *dummyVar;

  if (ix==NULL) return NULL;
  /* remove outside braces from indexing expression */
  if (ix->opCode == LBRACE)
    ix = ix->front();
  /* assumes that the next level is the 'IN' keyword (if present) */
  if (ix->opCode == IN) {
    SyntaxNode::iterator i = ix->begin();
    dummyVar = *i;
    set = *(++i);
  }else{
    dummyVar = NULL;
    set = ix;
  }
  return set;
}




/* ---------------------------------------------------------------------------
SyntaxNodeIx::printDiagnostic
-----------------------------------------------------------------------------*/
void
SyntaxNodeIx::printDiagnostic(ostream &fout) const {
  assert(ncomp > 0);
  fout << "qualifier: " << qualifier << "\n";
  fout << "number of indexing expressions: " << ncomp << "\n";
  for(int i=0;i<ncomp;i++){
    fout << "  " << i << ": dummyVar: " << dummyVarExpr[i] << "\n";
    fout << "     set     : " << sets[i] << "\n";
  }
}

/* ---------------------------------------------------------------------------
SyntaxNodeIx::getListDummyVars
-----------------------------------------------------------------------------*/
list<SyntaxNode *>
SyntaxNodeIx::getListDummyVars() const {
  list<SyntaxNode *> l;

  for(int i=0;i<ncomp;i++){
    SyntaxNode *dv = dummyVarExpr[i];
    // a dummy var expression is either ID/IDREF or (ID1,...IDn)
    if (dv->opCode == ID || dv->opCode ==IDREF ) {
      l.push_back(dv);
    }else if(dv->opCode == LBRACKET) {
      dv = dv->front();
      if (dv->opCode != COMMA) {
	     cerr << "A dummy variable expression is either ID or (ID1,...ID2)\n";
	     cerr << "Given expression: " << dv << "\n";
	     exit(1);
      }
//      ListNode *dvl = static_cast<ListNode *>(dv);
      for (SyntaxNode::iterator j = dv->begin(); j != dv->end(); ++j)
	     l.push_back(*j);
    }else{
      cerr << "A dummy variable expression is either ID or (ID1,...ID2)\n";
      cerr << "Given expression: " << dv << "\n";
      exit(1);
    }
  }
  return l;
}

/* ---------------------------------------------------------------------------
SyntaxNodeIx::splitExpression
-----------------------------------------------------------------------------*/
/* sets up the ->set, ->dummyVar components of the class

 A general indexing expression can be of the form

    {(i,j) in ARCS, k in SET: i>k}
*/
void SyntaxNodeIx::splitExpression()
{
	SyntaxNode *tmp, *tmp2;

	if (ncomp > 0)
		return;

	if (opCode != LBRACE)
	{
		cerr<< "Error in splitExpression: Indexing Expression must start with {\n";
		cerr<< "     " << this;
		exit(1);
	}

	tmp = this->front();
	// discard the colon (if there is one present: only interested in lhs)
	if (tmp->opCode == COLON)
	{
		SyntaxNode::iterator tj = tmp->begin();
		tmp = *tj; // step down tree
		qualifier = *(++tj); // qualifier from previous node
	}
	else
	{
		qualifier = NULL;
	}
	/* this should now be a comma separated list */
	if (tmp->opCode == COMMA)
	{
//		ListNode *tmpl = static_cast<ListNode*>(tmp);
		ncomp = tmp->nchild();
//		this->sets.resize(ncomp);
//		this->sets_mc.resize(ncomp);
//		this->dummyVarExpr.resize(ncomp);
		int i = 0;
		for (SyntaxNode::iterator ti = tmp->begin(); ti != tmp->end();++ti, ++i)
		{
			tmp2 = findKeywordinTree(*ti, IN);
			/* everything to the left of IN is a dummy variables */
			if (tmp2)
			{
				SyntaxNode::iterator j = tmp2->begin();
				dummyVarExpr.push_back(*j);
				sets.push_back(*(++j));
			}
			else
			{
				/* just set, but no dummyVar given */
				dummyVarExpr.push_back(NULL);
				sets.push_back(*ti);
			}
			/* try to find ModelComp of the set expression,
			 If it doesn't exist create */
			if (ModelComp *mc = sets.at(i)->findModelComp())
			{
				sets_mc.push_back(mc);
			}
			else
			{
				sets_mc.push_back(new ModelComp("dummy", TSET, NULL, sets[i]));
			}
		}
	}
	else
	{
		ncomp = 1;
//		this->sets.resize(1);
//		this->sets_mc.resize(1);
//		this->dummyVarExpr.resize(1);
		tmp2 = findKeywordinTree(tmp, IN);
		if (tmp2)
		{
			SyntaxNode::iterator tj = tmp2->begin();
			dummyVarExpr.push_back(*tj);
			sets.push_back(*(++tj));
		}
		else
		{
			/* just set, but no dummyVar given */
			dummyVarExpr.push_back(NULL);
			sets.push_back(tmp);
		}
		/* try to find ModelComp of the set expression,
		 If it doesn't exist create */
		if (ModelComp *mc = sets.at(0)->findModelComp())
		{
			sets_mc.push_back(mc);
		}
		else
		{
			sets_mc.push_back(new ModelComp("dummy", TSET, NULL, sets[0]));
		}
	}
	assert(sets.size()==ncomp);
	assert(sets_mc.size()==ncomp);
	assert(dummyVarExpr.size()==ncomp);
	assert(ncomp > 0);
}

void SyntaxNodeIx::resetSplitExpression()
{
	ncomp = 0;
	sets.clear();
	sets_mc.clear();
	for(vector<ModelComp*>::iterator it=sets_mc.begin();it!=sets_mc.end();it++)
	{
		ModelComp* mc = *it;
		if((mc->id).compare("dummy")==0)
		{
			delete mc;
		}
	}
	dummyVarExpr.clear();
}

/*----------------------------------------------------------------------------
SyntaxNodeIx::hasDummyVar
---------------------------------------------------------------------------- */
/** Find if the indexing expression given defines the given dummy variable.
 *
 *  @param name
 *         The name of the dummy variable to look for.
 *  @return The ("ID") SyntaxNode representing the dummy Variable (if found) or
 *          NULL (if not found)
 */
SyntaxNode *SyntaxNodeIx::hasDummyVar(const string& name)
{
	SyntaxNode *ret = NULL;

	for (int i = 0; i < ncomp; i++) {
		SyntaxNode *tmp = dummyVarExpr[i];

		if (!tmp) continue; // no dummy var, just a set.

		// this is either ID or (ID,   ,ID)
		if (tmp->opCode == ID) {
			IDNode *tmpid = (IDNode *) tmp;
			LOG("Found dummy variable: " << tmpid->id());
			if (name == tmpid->id())
				ret = tmp;
		}
		else{
			/* This is a multidimensional dummy variable: */
			assert(tmp->opCode == LBRACKET);
			tmp = tmp->front();
//			ListNode *tmpl = static_cast<ListNode*>(tmp);
			// and this should be a comma separated list
//			assert(tmpl);
			assert(tmp->opCode == COMMA);
			for (SyntaxNode::iterator j = tmp->begin(); j != tmp->end(); ++j) {
				// items on the list should be ID
				assert((*j)->opCode == ID);
				IDNode *tmp2 = (IDNode *) *j;
				LOG("Found dummy variable: " << tmp2->id());

				if (name == tmp2->id())
					ret = tmp2;
			}
		}
	}
	return ret;
}

/*----------------------------------------------------------------------------
SyntaxNodeIx::deep_copy
---------------------------------------------------------------------------- */
/** Copy the node and all its subnodes into new data structures.
 *
 *  SyntaxNodeIDREF nodes will also be duplicated, however they will point
 *  to the original ModelComp's (rather than duplicates of them).
 */
SyntaxNodeIx *
SyntaxNodeIx::deep_copy()
{
  SyntaxNodeIx *onix = new SyntaxNodeIx(opCode);

  onix->values.resize(nchild());
  for (int i = 0; i < nchild(); ++i)
    onix->values[i] = values[i]->deep_copy();

  // deep_copy is a virtual function, so qualifier->deep_copy is not defined
  // when qualifier==NULL
  if (qualifier) onix->qualifier = qualifier->deep_copy();

  onix->ncomp = ncomp;
  onix->sets.resize(ncomp);
  onix->dummyVarExpr.resize(ncomp);

  for(int i=0;i<ncomp;i++){
    onix->sets[i] = sets[i]->deep_copy();
    onix->dummyVarExpr[i] = dummyVarExpr[i] ? dummyVarExpr[i]->deep_copy()
                                            : NULL;
  }
  return onix;
}

void SyntaxNodeIx::calculateMemoryUsage(unsigned long& size)
{
	size += sizeof(SyntaxNodeIx);
	size += sizeof(SyntaxNode*)*this->dummyVarExpr.size();
	size += sizeof(SyntaxNode*)*this->sets.size();
	size += sizeof(ModelComp*)*this->sets_mc.size();
	this->calculateBaseValueVector(size);
}
