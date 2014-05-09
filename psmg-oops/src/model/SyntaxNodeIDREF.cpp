/*
 * SyntaxNodeIDREF.cpp
 *
 *  Created on: 28 Sep 2011
 *      Author: s0965328
 */

#include "SyntaxNodeIDREF.h"
#include "SyntaxNodeIDREFM.h"
#include "SyntaxNodeID.h"

#include "ObjComp.h"
#include "VarComp.h"
#include "../st_model/StochModel.h"
#include "../st_model/StochCtx.h"
#include "../util/util.h"

#include "../parser/sml.tab.h"

/* --------------------------------------------------------------------------
 SyntaxNodeIDREF::SyntaxNodeIDREF(ModelComp *r)
 ---------------------------------------------------------------------------- */
SyntaxNodeIDREF::SyntaxNodeIDREF(SyntaxNode *val1, ModelComp *comp) :
		SyntaxNode(IDREF, val1), ref(comp)
{
	assert(comp!=NULL);
	assert(val1->opCode == ID);
}

SyntaxNodeIDREF::~SyntaxNodeIDREF() {
}

SyntaxNodeIDREF::SyntaxNodeIDREF(const SyntaxNodeIDREF& src):SyntaxNode(src)
{
	ref = src.ref;
}

SyntaxNodeIDREF* SyntaxNodeIDREF::clone()
{
	assert(nchild()<=2);
	//make a copy, then
	SyntaxNodeIDREF* refn = new SyntaxNodeIDREF(*this);
	//update reference refn->ref
	SyntaxNodeID* idn = static_cast<SyntaxNodeID*>(refn->values[0]);
	if(refn->ref->type == TVAR && static_cast<VarComp*>(refn->ref)->isDet == true)
	{
		//referencing to the only var for this stage
		string refname = GV(var_det_prefix)+"_"+SCTX::currCtx->stagename+"_"+refn->ref->name;
		assert(idn->id.compare(refn->ref->name)==0);
		refn->ref = SCTX::rootCtx->model->findModelComp(refname);
		//update the idname in idnode as well - for sack of consistance
		idn->id = refname;
	}
	else {
		string refname = idn->id;
		assert(refname.compare(refn->ref->name)==0);
		refn->ref = SCTX::currCtx->model->findModelComp(refname);
	}
	assert(refn->ref!=NULL);
	return refn;
}

SyntaxNode* SyntaxNodeIDREF::appendDOTNotation(StochCtx* sctx)
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

ostream& SyntaxNodeIDREF::put(ostream& s) {
	assert(this->opCode == IDREF);
	assert(nchild()==1 || nchild()==2);
	assert(this->values[0]->opCode == ID);
	SyntaxNodeID* idn = static_cast<SyntaxNodeID*>(this->values[0]);
	s<<"IDREF{"<<idn->id<<":"<<(void*)ref<<"}";
	if(nchild()==2){
		assert(values[1]->opCode == COMMA);
		s<<"["<<values[1]<<"]";
	}
	else if(nchild()>2)
	{
		assert(false); //should never research here.
	}
	return s;
}

void SyntaxNodeIDREF::calculateMemoryUsage(unsigned long& size) {
	size += sizeof(SyntaxNodeIDREF);
	//ref is counted in ampl model's component
	this->calculateBaseValueVector(size);
}
