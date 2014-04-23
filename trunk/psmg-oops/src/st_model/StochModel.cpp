/* (c) 2008,2009 Jonathan Hogg and Andreas Grothey, University of Edinburgh
 *
 * This file is part of SML.
 *
 * SML is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, using version 3 of the License.
 *
 * SML is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see http://www.gnu.org/licenses/.
 */

#include "../util/global_util_functions.h"
#include "../sml/Config.h"
#include "StochCtx.h"
#include "StochModel.h"
#include "../model/SyntaxNode.h"
#include "../model/SyntaxNodeOP.h"
#include "../model/SyntaxNodeID.h"
#include "../model/SyntaxNodeIDREF.h"
#include "../model/SyntaxNodeString.h"
#include "../parser/sml.tab.h"
#include <boost/foreach.hpp>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <fstream>
#include <iostream>

using namespace std;


/* ---------------------------------------------------------------------------
 StochModel::StochModel()
 ---------------------------------------------------------------------------- */
/** Constructor */
StochModel::StochModel(const string& name, SyntaxNode *stochsets, AmplModel *par) : AmplModel(name,NULL,par)
{
	LOG("StochModel -- name["<<name<<"] parent["<<par->name<<"]");
	assert(stochsets!=NULL && stochsets->nchild()==4);
	SyntaxNode::iterator i = stochsets->begin();
	this->nodeset = *i; i++;
	this->ancestor = *i; i++;
	this->probs = *i; i++;
	this->stageset = *i;

	LOG("end StochModel constructor-- ");
}

StochModel::~StochModel()
{
	LOG("StochModel ~ Desctructor ");
	delete nodeset;
	delete ancestor;
	delete probs;
	delete stageset;
	for(boost::unordered_map<SyntaxNode*,boost::unordered_set<string>* >::iterator i=stageSetMap.begin();
			i!=stageSetMap.end();i++)
	{
		delete i->second;
	}
	stageSetMap.clear();
}

ExpandedModel* StochModel::createExpandedModel(string dummyVar,SetComp* comp,string value,ModelContext* parent)
{
	//this is break the OO-Design, a better implementation is to considering the ModelComp and StochModel
	//a StochModel will first need to conver to amplmodel then, create the expanded model tree using
	//method in AmplModel class
	assert(false); //this should never called
	return NULL;
}

