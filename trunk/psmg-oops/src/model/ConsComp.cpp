/*
 * ConsComp.cpp
 *
 *  Created on: 7 Jan 2014
 *      Author: s0965328
 */

#include "ConsComp.h"
#include "SyntaxNode.h"
#include "AmplModel.h"

ConsComp::ConsComp(const string& id, SyntaxNode* index, SyntaxNode* attr): ModelComp(id,TCON,index, attr),card(0)
//	moveUpLevel(0)
{
	// TODO Auto-generated constructor stub

}

ConsComp::~ConsComp() {
	// TODO Auto-generated destructor stub
}

void ConsComp::calculateLocalCon(ModelContext* ctx)
{
	LOG("calculateLocalCon -- in model["<<this->model->name<<"] id["<<this->name<<"]");
	this->card = 1;
	if(this->indexing!=NULL){ //a single constraint
		this->indexing->calculateConCard(ctx,this->card);
	}
	LOG( "calculateLocalVar -- in model["<<this->model->name<<"] id["<<this->name<<"] -- card["<<card<<"]");
}

void ConsComp::moveConsToLeft()
{
	LOG("ConsComp::moveConsToLeft --name["<<name<< "] - indx["<<this->indexing->print()<<"]-- attr["<<this->attributes->print()<<"] - declared level["<<this->model->level<<"]");
	SyntaxNode* ret = this->attributes->moveConsToLeft();
	this->attributes = ret;
}

void ConsComp::calculatePartialConstraints()
{
	LOG("ConsComp::calculatePartialConstraints --id["<<name<< "] - indx["<<this->indexing->print()<<"]-- attr["<<this->attributes->print()<<"] - declared level["<<this->model->level<<"]");
	this->attributes->calculatePartialConstraints(this->partial);

	boost::unordered_map<int,SyntaxNode*>::iterator it = this->partial.begin();
	for(;it!=this->partial.end();it++)
	{
		SyntaxNode* node = (*it).second;
		LOG("level ["<<(*it).first<<"] - ["<<node->print()<<"]");
	}
}


/* ---------------------------------------------------------------------------
 ConsComp::dump(ostream &fout)
 ---------------------------------------------------------------------------- */
void ConsComp::dump(ostream& fout,int counter)
{
	fout << "ConsComp:  ("<<counter<<"------------------------------------------------------\n";
	fout << "ConsComp: " << name << " (" << (void *) this << ")\n";
	if (attributes) {
		fout << "    attr: " << attributes << '\n';
	}
	if (indexing) {
		fout << "    indexing: " << indexing << "\n";
	}
}


/* --------------------------------------------------------------------------
 ModelComp::moveUp(int level)
 ---------------------------------------------------------------------------- */
/** Queue the ModelComp to be moved up by 'level' levels in the model tree:
 *  Just removing the component from the current model and adding it to a
 *  parent is dangerous, since ModelComp::moveUp is typically called from
 *  within a (nested) loop over all ModelComps (->comps) in the AmplModels
 *  removing/adding items to list<ModelComp*> comps while there is an
 *  iterator running over it will invalidate that iterator.
 *  => hence the request to move is scheduled to be executed by
 *     AmplModel::applyChanges() after the loop over all components
 *
 *  This method will also re-write the component for the new model
 *  I.e. all IDREFs to components below the new model will have their
 *  local indexing expression expanded
 */
