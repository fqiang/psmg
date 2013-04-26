/*
 * SyntaxNodeIDREF.cpp
 *
 *  Created on: 28 Sep 2011
 *      Author: s0965328
 */

#include "SyntaxNodeIDREF.h"
#include "../parser/sml.tab.h"
#include "../util/global_util_functions.h"

/* --------------------------------------------------------------------------
 SyntaxNodeIDREF::SyntaxNodeIDREF(ModelComp *r)
 ---------------------------------------------------------------------------- */
SyntaxNodeIDREF::SyntaxNodeIDREF(ModelComp *r, SyntaxNode *val1) :
		SyntaxNode(IDREF, val1), ref(r), stochparent(0) {

}

SyntaxNodeIDREF::SyntaxNodeIDREF(int opCode_, ModelComp *r) :
		SyntaxNode(opCode_), ref(r), stochparent(0) {
	assert(opCode==IDREF||opCode==IDREFM);
	LOG(
			"SyntaxNodeIDREF constructor called -- opCode["<<opCode_<<"] ModelComp["<<r<<"]");
}

SyntaxNodeIDREF::~SyntaxNodeIDREF() {
	LOG("SyntaxNodeIDREF destructor -- ");
}

ostream& SyntaxNodeIDREF::put(ostream& s) const {

	switch (opCode) {
	case IDREF:
		if (nchild() < 0)
			// as yet unset IDREF
			s << "IDREF";
		else {
			/* this is the new ID processor */
			if (nchild() == 0)
				s << ref->id;
			else {
				SyntaxNode::iterator i = begin();
				s << ref->id << "[" << **i;
				while (++i != end())
					s << "," << **i;
				s << "]";
			}
		}
		break;
	case IDREFM:
		/* this is the new ID processor (for submodels) */
		// ??? is this correct
		s << ref->id;
		break;
	default:
		cerr << "In SyntaxNodeIDREF::put but not an IDREF or IDREFM\n";
		exit(1);
	}

	return s;
}

/* --------------------------------------------------------------------------
 SyntaxNodeIDREF *SyntaxNodeIDREF::deep_copy()
 ---------------------------------------------------------------------------- */
SyntaxNodeIDREF* SyntaxNodeIDREF::deep_copy() {
	SyntaxNodeIDREF *newn = new SyntaxNodeIDREF(opCode, ref);

	newn->values.resize(nchild());
	for (int i = 0; i < nchild(); ++i)
		newn->values[i] = values[i]->deep_copy();
	newn->stochparent = stochparent;

	return newn;
}

/* --------------------------------------------------------------------------
 SyntaxNodeIDREF *SyntaxNodeIDREF::clone()
 ---------------------------------------------------------------------------- */
SyntaxNodeIDREF * SyntaxNodeIDREF::clone() {
	LOG("enter SyntaxNodeIDREF::clone -- ");
	SyntaxNodeIDREF *newn = new SyntaxNodeIDREF(opCode, ref);

	newn->values = values;
	newn->stochparent = stochparent;
	LOG("exit SyntaxNodeIDREF::clone -- ");
	return newn;
}

void SyntaxNodeIDREF::calculateMemoryUsage(unsigned long& size) {
	size += sizeof(SyntaxNodeIDREF);
	//ref is counted in ampl model's component
	this->calculateBaseValueVector(size);
}