AmplModel* StochModel::convertToAmplModel(ModelContext* parCtx)
{
	SCTX::reset(); //reseting static values;
	StochCtx* curr_sctx = new StochCtx(NULL);

	if(this->nodeset->opCode == IDREF)
	{
		SCTX::ndSetComp = static_cast<SyntaxNodeIDREF*>(this->nodeset)->ref;
	}
	else
	{
		SCTX::ndSetComp = static_cast<SyntaxNodeIDREF*>(this->nodeset->values[1])->ref;
		SCTX::nd = static_cast<SyntaxNodeID*>(this->nodeset->values[0]->values[0])->id;
	}
	assert(this->ancestor->opCode == IDREF);
	SCTX::paSetComp = static_cast<SyntaxNodeIDREF*>(this->ancestor)->ref;
	assert(this->probs->opCode == IDREF);
	SCTX::pbSetComp = static_cast<SyntaxNodeIDREF*>(this->probs)->ref;
	if(this->stageset->opCode == IDREF)
	{
		SCTX::stSetComp = static_cast<SyntaxNodeIDREF*>(this->stageset)->ref;
	}
	else
	{
		SCTX::stSetComp = static_cast<SyntaxNodeIDREF*>(this->stageset->values[1])->ref;
		SCTX::st = static_cast<SyntaxNodeID*>(this->stageset->values[0]->values[0])->id;
	}

	AmplModel* prev_model = parent; //the ample model above
	Set* stset = static_cast<Set*>(parCtx->getCompValue(SCTX::stSetComp));
	for(uint i = 0; i < stset->setValues_data_order.size(); i++)
	{
		//create node set in prev_model that the current stage model is repeated on.
		curr_sctx->stagename = stset->setValues_data_order[i];
		curr_sctx->stage_level  = i;
		curr_sctx->model_dummy = GV(node_dummy_prefix) + curr_sctx->stagename;

		string model_set_name = "model_set" + curr_sctx->stagename;
		//NODES
		SyntaxNode* node_idref = new SyntaxNodeIDREF(new SyntaxNodeID(SCTX::ndSetComp->name),SCTX::ndSetComp);
		//n0 in NODES
		SyntaxNode* node_set_index_list = new SyntaxNode(COMMA,new SyntaxNode(IN, new SyntaxNode(COMMA, new SyntaxNodeID(curr_sctx->model_dummy)),node_idref));
		//Parent
		SyntaxNode* node_paref = new SyntaxNodeIDREF(new SyntaxNodeID(SCTX::paSetComp->name),SCTX::paSetComp);
		node_paref->push_back(new SyntaxNode(COMMA,new SyntaxNodeID(curr_sctx->model_dummy)));  //Parent[n0]
		SyntaxNode* prev_model_dummy = NULL;
		if(i == 0){
			prev_model_dummy = new SyntaxNodeString(GV(root_stage_parent_name));
		}
		else{
			prev_model_dummy = new SyntaxNodeID(curr_sctx->parent->model_dummy);
		}
		SyntaxNode* node_set_cond = new SyntaxNode(EQ,node_paref,prev_model_dummy); // Parent[n0] == "null"
		SyntaxNode* node_set = new SyntaxNode(LBRACE,new SyntaxNode(COLON,node_set_index_list,node_set_cond)); //{n0 in NODES: Parent[n0] == "null" }
		SyntaxNode* node_set_attr = new SyntaxNode(ASSIGN, node_set); // = {n0 in NODES: Parent[n0] == "null"}
		SyntaxNode* node_set_attributes = new SyntaxNode(COMMA,node_set_attr);
		SetComp* model_set = new SetComp(model_set_name,NULL, node_set_attributes,prev_model);
		prev_model->addComp(model_set);

		//create indexing for curr_model
		SyntaxNode* model_set_ref = new SyntaxNodeIDREF(new SyntaxNodeID(model_set->name),model_set);
		SyntaxNode* model_index_list = new SyntaxNode(COMMA, new SyntaxNode(IN, new SyntaxNode(COMMA, new SyntaxNodeID(curr_sctx->model_dummy)), model_set_ref));
		SyntaxNode* model_index = new SyntaxNode(LBRACE,new SyntaxNode(COLON, model_index_list, NULL));

		//create the curr_model for current stage
		string model_name = this->name + "STAGE" + curr_sctx->stagename;
		curr_sctx->model = new AmplModel(model_name, model_index ,prev_model);

		if(i==0) {
			SCTX::rootCtx = curr_sctx;
		}

		//now scan ModelComps and add model comp belong to current stage into the current model.
		BOOST_FOREACH(SetComp* setcomp , this->set_comps)
		{
			if(this->isInCurrentStage(setcomp->stage,curr_sctx->stagename,parCtx))
			{
				LOG("setcomp ["<<setcomp->name<<"] is in model["<<curr_sctx->model->name<<"]");
				SyntaxNode* index = setcomp->indexing==NULL? NULL :setcomp->indexing->clone();
				SyntaxNode* attr = setcomp->attributes==NULL? NULL :setcomp->attributes->clone();
				SetComp* nsetcomp = new SetComp(setcomp->name,index,attr,curr_sctx->model);
				curr_sctx->model->addComp(nsetcomp);
			}
		}
		BOOST_FOREACH(ParamComp* paramcomp , this->param_comps)
		{
			if(this->isInCurrentStage(paramcomp->stage,curr_sctx->stagename,parCtx))
			{
				LOG("paramcomp ["<<paramcomp->name<<"] is in model["<<curr_sctx->model->name<<"]");
				SyntaxNode* index = paramcomp->indexing==NULL? NULL : paramcomp->indexing->clone();
				SyntaxNode* attr = paramcomp->attributes==NULL? NULL :paramcomp->attributes->clone();
				ParamComp* paramcomp = new ParamComp(paramcomp->name,index,attr,curr_sctx->model);
				curr_sctx->model->addComp(paramcomp);
			}
		}
		BOOST_FOREACH(VarComp* varcomp , this->var_comps)
		{
			if(this->isInCurrentStage(varcomp->stage,curr_sctx->stagename,parCtx))
			{
				LOG("varcomp ["<<varcomp->name<<"] is in model["<<curr_sctx->model->name<<"]");
				SyntaxNode* index = varcomp->indexing==NULL? NULL : varcomp->indexing->clone();
				SyntaxNode* attr = varcomp->attributes==NULL? NULL : varcomp->attributes->clone();
				VarComp* nvarcomp = new VarComp(varcomp->name,index,attr,curr_sctx->model);
				curr_sctx->model->addComp(nvarcomp);
			}
		}

		BOOST_FOREACH(ConsComp* concomp , this->con_comps)
		{
			if(this->isInCurrentStage(concomp->stage,curr_sctx->stagename,parCtx))
			{
				LOG("concomp ["<<concomp->name<<"] is in model["<<curr_sctx->model->name<<"]");
				//handling Exp operator:
				//if there is Exp in constraint, need to move this constraint to stage 0
				//create stage level sets and sum over the weight expr involved in the Exp
				//this again will make a separable constraints over multiple child scenarios
				//should update into a DOT operator in SyntaxNodeIDREFM

				//handling Ancestor operator:
				//convert ancestor node into a IDREF node that reference to the correct model comp at correct model level
				bool exp = concomp->attributes->hasExp();
				if(exp == true)
				{
					//clone will: replace Exp[expr] ===> sum{n1 in NODES: Parent[n1]=n0} ( Prob[n1] * sum{n2 in NODES: Parent[n2]=n1}( Prob[n2]*almSTAGE1[n1].almSTAGE2[n2].expr ) )
					//if expectation expr in stage 2 as an example.
					//Dot notation also appended if necessary to convert the IDREF node into DOT node with IDREFM node. (ie. the dot notation to reference a comp below).
					SyntaxNode* attr = concomp->attributes->clone();
					//need to move this constraint to stochroot level
					SyntaxNode* index = concomp->indexing==NULL?NULL:concomp->indexing->clone();
					string nconname = concomp->name + "_EXP" + curr_sctx->stagename;
					//create the constraint comp in stoch root model.
					ConsComp* nconcomp = new ConsComp(nconname,index,attr,SCTX::rootCtx->model);
					SCTX::rootCtx->model->addComp(nconcomp);
				}
				else
				{
					SyntaxNode* index = concomp->indexing==NULL? NULL :concomp->indexing->clone();
					SyntaxNode* attr = concomp->attributes==NULL? NULL : concomp->attributes->clone();
					ConsComp* nconcomp = new ConsComp(concomp->name,index,attr,curr_sctx->model);
					curr_sctx->model->addComp(nconcomp);
				}
			}
		}
		if(obj_comp!=NULL)
		{
			if(this->isInCurrentStage(obj_comp->stage,curr_sctx->stagename,parCtx))
			{
				LOG("obj_comp ["<<obj_comp->name<<"] is in model["<<curr_sctx->model->name<<"]");
				assert(obj_comp->indexing == NULL);  //single objective no indexing
				bool exp = obj_comp->attributes->hasExp();
				assert(exp == false); //obj can't has expectation opcode, because objective in always weighted by the probs reach it's stage
				assert(i == stset->setValues_data_order.size()-1); //obj has to be defined only in the final stage
				SyntaxNode* attr = obj_comp->attributes->clone();
				//now need to weight the obj attribute by probs reach it
				for(uint j = 1; j <= i; j++)
				{//starting from stage 1 to curr stage, because stage 0 is only root node
					string pddummy = GV(node_dummy_prefix) + stset->setValues_data_order[j]; //pddummy must be one of the model_dummy_name
					SyntaxNodeIDREF* pdrefn = new SyntaxNodeIDREF(new SyntaxNodeID(SCTX::pbSetComp->name),SCTX::pbSetComp);
					pdrefn->push_back(new SyntaxNode(COMMA,new SyntaxNodeID(pddummy)));
					attr = new SyntaxNodeOP(TIMES,attr,pdrefn);
				}
				ObjComp* nobjcomp = new ObjComp(obj_comp->name,obj_comp->otype,attr,curr_sctx->model);
				curr_sctx->model->addComp(nobjcomp);
			}
		}
		assert(this->subm_comps.size() == 0); //not yet know how to deal with submodels in stochastic model

		//adding the new created ampl model to model tree
		assert((curr_sctx->parent==NULL && prev_model==parent) || prev_model == curr_sctx->parent->model);
		prev_model->addComp(curr_sctx->model);
		prev_model = curr_sctx->model;
		curr_sctx = new StochCtx(curr_sctx);
	}

	if(GV(logModel)){
		string mfile = GV(logdir)+GV(logModelFile)+"_stoch";
		AmplModel::root->logModel(mfile.c_str());
	}
	return SCTX::rootCtx->model;
}


/*
 * Is the stagename in the stage represented by stage expression in stage
 */
bool StochModel::isInCurrentStage(SyntaxNode* stage, string& stagename, ModelContext* ctx)
{
	if(stage == NULL) return true; //by default if in stochastic model, and no stage related to a model comp, then it belong to all stages
	boost::unordered_map<SyntaxNode*,boost::unordered_set<string>* >::iterator it = this->stageSetMap.find(stage);
	boost::unordered_set<string>* stageset = NULL;
	if(it == this->stageSetMap.end())
	{//now compute the stage set
		stageset = new boost::unordered_set<string>();
		stage->calcStageSet(ctx,stageset);
		this->stageSetMap.insert(pair<SyntaxNode*,boost::unordered_set<string>* >(stage,stageset));
	}
	else
	{
		stageset = it->second;
	}
	boost::unordered_set<string>::iterator s = stageset->find(stagename);
	if(s!=stageset->end()) return true;
	return false;
}

