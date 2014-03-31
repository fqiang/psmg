/*
 * SyntaxNodeIDREF.cpp
 *
 *  Created on: 28 Sep 2011
 *      Author: s0965328
 */

#include "SyntaxNodeIDREF.h"
#include "SyntaxNodeID.h"

#include "ObjComp.h"
#include "../parser/sml.tab.h"

/* --------------------------------------------------------------------------
 SyntaxNodeIDREF::SyntaxNodeIDREF(ModelComp *r)
 ---------------------------------------------------------------------------- */
SyntaxNodeIDREF::SyntaxNodeIDREF(int opCode, SyntaxNode *val1, ModelComp *comp) :
		SyntaxNode(IDREF, val1), ref(comp), stochparent(0)
{
	assert(comp!=NULL);
	assert(opCode == IDREF);
}

SyntaxNodeIDREF::SyntaxNodeIDREF(int opCode_, ModelComp *comp) :
		SyntaxNode(opCode_), ref(comp), stochparent(0) {
	assert(opCode==IDREF);
	assert(comp!=NULL);
	LOG("SyntaxNodeIDREF constructor called -- opCode["<<opCode_<<"] ModelComp["<<comp->name<<"]");
}

SyntaxNodeIDREF::~SyntaxNodeIDREF() {
	LOG("SyntaxNodeIDREF destructor -- ");
}

ostream& SyntaxNodeIDREF::put(ostream& s) {
	assert(this->opCode == IDREF);
	assert(nchild()==1 || nchild()==2);
	assert(this->values[0]->opCode == ID);
	SyntaxNodeID* idn = static_cast<SyntaxNodeID*>(this->values[0]);
	s<<"IDREF{"<<idn->id<<"}";
	if(nchild()==2){
		assert(values[1]->opCode == COMMA);
		s<<"["<<values[1]<<"]";
	}
	return s;
}

void SyntaxNodeIDREF::calculateMemoryUsage(unsigned long& size) {
	size += sizeof(SyntaxNodeIDREF);
	//ref is counted in ampl model's component
	this->calculateBaseValueVector(size);
}



//legacy!!

////--------------------------------------------------------------
//
///* --------------------------------------------------------------------------
// SyntaxNodeIDREF *SyntaxNodeIDREF::deep_copy()
// ---------------------------------------------------------------------------- */
//SyntaxNodeIDREF* SyntaxNodeIDREF::deep_copy() {
//	SyntaxNodeIDREF *newn = new SyntaxNodeIDREF(opCode, ref);
//
//	newn->values.resize(nchild());
//	for (int i = 0; i < nchild(); ++i)
//		newn->values[i] = values[i]->deep_copy();
//	newn->stochparent = stochparent;
//
//	return newn;
//}
//
///* --------------------------------------------------------------------------
// SyntaxNodeIDREF *SyntaxNodeIDREF::clone()
// ---------------------------------------------------------------------------- */
//SyntaxNodeIDREF * SyntaxNodeIDREF::clone() {
//	LOG("enter SyntaxNodeIDREF::clone -- ");
//	SyntaxNodeIDREF *newn = new SyntaxNodeIDREF(opCode, ref);
//
//	newn->values = values;
//	newn->stochparent = stochparent;
//	LOG("exit SyntaxNodeIDREF::clone -- ");
//	return newn;
//}


