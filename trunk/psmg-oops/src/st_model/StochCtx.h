/*
 * StageCtx.h
 *
 *  Created on: 22 Apr 2014
 *      Author: s0965328
 */
#include "../util/global_util_functions.h"
#include "../model/ModelComp.h"

#define SCTX StochCtx

#ifndef STOCHCTX_H_
#define STOCHCTX_H_

using namespace std;
class StochCtx {

private:

public:
	StochCtx(StochCtx* par);
	virtual ~StochCtx();

	static string nd;
	static string st;
	static ModelComp* stSetComp;
	static ModelComp* ndSetComp;
	static ModelComp* pbSetComp;
	static ModelComp* paSetComp;

	//! the current stage context the convertor is working on
	//! This is not thread safe, however providing a quick way to updating the reference in SyntaxNode clone method
	static StochCtx* currCtx;
	static StochCtx* rootCtx;
	static void reset();

	StochCtx* parent;
	StochCtx* child;
	//! the model corresponding to this stage
	AmplModel* model;
	//! the stage name for this stage
	string stagename;
	//! the stage level for this stage
	int stage_level;
	//! the model dummy for this stage
	string model_dummy;

};

#endif /* STAGECTX_H_ */
