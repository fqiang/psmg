/*
 * SyntaxNodeIDREFM.cpp
 *
 *  Created on: 6 Jan 2014
 *      Author: s0965328
 */

#include "SyntaxNodeIDREFM.h"
#include "SyntaxNodeID.h"
#include "AmplModel.h"
#include "../st_model/StochCtx.h"

#include "ObjComp.h"
#include "../parser/sml.tab.h"

SyntaxNodeIDREFM::SyntaxNodeIDREFM(SyntaxNode* id, AmplModel* model):
	SyntaxNode(IDREFM,id),ref(model)
{
	assert(model!=NULL);
	assert(id->opCode == ID);
	assert(opCode == IDREFM);
}

SyntaxNodeIDREFM::~SyntaxNodeIDREFM() {
	// TODO Auto-generated destructor stub
}

SyntaxNodeIDREFM::SyntaxNodeIDREFM(const SyntaxNodeIDREFM& src):SyntaxNode(src)
{
	ref = src.ref;
}

SyntaxNodeIDREFM* SyntaxNodeIDREFM::clone(){
	assert(false); //will not be called, because stoch model don't use REFM notation in model file.
	return new SyntaxNodeIDREFM(*this);
}

SyntaxNode* SyntaxNodeIDREFM::appendDOTNotation(StochCtx* sctx)
{
	SyntaxNode* rval = this;
	if(sctx->model == ref->model)
	{
		SyntaxNodeIDREFM* refm = new SyntaxNodeIDREFM(new SyntaxNodeID(sctx->model->name),sctx->model);
		SyntaxNode* expr_list = new SyntaxNode(COMMA,new SyntaxNodeID(sctx->model_dummy));
		refm->push_back(expr_list);
		SyntaxNode* dotn = new SyntaxNode(DOT,refm,this);
		rval = dotn;
	}
	return rval;
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
	return s;
}

void SyntaxNodeIDREFM::calculateMemoryUsage(unsigned long& size) {
	size += sizeof(SyntaxNodeIDREFM);
	//ref is counted in ampl model's component
	this->calculateBaseValueVector(size);
}
