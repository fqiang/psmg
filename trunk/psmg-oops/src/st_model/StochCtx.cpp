/*
 * StageCtx.cpp
 *
 *  Created on: 22 Apr 2014
 *      Author: s0965328
 */

#include "StochCtx.h"

string StochCtx::nd = "";
string StochCtx::st = "";
ModelComp* StochCtx::stSetComp = NULL;
ModelComp* StochCtx::ndSetComp = NULL;
ModelComp* StochCtx::pbSetComp = NULL;
ModelComp* StochCtx::paSetComp = NULL;

StochCtx* StochCtx::currCtx = NULL;
StochCtx* StochCtx::rootCtx = NULL;

StochCtx::StochCtx(StochCtx* par):parent(par),child(NULL),model(NULL),stagename(""),stage_level(-1),model_dummy("")
{
	if(parent!=NULL) parent->child = this;
	currCtx = this;
}

StochCtx::~StochCtx() {
	// TODO Auto-generated destructor stub
}

void StochCtx::reset()
{
	nd = "";
	st = "";
	stSetComp = NULL;
	ndSetComp = NULL;
	pbSetComp = NULL;
	paSetComp = NULL;
	currCtx = NULL;
	rootCtx = NULL;
}