void ConsComp::moveUp(int level) {
	LOG( "enter moveUp --["<<level<<"]-- id["<<this->name<<"] indexing["<<this->indexing->print()<<"] attribute["<<this->attributes->print()<<"]");
//	AmplModel *current = model;
//	int i, posm;
//
//	// -------------------- Expand local indexing expression -----------------
//	/* This ModelComp is written for the 'current' model and is now re-assigned
//	 to a different model. In order for that to work the indexing expressions
//	 in all IDREFs in its attribute/indexing section have got to be
//	 rewritten.
//
//	 Indexing expressions applicable to a IDREF are divided into local and
//	 block indexing. 'local' is directly associated with the IDREF (as
//	 arguments in ->values[]. 'block' originate from the indexing expressions
//	 of the blocks up to the current model in the model tree. Both indexing
//	 expression together need to combine to get the correct global indexing.
//
//	 When moving a ModelComp up in the tree, we therefore need to do the
//	 following to have correct global indexing:
//	 - all IDREFs to ModelComp's in models below the new model
//	 (current.parent(level)) need to have the block indexing expressions
//	 between their own model and current.parent(level) added
//	 to their local indexing
//	 */
//
//	// get list of models from current model to root
//	vector<AmplModel*> mlist;
//	int nlevels = 0;
//	for(AmplModel *tmp = current;tmp->parent != NULL;tmp = tmp->parent) {
//		LOG("adding model["<<tmp->name<<"] to list");
//		mlist.push_back(tmp);
//		nlevels++;
//	}
//	// it's possible to move the model up by at most as many levels as there
//	// are from here to the root
//	assert(nlevels - level > 0);
//
//	// get list of all IDREF nodes in dependencies
//	list<SyntaxNode*> idrefnodes;
//	if (indexing)
//		indexing->findIDREF(&idrefnodes);
//	if (attributes)
//		attributes->findIDREF(&idrefnodes);
//
////	//--- All the depended ModelComp also should be moved up --Feng (two parts fix)
////	vector<AmplModel*> upModelList;
////	int upLevel = level;
////	for(AmplModel* tmp=current;tmp->parent!=NULL;tmp=tmp->parent)
////	{
////		if(upLevel == 0)
////		{
////			LOG("adding model["<<tmp->name<<"] to upModelList");
////			upModelList.push_back(tmp);
////		}
////		else
////		{
////			upLevel--;
////		}
////	}
////	//end fix -- part 1
//
//	// loop over all IDREF nodes
//	list<SyntaxNode*>::const_iterator p;
//	for(p = idrefnodes.begin();p != idrefnodes.end();++p) {
//		SyntaxNodeIDREF *currSN = dynamic_cast<SyntaxNodeIDREF*>(*p);
//		ModelComp *currMC = currSN->ref;
//		AmplModel *currAM = currMC->model;
//		LOG( "-- Moveup -- at currSN["<<currSN->print()<<"] currMC["<<currMC->id<<"] currAM["<<currAM->name<<"]");
//
//		// need to check if this model is below the new assigned model
//		bool found = false;
//		for(posm = 0;posm < level;posm++) {
//			if (mlist[posm] == currAM) {
//				found = true;
//				break;
//			}
//		}
//		if (found) {
//			LOG( "found currMC["<<currMC->id<<"] currAM["<<currAM->name<<"] syntaxNode["<<currSN->print()<<"] at posm["<<posm<<"]");
//			// this is a model between the old and new model for ModelComp *this
//			// posm gives the position: 0 is the old model, level is the new
//			// one
//			// => need to add indexing expressions between posm and level-1
//			// starting with level-1
//			for(i = posm;i < level;++i) {
//				SyntaxNode *mix = mlist[i]->node->indexing;
//				if (mix->getNComp() != 1) {
//					cerr << "ModelComp::moveUp() does not support intermediate models with !=1 dummy Var" << endl;
//					exit(1);
//				}
//				currSN->push_front(mix->getDummyVarExpr(0));
//				/* indexing dummy var of mlist[level-1-i]*/
//				LOG("["<<i<<"] - now syntaxNode["<<currSN->print()<<"]");
//			}
//		}
//
////		bool isDefined = false;
////		for(vector<AmplModel*>::iterator it=upModelList.begin();it!=upModelList.end();it++)
////		{
////			//hack!
////			if(currAM->name.compare("root")==0)
////			{
////				LOG("currMC["<<currMC->id<<"] is in root! ");
////				isDefined = true;
////				break;
////			}
////			if((*it)==currAM)
////			{
////				LOG("currMC["<<currMC->id<<"] is defined ");
////				isDefined = true;
////				break;
////			}
////		}
////		if(!isDefined)
////		{
////			LOG("currMC["<<currMC->id<<"] is not defined! ");
////			changeitem rem = {currMC,currAM,CHANGE_REM};
////			changeitem add = {currMC,mlist[level],CHANGE_ADD};
////			AmplModel::changes.push_back(rem);
////			AmplModel::changes.push_back(add);
////		}
//	}
//	this->moveUpLevel = level;
//	LOG("setting for ModelComp["<<this->id<<"] moveUpLevel["<<this->moveUpLevel<<"]");
//	// and queue this item to be moved up by AmplModel::applyChanges
//	changeitem rem = { this, model, CHANGE_REM };
//	AmplModel::changes.push_back(rem); // Q for removal
//	model = mlist[level];
//	changeitem add = { this, model, CHANGE_ADD };
//	AmplModel::changes.push_back(add);
	assert(false); //fixme: this is for handling expectation constraints
	LOG("exit moveUp --["<<level<<"]-- id["<<this->name<<"] indexing["<<this->indexing->print()<<"] attribute["<<this->attributes->print()<<"]");
}
