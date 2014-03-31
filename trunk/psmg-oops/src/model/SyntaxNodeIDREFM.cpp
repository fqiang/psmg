/*
 * SyntaxNodeIDREFM.cpp
 *
 *  Created on: 6 Jan 2014
 *      Author: s0965328
 */

#include "SyntaxNodeIDREFM.h"
#include "SyntaxNodeID.h"

#include "ObjComp.h"
#include "../parser/sml.tab.h"

SyntaxNodeIDREFM::SyntaxNodeIDREFM(int opCode, SyntaxNode* id, AmplModel* model):
	SyntaxNode(IDREFM,id),ref(model)
{
	assert(model!=NULL);
	assert(opCode == IDREFM);
}

SyntaxNodeIDREFM::~SyntaxNodeIDREFM() {
	// TODO Auto-generated destructor stub
}

ostream& SyntaxNodeIDREFM::put(ostream& s)
{
	assert(this->opCode == IDREFM);
	assert(nchild()==1 || nchild() == 2);
	assert(this->values[0]->opCode == ID);
	SyntaxNodeID* idn = static_cast<SyntaxNodeID*>(this->values[0]);
	s<<"IDREFM{"<<idn->id<<"}";
	if(nchild()==2){
		assert(values[1]->opCode == COMMA);
		s<<"["<<values[1]<<"]";
	}
}

void SyntaxNodeIDREFM::calculateMemoryUsage(unsigned long& size) {
	size += sizeof(SyntaxNodeIDREFM);
	//ref is counted in ampl model's component
	this->calculateBaseValueVector(size);
}
