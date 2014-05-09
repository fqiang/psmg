/*
 * SyntaxNode1.cpp
 *
 *  Created on: 28 Sep 2011
 *      Author: s0965328
 */

#include <queue>
#include <cassert>
#include <typeinfo>
#include <boost/foreach.hpp>
#include <boost/unordered_map.hpp>
#include "SyntaxNode.h"
#include "SyntaxNodeIDREF.h"
#include "SyntaxNodeIDREFM.h"
#include "SyntaxNodeValue.h"
#include "SyntaxNodeString.h"
#include "SyntaxNodeSumExp.h"
#include "SyntaxNodeID.h"
#include "SyntaxNodeOP.h"
#include "SetComp.h"
#include "AmplModel.h"
#include "ParamComp.h"
#include "../st_model/StochCtx.h"

#include "../context/ModelContext.h"
#include "../context/ExpandedModel.h"
#include "../context/SetSimple.h"
#include "../context/SetOrdered.h"
#include "../context/ParamMult.h"
#include "../context/ParamSingle.h"
#include "../context/PValueValue.h"
#include "../context/PValueSym.h"
#include "../context/IndexSet.h"
#include "../context/Var.h"
#include "../context/VarSingle.h"
#include "../context/Param.h"
#include "../util/util.h"
#include "../util/DummyVarGen.h"

#include "../parser/sml.tab.h"

using namespace std;
using namespace boost;

SyntaxNode *SyntaxNode::push_back(SyntaxNode *newitem) {
	values.push_back(newitem);
	return this;
}

string SyntaxNode::print() {
	ostringstream ost;
	ost << (*this);
	return ost.str();
}

/* ==========================================================================
 SyntaxNode Methods to follow
 ============================================================================*/
SyntaxNode::SyntaxNode(const SyntaxNode &src) :opCode(src.opCode) {
	LOG("calling SyntaxNode copy constructor ..");
	for(SyntaxNode::iterator it = src.begin();it!=src.end();it++)
	{
		SyntaxNode* node = (*it)->clone();
		this->values.push_back(node);
	}
}

SyntaxNode* SyntaxNode::clone(){
	SyntaxNode* rval = NULL;
	if(opCode==ANCESTOR)
	{//need to convert to a IDREF node to reference the model comp in the correct ancestor model
		assert(this->nchild()==2); // must be 2 children: int value node and idref node.
		assert(values[1]->opCode == IDREF && values[0]->opCode == VALUE);
		StochCtx* record_sctx = SCTX::currCtx;
		int labove = static_cast<SyntaxNodeValue*>(this->values[0])->val;
		//now update the currCtx in Stoch Context
		for(int i=0;i<labove;i++) SCTX::currCtx = SCTX::currCtx->parent;
		rval = this->values[1]->clone();

		//must be in the same level, because currCtx is mean to update the the model that owns this comp
		//ie. ancestor(1).comp , the ancestor(1) will first processed and upate the SCTX::currCtx to the context that owned this
		//model comp -- sanity check!
		assert(static_cast<SyntaxNodeIDREF*>(rval)->ref!=NULL && static_cast<SyntaxNodeIDREF*>(rval)->ref->model == SCTX::currCtx->model);

		//restore backup SCTX
		SCTX::currCtx = record_sctx;
	}
	else if(opCode == EXPECTATION)
	{//replace Exp[expr] ===> sum{n1 in NODES: Parent[n1]=n0} ( Prob[n1] * sum{n2 in NODES: Parent[n2]=n1}( Prob[n2]* almSTAGE1[n1].almSTAGE[n2].expr ) )
		assert(nchild()==1);
		rval = values[0]->clone(); //make copy of the expr of EXP[expr]
		int nProb = 0;
		for(StochCtx* i = SCTX::currCtx;i!=SCTX::rootCtx;i=i->parent)
		{
			rval = rval->appendDOTNotation(i);
			SyntaxNodeIDREF* probn = new SyntaxNodeIDREF(new SyntaxNodeID(SCTX::pbSetComp->name),SCTX::pbSetComp);
			SyntaxNode* probn_expr_list = new SyntaxNode(COMMA,new SyntaxNodeID(i->model_dummy));
			probn->push_back(probn_expr_list);
			SyntaxNode* sum_expr = new SyntaxNodeOP(TIMES,probn,rval);
			nProb++;

			//instead of creating a normal sum node, a special node SUMEXP will be created for the EXPECTATION operation
			//- this is to efficiently handling sum expression
//			SyntaxNodeIDREF* parn = new SyntaxNodeIDREF(new SyntaxNodeID(SCTX::paSetComp->name),SCTX::paSetComp);
//			SyntaxNode* par_expr_list = new SyntaxNode(COMMA,new SyntaxNodeID(i->model_dummy));
//			parn->push_back(par_expr_list);
//			SyntaxNode* condn = new SyntaxNode(EQ,parn,new SyntaxNodeID(i->parent->model_dummy));
//			SyntaxNodeIDREF* noden = new SyntaxNodeIDREF(new SyntaxNodeID(SCTX::ndSetComp->name),SCTX::ndSetComp);
//			SyntaxNode* index_expr = new SyntaxNode(IN, new SyntaxNode(COMMA,new SyntaxNodeID(i->model_dummy)),noden);
//			SyntaxNode* index_expr_list = new SyntaxNode(COMMA,index_expr);
//			SyntaxNode* sum_index_set = new SyntaxNode(LBRACE,new SyntaxNode(COLON,index_expr_list,condn));
//			rval = new SyntaxNode(SUM,sum_index_set,sum_expr);

			rval = sum_expr;
		}
		rval = new SyntaxNodeSumExp(rval,nProb);
		LOG("clone :: EXPECTATION convert to ["<<rval->print()<<"]");
	}
	else
		rval = new SyntaxNode(*this);
	return rval;
}

SyntaxNode::SyntaxNode(int code, SyntaxNode *val1, SyntaxNode *val2, SyntaxNode* val3) :opCode(code) {
	if (val1 != NULL) {
		values.push_back(val1);
	}
	if (val2 != NULL) {
		values.push_back(val2);
	}
	if (val3 != NULL) {
		values.push_back(val3);
	}
	LOG("SyntaxNode constructor  --- "<<values.size()<<"<- opCode["<<opCode<<"] val1:["<<val1<<"] val2:["<<val2<<"] val3:["<<val3<<"]");
}

SyntaxNode::~SyntaxNode() {
	for(vector<SyntaxNode*>::iterator it = values.begin();it != values.end();it++) {
		delete *it;
	}
	values.clear();
}


ostream& SyntaxNode::put(ostream&s) {
	if (this == NULL)
		return s;

	SyntaxNode::iterator i = this->begin();

	switch (this->opCode)
	{
		case DOT:
			s << **i << ".";
			s << **(++i);
			break;
		case COMMA:
			s << **i;
			while(++i != end())
				s << "," << (**i);
			break;
		case DIFF:
			if (this->nchild() > 1)
				s << **(i++);
			s << " diff " << **i;
			break;
		case CROSS:
			if (this->nchild() > 1)
				s << **(i++);
			s << " cross " << **i;
			break;
		case UNION:
			if (this->nchild() > 1)
				s<<**(i++);
			s<<" union "<<**i;
			break;
		case DOTDOT:
			if (this->nchild() > 1)
				s << **(i++);
			s << " .. " << **i;
			break;
		case SUM:
			s << "sum " << **i;
			s << "(" << **(++i) <<")";
			break;
		case MAX:
			s << "max " << **i;
			s << **(++i);
			break;
		case MIN:
			s << "min " << **i;
			s << **(++i);
			break;
		case EXPECTATION:
			s << "Exp( " << **i << ")";
			break;
		case LAST:
			s << "last( " << **i << ")";
			break;
		case FIRST:
			s << "first( " << **i << ")";
			break;
			// -------------------------functions f(..) --------------------------
		case ORD:
			s << "ord" << **i;
			break;
		case CARD:
			s << "card" << **i;
			break;
			// -------------------------terminals --------------------------
		case ORDERED:
			s << "ordered";
			break;
		case SYMBOLIC:
			s << "symbolic";
			break;
		case DETERMINISTIC:
			s << "deterministic";
			break;
			/* these are lots of simple unary operators */
		case WITHIN:
			s << "within " << **i;
			break;
		case LBRACE:
			s << "{" << *i << "}";
			break;
		case COLON:
			s << **i;
			if (this->nchild()==2){
				s << " : " << **(++i);
			}
			assert(this->nchild()==1 || this->nchild()==2);
			break;
		case IF:
			s << "if " << **i;
			s << " then " << **(++i);
			if (this->nchild() == 3)
				s << " else " << **(++i);
			break;
		case AND:
			s << **i;
			s << " && ";
			s << **(++i);
			break;
		case NE:
			s << **i;
			s << " != ";
			s << **(++i);
			break;
		case EQ:
			s << **i;
			s << " == ";
			s << **(++i);
			break;
		case GT:
			s << **i;
			s << " > ";
			if (this->nchild() == 2)
				s << **(++i);
			break;
		case GE:
			s << **i;
			s << " >= ";
			if (this->nchild() == 2)
				s << **(++i);
			break;
		case LE:
			s << **i;
			s << " <= ";
			if (this->nchild() == 2)
				s << **(++i);
			break;
		case LT:
			s << **i;
			s << " < ";
			if (this->nchild() == 2)
				s << **(++i);
			break;
		case ASSIGN:
			s<<**i;
			s<<" = ";
			if (this->nchild() == 2)
				s << **(++i);
			break;
		case DEFINED:
			s<<**i;
			s << ":=";
			if (this->nchild() == 2)
				s << **(++i);
			break;
		case SETOF:
			s<<**i;
			s<<" setof ";
			s<<**(++i);
			break;
		case IN:
			s<<**i;
			s<<" IN ";
			s<<**(++i);
			break;
		case COS:
			s<<" cos(";
			s<<**i;
			s<<")";
			break;
		case SIN:
			s<<" sin(";
			s<<**i;
			s<<")";
			break;
		case ANCESTOR:
			s<<" ancestor(";
			s<<**i;
			s<<",";
			if (this->nchild() == 2)
				s << **(++i);
			s<<")";
			break;
		default:
			LOG("Unknown opcode ["<<this->opCode<<"]");
			LOG("nchild=[" << this->nchild() << "]");
			for(;i != this->end();++i) {
				LOG("\t["<<**i<<"]");
			}
			assert(false);
	}
	return s;
}

//Feng
void SyntaxNode::calculateConCard(ModelContext* ctx,int& card)
{
	LOG(this->print());
	assert(this->opCode == LBRACE);
	assert(this->values[0]->opCode == COLON);
	SyntaxNode* setexpr_list = this->values[0]->values[0];
	assert(setexpr_list->nchild()==1); //only support one dummy index
	IndexSet* iset = this->createIndexSet(ctx);
	assert(iset->tuples.size()== 1); //only support one dummy index for constraint now
	card = iset->tuples.begin()->set->card;
	delete iset;
}
void SyntaxNode::calculateVarDimCard(ModelContext* ctx, uint& dim, uint& card) {
	LOG(this->print());
	assert(this->opCode == LBRACE);
	assert(this->values[0]->opCode == COLON);
	IndexSet* iset = this->createIndexSet(ctx);
	dim = iset->tuples.size();
	BOOST_FOREACH(iset_tuple& tuple, iset->tuples){
		card = card * tuple.set->card;
	}
	delete iset;
}

void SyntaxNode::calculateVarBounds(ModelContext* ctx, double& upper, double& lower)
{
	LOG( "calculateVarDounds --  opCode["<<this->opCode<<"]  ["<<this->print()<<"]");
	assert(this->opCode == COMMA);
	SyntaxNode::iterator i = this->begin();
	for(;i!=this->end();i++){
		if ((*i)->opCode == GE) {
			assert((*i)->nchild()==1);
			PValue* pval = NULL;
			(*i)->values[0]->evalTerm(ctx,&pval);
			assert(typeid(*pval)==typeid(PValueValue));
			lower = static_cast<PValueValue*>(pval)->value;
			delete pval;
		}
		else if((*i)->opCode == LE)
		{
			assert((*i)->nchild()==1);
			PValue* pval = NULL;
			(*i)->values[0]->evalTerm(ctx,&pval);
			assert(typeid(*pval)==typeid(PValueValue));
			upper = static_cast<PValueValue*>(pval)->value;
			delete pval;
		}
		else {
			LOG("other bounds not yet implemented");
			assert(false);
		}
	}
	LOG( "calculateVarDounds --  upper["<<upper<<"]  lower["<<lower<<"]");
}

void SyntaxNode::calculateConsBounds(ModelContext* ctx, double& upper, double& lower)
{
	LOG("calculateConsBounds --  opCode["<<this->opCode<<"]  ["<<this->print()<<"]");
	assert(this->opCode == COMMA);
	BOOST_FOREACH(SyntaxNode* attr, this->values)
	{
		PValue* pval = NULL;
		if(attr->opCode == ASSIGN) {
			assert(attr->nchild()==2);
			attr->values[1]->evalTerm(ctx,&pval);
			assert(typeid(*pval)==typeid(PValueValue));
			lower = static_cast<PValueValue*>(pval)->value;
			upper = static_cast<PValueValue*>(pval)->value;
		}
		else if(attr->opCode == GE)
		{
			attr->values[0]->evalTerm(ctx,&pval);
			assert(typeid(*pval)==typeid(PValueValue));
			lower = static_cast<PValueValue*>(pval)->value;
		}
		else if(attr->opCode == LE){
			attr->values[0]->evalTerm(ctx,&pval);
			assert(typeid(*pval)==typeid(PValueValue));
			upper = static_cast<PValueValue*>(pval)->value;
		}
		else {
			LOG("other bounds not yet implemented");
			assert(false); //ass opcode not yet implemented
		}
		delete pval;
	}

	LOG( "calculateConsBounds --  upper["<<upper<<"]  lower["<<lower<<"]");
}

int SyntaxNode::calculateSetDim() {
	LOG(this->print());
	int dim = 0;
	if(this->opCode == WITHIN)
	{
		dim = this->values[0]->calculateSetDim();
	}
	else if (this->opCode == ASSIGN) { //OpNode
		assert(this->values[0] != NULL);
		assert(this->values.size() == 1);
		dim = this->values[0]->calculateSetDim();
	}
	else if (this->opCode == DIFF) { //CompositeSet
		assert(this->values[0] != NULL && this->values[1] != NULL);
		dim = this->values[0]->calculateSetDim();
	}
	else if (this->opCode == CROSS) { //CompositeSet
		assert(this->values[0] != NULL && this->values[1] != NULL);
		dim = this->values[0]->calculateSetDim() + this->values[1]->calculateSetDim();
	}
	else if (this->opCode == IDREF) { //SyntaxNodeIDREF
		SyntaxNodeIDREF* nodeIdRef = static_cast<SyntaxNodeIDREF*>(this);
		dim =  static_cast<SetComp*>(nodeIdRef->ref)->setDim;
	}
	else if (this->opCode == ID) { //SyntaxNodeID
		dim = 1;
	}
	else if (this->opCode == LBRACE) { //indexing_set - only indexing_set can has LBRACE as opCode
		assert(this->nchild()==1);
		assert(this->values[0]->opCode == COLON);
		SyntaxNode* indexing_set_expr_list = this->values[0]->values[0];
		assert(indexing_set_expr_list->opCode == COMMA); //comma separate list of ( i in Set, j in Set2)
		assert(indexing_set_expr_list->nchild()==1); // only support 1-index set for now
		SyntaxNode* set_expr = indexing_set_expr_list->values[0];
		assert(set_expr->opCode == IN);
		SyntaxNodeIDREF* idref = static_cast<SyntaxNodeIDREF*>(set_expr->values[1]);
		SetComp* set = static_cast<SetComp*>(idref->ref);
		dim = set->setDim;
	}
	else if (this->opCode == DOTDOT) { //SimpleSet
		dim = 1;
	}
	else if(this->opCode == UNION)
	{
		dim = this->values[0]->calculateSetDim();
	}
	else if (this->opCode == ORDERED) { //ordered property
		dim = 1;
	}
	else if (this->opCode == COLON) {
		dim = this->values[0]->calculateSetDim();
	}
	else if (this->opCode == SETOF)
	{
		dim = this->values[1]->calculateSetDim();
		assert(false); //dim of a setof constructed set will need to be decided by within attribute
	}
	else {
		LOG("opCode["<<this->opCode<<"] was not yet implemented --- calculateSetDim");
		assert(false);
	}
	return dim;
}


bool SyntaxNode::isParamSymbolic()
{
	bool rval = false;
	if(this->opCode == COMMA)
	{
		SyntaxNode::iterator i;
		for(i=this->begin();i!=this->end();i++)
		{
			if((*i)->opCode == SYMBOLIC)
			{
				rval = true;
				break;
			}
		}
	}
	return rval;
}

int SyntaxNode::calculateParamSetIndicies()
{
	LOG("calculateParamSetIndicies - opCode["<<opCode<<"]");
	assert(this->opCode == LBRACE);
	assert(this->values[0]->opCode == COLON);
	int rval = 0;
	SyntaxNode* setexpr_list = this->values[0]->values[0];
	assert(setexpr_list->opCode == COMMA);

	SyntaxNode::iterator i;
	for(i=setexpr_list->begin();i!=setexpr_list->end();i++)
	{
		assert((*i)->opCode == IN || (*i)->opCode == IDREF); //only supports one-dim set now.
		rval ++ ;
	}
	return rval;
}

void SyntaxNode::calculateParamIndicies(vector<string>& paramIndiciesDummy, vector<SetComp*>& paramIndiciesComp) {
	LOG("calculateParamIndicies - opCode["<<this->opCode<<"]  "<<this->print());
	assert(this->opCode == LBRACE);
	assert(this->values[0]->opCode == COLON);
	assert(this->values[0]->nchild() == 1); // not yet support for condition on dummy index for param declaration.
	SyntaxNode* setexpr_list = this->values[0]->values[0];
	assert(setexpr_list->opCode == COMMA);
	for(SyntaxNode::iterator it = setexpr_list->begin();it!=setexpr_list->end();it++)
	{
		if ((*it)->opCode == IDREF) {
			SyntaxNodeIDREF* refn = static_cast<SyntaxNodeIDREF*>(this);
			assert(refn->ref->type == TSET);
			string dummy = DummyVarGen::nextDummy();
			paramIndiciesDummy.push_back(dummy);
			paramIndiciesComp.push_back(static_cast<SetComp*>(refn->ref));
		}
		else if((*it)->opCode == IN)
		{
			assert((*it)->nchild()==2); //ie. a IN Arcs
			assert((*it)->values[0]->opCode == COMMA);
			assert((*it)->values[0]->nchild() == 1); //only support one dummy var
			assert((*it)->values[0]->values[0]->opCode == ID);
			assert((*it)->values[1]->opCode == IDREF);
			SyntaxNodeID* idn = static_cast<SyntaxNodeID*>((*it)->values[0]->values[0]);
			SyntaxNodeIDREF* refn = static_cast<SyntaxNodeIDREF*>((*it)->values[1]);
			assert(refn->ref->type == TSET);
			SetComp* setcomp = static_cast<SetComp*>(refn->ref);
			assert(idn!=NULL && refn!=NULL);
			paramIndiciesDummy.push_back(idn->id);
			paramIndiciesComp.push_back(setcomp);
			LOG("add parameter indices ["<<idn->id<<"]--->["<<refn->ref->name<<"]");
		}
	}
}

void SyntaxNode::calculateSetValue(ModelContext* context,Set* rval)
{
	LOG("calculateSetValue - "<<this->print());
	Set* set = calculateSetValue(context);
	//copy set values to rval
	rval->copyFromSet(set);
	LOG("calculateSetValue - set returned -- "<<rval->toString());
}
//non-recurisve method
Set* SyntaxNode::calculateSetValue(ModelContext* context) {
	LOG(this->print());
	Set* rval = NULL;
	if (this->opCode == CROSS) {
		assert(this->nchild() == 2);
		Set* left = this->values[0]->calculateSetValue(context);
		Set* right = this->values[1]->calculateSetValue(context);
		rval = new SetSimple(Set::TMP,left->dim+right->dim);
		assert(left!=NULL && right!= NULL);
		left->setCross(right,rval);
		Set::deleteSet(left);
		Set::deleteSet(right);
	}
	else if (this->opCode == DIFF) {
		assert(this->nchild() == 2);
		Set* left = this->values[0]->calculateSetValue(context);
		Set* right = this->values[1]->calculateSetValue(context);
		assert(left != NULL && right != NULL);
		rval = new SetSimple(Set::TMP,left->dim);
		left->setDiff(right,rval);
		Set::deleteSet(left);
		Set::deleteSet(right);
	}
	else if(this->opCode == UNION){
		assert(this->nchild() == 2);
		Set* left = this->values[0]->calculateSetValue(context);
		Set* right = this->values[1]->calculateSetValue(context);
		assert(right!=NULL && left!=NULL);
		rval = new SetSimple(Set::TMP,left->dim);
		right->setUnion(left,rval);
		Set::deleteSet(right);
		Set::deleteSet(left);
	}
	else if (this->opCode == LBRACE) {
		assert(this->nchild() == 1);
		IndexSet* iset = this->createIndexSet(context);
		assert(iset->tuples.size()==1); // TODO: support only 1 dummy index for now.
		rval = new SetSimple(Set::TMP,1);
		rval->copyFromSet(iset->tuples.begin()->set);
		delete iset;
	}
	else if (this->opCode == IDREF) {
		SyntaxNodeIDREF* nodeIdRef = static_cast<SyntaxNodeIDREF*>(this);
		assert(nodeIdRef->ref!=NULL);
		if (nodeIdRef->ref->type == TSET) {
			ModelComp* comp = nodeIdRef->ref;
			rval = static_cast<Set*>(context->getCompValue(comp));
		}
//		else if (nodeIdRef->ref->type == TPARAM) {
//			SingleSymParam* aParam = static_cast<SingleSymParam*>(context->getCompValue(nodeIdRef->ref));
////			assert(aParam->card == 1);
//			rval = new SimpleSet(Set::TMP,1);
//			rval->addSetValue( aParam->value);
//		}
		else {
			LOG("type ["<<nodeIdRef->ref->type<<"not suppose to be here!");
			assert(false);
		}
	}
	else if (this->opCode == DOTDOT) {
		LOG(this->values.size());
		int start;
		int end;
		if (this->values[0]->opCode == VALUE) {
			LOG(this->values[0]->opCode);
			SyntaxNodeValue* valn = static_cast<SyntaxNodeValue*>(this->values[0]);
			start = valn->val;
			LOG("set starting ["<<start<<"]");
		}
		else
		{
			LOG("["<<this->opCode<<"]not yet supported!");
			assert(false);
		}

		if (this->values[1]->opCode == VALUE) {
			LOG(this->values[1]->opCode);
			SyntaxNodeValue* valn = static_cast<SyntaxNodeValue*>(this->values[1]);
			end = valn->val;
			LOG("set ending ["<<end<<"]");
		}
		else if (this->values[1]->opCode == IDREF) {
			SyntaxNodeIDREF* refn = static_cast<SyntaxNodeIDREF*>(this->values[1]);
			assert(refn->ref->type == TPARAM);
			ParamSingle* aParam = static_cast<ParamSingle*>(context->getCompValue(refn->ref));
			assert(aParam->card == 1);
			PValue* pval = aParam->value;
			assert(typeid(*pval)==typeid(PValueValue));
			end = static_cast<PValueValue*>(pval)->value;
			LOG("set end ["<<end<<"]");
		}
		else {
			LOG("case not yet implmented.");
			assert(false);
		}

		rval = new SetSimple(Set::TMP,1);
		for(int i = start;i <= end;i++) {
			ostringstream oss;
			oss << i;
			rval->addSetValue(oss.str());
		}
	}
	else if (this->opCode == SETOF){
		IndexSet* iset = this->values[0]->createIndexSet(context);
		assert(iset->tuples.size()==1); //setof indexset only has one dimensional
		iset_tuple& tuple = *(iset->tuples.begin());
		string dummy = tuple.dummyVar;
		Set* set = tuple.set;
		if(typeid(*(this->values[1])) == typeid(SyntaxNodeID))
		{
			SyntaxNodeID* idn = static_cast<SyntaxNodeID*>(this->values[1]);
			string id = idn->id;
			assert(id.compare(dummy)==0);
			rval = new SetSimple(Set::TMP,1);
			rval->copyFromSet(set);
		}
		else
		{
			assert(typeid(*this->values[1])==typeid(SyntaxNodeIDREF));
			rval = new SetSimple(Set::TMP,1);
			SyntaxNodeIDREF* refn = static_cast<SyntaxNodeIDREF*>(this->values[1]);
			assert(refn->ref->type == TPARAM);
			ParamMult* param = static_cast<ParamMult*>(context->getCompValue(refn->ref));
			assert(refn->nchild()==2); // ie. IDREF identifier expr_list
			assert(refn->values[1]->opCode == COMMA);
			assert(refn->values[1]->nchild()== 1); //TODO only support one dummy var
			SyntaxNodeID* idn = static_cast<SyntaxNodeID*>(refn->values[1]->values[0]);

			assert(dummy.compare(idn->id)==0);  // setof { g: ... } Location[g] -- test g=g
			vector<string>::iterator it = set->setValues_data_order.begin();
			for(;it!=set->setValues_data_order.end();it++)
			{
				PValue* pval = param->findParamValue(*it);
				assert(typeid(*pval)==typeid(PValueSym));
				rval->addSetValue(static_cast<PValueSym*>(pval)->value);
			}
		}
		delete iset;

	}
	else {
		LOG("calculateSetValue -- op not yet implmented -- opCode["<<this->opCode<<"]");
		assert(false);
	}
	return rval;
}

IndexSet* SyntaxNode::createIndexSet(ModelContext* context)
{
	LOG("createIndexSet - "<<this->print());
	assert(this->opCode == LBRACE); //indexing set { ..... }
	assert(this->nchild() == 1 && this->values[0]->opCode == COLON);
	IndexSet* iset = new IndexSet(IndexSet::TMP);
	SyntaxNode* indexing_set = this->values[0];
	SyntaxNode* setexpr_list = indexing_set->values[0];
	SyntaxNode* cond = indexing_set->nchild()==2?indexing_set->values[1]:NULL;

	assert(setexpr_list->opCode == COMMA);
	if(cond == NULL)
	{
		for(SyntaxNode::iterator i = setexpr_list->begin();i!=setexpr_list->end();i++)
		{
			SyntaxNode* setexpr = (*i);
			LOG("for setexpr -- "<<setexpr->print());
			string dummy = "";
			ModelComp* comp = NULL;
			Set* aSet = NULL;
			if(setexpr->opCode == IN)
			{
				SyntaxNode* dummy_list = setexpr->values[0];
				assert(dummy_list->opCode == COMMA && dummy_list->nchild()==1); //TODO support 1 dummy var only!
				dummy = static_cast<SyntaxNodeID*>(dummy_list->values[0])->id;
				comp = static_cast<SyntaxNodeIDREF*>(setexpr->values[1])->ref;
				aSet = static_cast<Set*>(context->getCompValue(comp));
				iset->addSet(dummy,aSet,static_cast<SetComp*>(comp));
			}
			else if(setexpr->opCode == IDREF) //ie. {SET1, SET2...}
			{ //no dummy variable list
				dummy = DummyVarGen::nextDummy();
				comp = static_cast<SyntaxNodeIDREF*>(setexpr)->ref;
				assert(comp->type == TSET);
				aSet = static_cast<Set*>(context->getCompValue(comp));
				iset->addSet(dummy,aSet,static_cast<SetComp*>(comp));
			}
			else if(setexpr->opCode == ID)
			{
				assert(setexpr_list->nchild()==1); //ie. supports only {a} , a is a dummy variable
				dummy = static_cast<SyntaxNodeID*>(setexpr)->id;
				aSet = new SetSimple(Set::TMP,1);
				string val = context->getDummyValue(dummy);
				ModelComp* comp = context->getDummyComp(dummy);
				aSet->addSetValue(val);
				iset->addSet(dummy,aSet,static_cast<SetComp*>(comp));
			}
			else
			{
				LOG("createIndexSet -- opCode["<<setexpr->opCode<<"] not yet implemented!");
 				assert(false); //not yet implmeneted!
			}
		}
	}
	else
	{//if there is condition for this dummy index set -- most like to be a *full* conditioned index set, ie. {i in SET: condition}
		assert(setexpr_list->nchild() == 1); //TODO: support only 1 dummy index for now !
		SyntaxNode* setexpr = setexpr_list->values[0];
		assert(setexpr->opCode == IN); //has to be "{i in SET: condition} ,because condition is not null
		SyntaxNode* dummy_list = setexpr->values[0];
		assert(dummy_list->opCode == COMMA && dummy_list->nchild() == 1); //TODO support 1 dummy var only!
		string dummy = static_cast<SyntaxNodeID*>(dummy_list->values[0])->id;
		ModelComp* comp = static_cast<SyntaxNodeIDREF*>(setexpr->values[1])->ref;
		Set* aSet = static_cast<Set*>(context->getCompValue(comp));
		Set* set = new SetSimple(Set::TMP,1); //TODO support fixed dim = 1 for now only.
		iset->name = IndexSet::NEWSET; //the index set involved a new set created
		BOOST_FOREACH(string& val, aSet->setValues_data_order)
		{
			context->addDummyCompValueMapTemp(dummy,comp,val);
			if(cond->evalBool(context))
			{
				set->addSetValue(val);
			}
			context->removeDummySetValueMapTemp(dummy);
		}
		iset->addSet(dummy,set,static_cast<SetComp*>(comp));
	}
	LOG("createIndexSet -- return iset  -- ["<<iset->toString()<<"]");
	return iset;
}

/*
 * Precondition: *rval == NULL.
 * Postcondition: *rval point to memory on heap, therefore caller need to call delete explicitly after
 * 				assigning it's value
 */
void SyntaxNode::evalTerm(ModelContext* context, PValue** rval) {
	LOG("evalTerm  ---  opCode["<<this->opCode<<"]  "<<this->print());
	assert(*rval == NULL);
	if (this->opCode == VALUE) {
		SyntaxNodeValue* vnode = static_cast<SyntaxNodeValue*>(this);
		*rval = new PValueValue(vnode->val);
	}
	else if (this->opCode == IF) {
		bool cond = this->values[0]->evalBool(context);
		cond == true ? this->values[1]->evalTerm(context,rval) : this->values[2]->evalTerm(context,rval);
	}
	else if (this->opCode == IDREF) {
		SyntaxNodeIDREF* refn = static_cast<SyntaxNodeIDREF*>(this);
		assert(refn->ref->type == TPARAM);
		CompDescr* compvalue = context->getCompValue(refn->ref);
		if(typeid(*compvalue)==typeid(ParamSingle))
		{
			ParamSingle* param = static_cast<ParamSingle*>(compvalue);
			*rval = param->value->clone();
		}
		else if(typeid(*compvalue)==typeid(ParamMult))
		{
			string key;
			assert(this->nchild()==2);
			this->values[1]->getIndiciesKey(context,key);
			ParamMult* theParam = static_cast<ParamMult*>(compvalue);
			*rval=theParam->findParamValue(key)->clone();
		}
		else
		{
			LOG("typeid(ParamSingle) -- "<<typeid(ParamSingle).name());
			LOG("typeid(ParamMult) -- "<<typeid(ParamMult).name());
			LOG("typeid(*compvalue) -- "<<typeid(*compvalue).name());
			assert(false);//error in model - other type not allowed
		}
	}
	else if (this->opCode == ORD) {
		assert(this->nchild()==1);
		assert(this->values[0]->opCode == ID);
		SyntaxNodeID* idn = static_cast<SyntaxNodeID*>(this->values[0]);
		string dummy = idn->id;
		string dummyval = context->getDummyValue(dummy);
		ModelComp* comp = context->getDummyComp(dummy);
		SetOrdered* set = static_cast<SetOrdered*>(context->getCompValue(comp));
		*rval = new PValueValue((double) (set->setOrder(dummyval)));
	}
	else if(this->opCode == ID)
	{
		SyntaxNodeID* idn = static_cast<SyntaxNodeID*>(this);
		string val = context->getDummyValue(idn->id);
		*rval = new PValueSym(val);
	}
	else if(this->opCode == STRING)
	{
		string val = (static_cast<SyntaxNodeString*>(this))->val;
		*rval = new PValueSym(val);
	}
	else if (this->opCode == SUM) {
		IndexSet* iset = this->values[0]->createIndexSet(context);
		assert(iset->tuples.size()==1); //TODO: support only one index set for sum expression
		iset_tuple& tuple = *(iset->tuples.begin());
		string dummy = tuple.dummyVar;
		Set* aSet =  tuple.set;
		SetComp* comp = tuple.setcomp;

		vector<string>::iterator it = aSet->setValues_data_order.begin();
		PValue* sum = new PValueValue((double)0);
		for(;it != aSet->setValues_data_order.end();it++) {
			string value = (*it);
			context->addDummyCompValueMapTemp(dummy, comp, value);
			PValue* tmp = NULL;
			this->values[1]->evalTerm(context,&tmp);
			sum->accumulate(tmp);
			context->removeDummySetValueMapTemp(dummy);
			delete tmp;
		}
		*rval = sum;
	}
	else if (this->opCode == CARD) {
		assert(typeid(*rval)==typeid(PValueValue));
		assert(this->values.size() == 1);
		assert(this->values[0]->values.size() == 1);
		assert(this->values[0]->values[0]->values[0]->values[0]->opCode == IDREF);
		SyntaxNodeIDREF* refNode = static_cast<SyntaxNodeIDREF*>(this->values[0]->values[0]->values[0]->values[0]);
		SetSimple* aSet = static_cast<SetSimple*>(context->getCompValue(refNode->ref));
		*rval = new PValueValue((double) aSet->card);
	}
	else if(this->opCode == PLUS)
	{
		PValue* left =NULL;
		PValue* right=NULL;
		this->values[0]->evalTerm(context,&left);
		this->values[1]->evalTerm(context,&right);
		*rval = left->plus(right);
		delete left;
		delete right;
	}
	else if(this->opCode == TIMES)
	{
		PValue* left =NULL;
		PValue* right=NULL;
		this->values[0]->evalTerm(context,&left);
		this->values[1]->evalTerm(context,&right);
		*rval = left->times(right);
		delete left;
		delete right;
	}
	else if (this->opCode == DIVID) //DIVID operator
	{
		PValue* numerator=NULL;
		PValue* denominator=NULL;
		this->values[0]->evalTerm(context,&numerator);
		this->values[1]->evalTerm(context,&denominator);
		*rval = numerator->divid(denominator);
		delete numerator;
		delete denominator;
	}
	else if (this->opCode == MINUS) {
		if (this->values.size() == 1) {
			PValue* left = NULL;
			this->values[0]->evalTerm(context,&left);
			*rval = left->neg();
		}
		else if (this->values.size() == 2) {
			PValue* left = NULL;
			PValue* right = NULL;
			this->values[0]->evalTerm(context,&left);
			this->values[1]->evalTerm(context,&right);
			*rval = left->minus(right);
			delete left;
			delete right;
		}
		else {
			LOG("can't have more than two for MINUS operator");
			assert(false);
		}
	}
	else if(this->opCode == POWER)
	{
		PValue* left = NULL;
		PValue* right = NULL;
		this->values[0]->evalTerm(context,&left);
		this->values[1]->evalTerm(context,&right);
		*rval = left->power(right);
		delete left;
		delete right;
	}
	else {
		LOG("evalTerm ---  opCode["<<this->opCode<<"] not yet implemented");
		assert(false);
	}
	LOG("evalTerm  ---  opCode["<<this->opCode<<"]  "<<this->print() <<" --- return "<<(*rval)->toString());
	assert(*rval != NULL);
}

bool SyntaxNode::evalBool(ModelContext* context) {
//	LOG("evalBool - single - opCode["<<this->opCode<<"] "<<this->print());
	bool rval = false;
	if(this->opCode == GT)
	{
		PValue* left = NULL;
		PValue* right = NULL;
		this->values[0]->evalTerm(context,&left);
		this->values[1]->evalTerm(context,&right);
		rval = left->isGT(right);
		delete left;
		delete right;
	}
	else if (this->opCode == EQ) {
		PValue* r0=NULL;
		PValue* r1=NULL;
		this->values[0]->evalTerm(context,&r0);
		this->values[1]->evalTerm(context,&r1);
		LOG("--- value for r0["<<r0<<"] r1["<<r1<<"]");
		rval = r0->isEQ(r1);
		delete r0;
		delete r1;
	}
	else if (this->opCode == NE) {
		PValue* r0 = NULL;
		PValue* r1 = NULL;
		this->values[0]->evalTerm(context,&r0);
		this->values[1]->evalTerm(context,&r1);
		LOG("--- value for r0["<<r0<<"] r1["<<r1<<"]");
		rval = r0->isNE(r1);
		delete r0;
		delete r1;
	}
	else if (this->opCode == IN) {
		PValue* pval = NULL;
		this->values[0]->evalTerm(context,&pval);
		LOG("--- value for "<<this->values[0]<<"["<<pval->toString()<<"]");
		assert(typeid(*pval)==typeid(PValueSym));
		string val = static_cast<PValueSym*>(pval)->value;
		delete pval;
		SyntaxNodeIDREF* refn = static_cast<SyntaxNodeIDREF*>(this->values[1]);
		assert(refn->ref->type == TSET);
		SetSimple* aSet = static_cast<SetSimple*>(context->getCompValue(refn->ref));
		vector<string>::iterator it = aSet->setValues_data_order.begin();
		for(;it != aSet->setValues_data_order.end();it++) {
//			assert((*it)->valueList.size() == 1);
			if (val.compare(*it) == 0) {
				rval = true;
				break;
			}
		}
	}
	else if (this->opCode == AND) {
		bool r0 = false;
		bool r1 = false;
		r0 = this->values[0]->evalBool(context);
		LOG("-- value for r0["<<r0<<"]");
		if (r0) {
			r1 = this->values[1]->evalBool(context);
			LOG("-- value for r1["<<r1<<"]");
		}
		rval = r0 && r1;
	}
	else {
		LOG("opCode["<<this->opCode<<"] not yet implemented!");
		assert(false);
	}
	LOG("evalBool - single - rval["<<rval<<"] ");
	return rval;
}


SyntaxNode* SyntaxNode::findDirectChild( int op) {
	SyntaxNode* ret = NULL;
	for(SyntaxNode::iterator i=this->begin();i!=end();i++)
	{
		if(op == (*i)->opCode)
		{
			ret = (*i); break;
		}
	}
	return ret;
}

bool SyntaxNode::isContainsIDREF_TVAR_in_child()
{
	bool rval = false;
	for(SyntaxNode::iterator i = this->begin();i!=end();i++){
		if(IDREF == (*i)->opCode && TVAR == static_cast<SyntaxNodeIDREF*>(*i)->ref->type)
		{
			rval = true;
			break;
		}
		rval = (*i)->isContainsIDREF_TVAR_in_child();
		if(rval == true) break;
	}
	return rval;
}

void SyntaxNode::calcStageSet(ModelContext* ctx, boost::unordered_set<string>* stageset)
{
	assert(this->opCode == LBRACE && this->nchild()==1 && this->values[0]->opCode == COLON);
	assert(this->values[0]->values[0]->nchild()==1);//only 1 setexpr in setexpr_list
	SyntaxNode* set_expr = this->values[0]->values[0]->values[0];
	if(set_expr->opCode == VALUE)
	{
		LOG("calcStageSet -- VALUE -- "<<set_expr->print());
		ostringstream oss;
		oss << (int)(static_cast<SyntaxNodeValue*>(set_expr)->val);
		stageset->insert(oss.str());
	}
	else if(set_expr->opCode == IDREF)
	{
		SyntaxNodeIDREF* refn = static_cast<SyntaxNodeIDREF*>(set_expr);
		assert(refn->ref->type == TPARAM);
		ParamSingle* aParam = static_cast<ParamSingle*>(ctx->getCompValue(refn->ref));
		assert(aParam->card == 1);
		PValue* pval = aParam->value;
		assert(typeid(*pval)==typeid(PValueValue));
		ostringstream oss; oss<<(int)(static_cast<PValueValue*>(pval)->value);
		stageset->insert(oss.str());
	}
	else if(set_expr->opCode == DOTDOT)
	{
		LOG("calcStageSet -- DOTDOT -- "<<set_expr->print());
		int start;
		int end;
		if (set_expr->values[0]->opCode == VALUE) {
			SyntaxNodeValue* valn = static_cast<SyntaxNodeValue*>(set_expr->values[0]);
			start = valn->val;
			LOG("set starting ["<<start<<"]");
		}
		else
		{
			LOG("["<<set_expr->opCode<<"]not yet supported!");
			assert(false);
		}

		if (set_expr->values[1]->opCode == VALUE) {
			LOG(set_expr->values[1]->opCode);
			SyntaxNodeValue* valn = static_cast<SyntaxNodeValue*>(set_expr->values[1]);
			end = valn->val;
			LOG("set ending ["<<end<<"]");
		}
		else if (set_expr->values[1]->opCode == IDREF) {
			SyntaxNodeIDREF* refn = static_cast<SyntaxNodeIDREF*>(set_expr->values[1]);
			assert(refn->ref->type == TPARAM);
			ParamSingle* aParam = static_cast<ParamSingle*>(ctx->getCompValue(refn->ref));
			assert(aParam->card == 1);
			PValue* pval = aParam->value;
			assert(typeid(*pval)==typeid(PValueValue));
			end = static_cast<PValueValue*>(pval)->value;
			LOG("set end ["<<end<<"]");
		}
		else {
			LOG("case not yet implmented.");
			assert(false);
		}

		for(int i = start;i <= end;i++) {
			ostringstream oss;
			oss << i;
			stageset->insert(oss.str());
		}
	}
	else
	{
		LOG("calcStageSet-- opCode["<<opCode<<"] not yet implementated!");
		assert(false);
	}
}

void SyntaxNode::getIndiciesKey(ModelContext* ctx, string& idxkey)
{//this should be an expr_list
	assert(this->opCode == COMMA);
	assert(this->nchild()>=1);
	ostringstream oss;
	SyntaxNode::iterator i = this->begin();
	for(;i!=this->end();i++){
		PValue* pval = NULL;
		(*i)->evalTerm(ctx,&pval);
		assert(typeid(*pval)==typeid(PValueSym));
		oss<<static_cast<PValueSym*>(pval)->value;
	}
	idxkey = oss.str();
}

bool SyntaxNode::hasExp()
{
	bool rval = false;
	if(this->opCode == EXPECTATION)
	{
		rval = true;
	}
	else
	{
		for(SyntaxNode::iterator i=this->begin();i!=this->end();i++)
		{
			rval = (*i)->hasExp();
			if(rval == true)	break;
		}
	}
	return rval;
}

/*
 * this method to perform the move up operation for expecation node
 */
SyntaxNode* SyntaxNode::appendDOTNotation(StochCtx* sctx)
{
	for(uint i=0;i<values.size();i++)
	{
		SyntaxNode* rval = values[i]->appendDOTNotation(sctx);
		if(rval!=values[i]) values[i] = rval;
	}
	return this;
}

double SyntaxNode::evalRhs(ModelContext* context) {
	PValue* rval = NULL;
	if (this->opCode == ASSIGN) {
		this->values[1]->evalTerm(context,&rval); //ingore right hand side
	}
	else {
		LOG("Right-hand-side expression not yet supported! "<<this->print());
		assert(false);
	}

	return static_cast<PValueValue*>(rval)->value;
}


//End Feng



/* --------------------------------------------------------------------------
 SyntaxNode::print()
 --------------------------------------------------------------------------- */
/** Recursively prints the expression rooted at the current node in the
 *  expression tree.
 *
 *  @note use_global_names influences how nodes of type IDREF are printed.
 */
ostream& operator<<(ostream&s, SyntaxNode *node) {
	if (node == NULL)
		return s;
	return node->put(s);
}

ostream& operator<<(ostream&s, SyntaxNode &node) {
	if (&node == NULL)
		return s;
	return node.put(s);
}

void SyntaxNode::calculatePartialConstraints(boost::unordered_map<int,SyntaxNode*>& partials)
{
	assert(partials.size()==0);
	if(this->opCode == IDREF)
	{//leaf node
		SyntaxNodeIDREF* refn = static_cast<SyntaxNodeIDREF*>(this);
		ModelComp* comp = refn->ref;
		assert(comp->type == TVAR || comp->type == TPARAM);
		if(comp->type==TVAR)
		{
			int level = comp->model->level;
			partials.insert(pair<int,SyntaxNode*>(level,this));
		}
		else if(comp->type==TPARAM)
		{
			int level  = -1;
			partials.insert(pair<int,SyntaxNode*>(level,this));
		}
	}
	else if(this->opCode == DOT)
	{//leaf node IDREFM - can be of form : A[i].B[j].var[index] , where A, B are IDREFM and var is IDREF for var or param
	 // we have an explict reference to IDREF in an AmplModel
		assert(this->values[0]->opCode == DOT || this->values[0]->opCode == IDREFM);
		assert(this->values[1]->opCode == IDREF);
		SyntaxNodeIDREF* refn = static_cast<SyntaxNodeIDREF*>(this->values[1]);
		partials.insert(pair<int,SyntaxNode*>(refn->ref->model->level, this));
	}
	else if (this->opCode == VALUE)
	{
		int level  = -1;
		partials.insert(pair<int,SyntaxNode*>(level,this));
	}
	else if(this->opCode == SUM)
	{
		assert(this->nchild()==2);
		SyntaxNode* indx = this->values.at(0);
		boost::unordered_map<int,SyntaxNode*> child;
		this->values.at(1)->calculatePartialConstraints(child);
		boost::unordered_map<int,SyntaxNode*>::iterator it=child.begin();
		for(;it!=child.end();it++)
		{
			SyntaxNode* newSum = new SyntaxNode(SUM,indx,(*it).second);
			partials.insert(pair<int,SyntaxNode*>((*it).first,newSum));
		}
	}
	else if(this->opCode == SUMEXP)
	{ // SumExp{nProbs} (expr) ---->  SumExp{nProbs}(expr1) + ... SumExp{nProbs}(exprn); given expr = expr1 + ... + exprn
		assert(nchild()==1);
		boost::unordered_map<int,SyntaxNode*> child;
		this->values[0]->calculatePartialConstraints(child);
		int nProbs = static_cast<SyntaxNodeSumExp*>(this)->nProbs;
		boost::unordered_map<int,SyntaxNode*>::iterator it=child.begin();
		for(;it!=child.end();it++)
		{
			SyntaxNode* newSumExp = new SyntaxNodeSumExp((*it).second,nProbs);
			partials.insert(pair<int,SyntaxNode*>((*it).first,newSumExp));
		}
	}
	else if(this->opCode == LSBRACKET)
	{
		assert(this->values[0]->opCode == IDREF);
		this->values[0]->calculatePartialConstraints(partials);
	}
	else
	{
		if(this->opCode == PLUS || this->opCode == MINUS )
		{//binary operators - additively separable
			if(nchild()==2) {
				boost::unordered_map<int,SyntaxNode*> child_left;
				boost::unordered_map<int,SyntaxNode*> child_right;
				this->values.at(0)->calculatePartialConstraints(child_left);
				this->values.at(1)->calculatePartialConstraints(child_right);
				assert(child_left.size()>=1);
				assert(child_right.size()>=1);
				for(int i=-1;i<=AmplModel::MAX_LEVEL ; i++)
				{
					boost::unordered_map<int,SyntaxNode*>::iterator it_left = child_left.find(i);
					boost::unordered_map<int,SyntaxNode*>::iterator it_right = child_right.find(i);
					if(it_left == child_left.end() && it_right != child_right.end())
					{//(null) -/+ right
						SyntaxNode* newNode = opCode == MINUS? new SyntaxNodeOP(opCode,(*it_right).second): it_right->second;
						partials.insert(pair<int,SyntaxNode*>(i, newNode));
					}
					else if(it_right==child_right.end() && it_left != child_left.end())
					{//left -/ (null)
						partials.insert(pair<int,SyntaxNode*>(i, (*it_left).second));
					}
					else if(it_right!=child_right.end() && it_left!=child_left.end())
					{//left -/+ right
						SyntaxNode* newNode = new SyntaxNodeOP(opCode,(*it_left).second,(*it_right).second);
						partials.insert(pair<int,SyntaxNode*>(i, newNode));
					}
					else
					{
						//not possible
					}

				}
			}
			else if(nchild()==1)
			{//negative op
				assert(this->opCode == MINUS);
				boost::unordered_map<int,SyntaxNode*> conspart;
				this->values.at(0)->calculatePartialConstraints(conspart);
				assert(partials.size()>=1);
				for(int i=-1;i<=AmplModel::MAX_LEVEL ; i++)
				{
					boost::unordered_map<int,SyntaxNode*>::iterator it = conspart.find(i);
					if(it != conspart.end())
					{
						SyntaxNode* newNode  = new SyntaxNodeOP(MINUS,(*it).second);
						partials.insert(pair<int,SyntaxNode*>(i, newNode));
					}
				}
			}
			else
			{
				assert(false); // not possible
			}
		}
		else if(this->opCode == POWER){
			assert(nchild()==2);
			boost::unordered_map<int,SyntaxNode*> left;
			boost::unordered_map<int,SyntaxNode*> right;
			boost::unordered_map<int,SyntaxNode*>::iterator li;
			boost::unordered_map<int,SyntaxNode*>::iterator ri;
			this->values.at(0)->calculatePartialConstraints(left);
			this->values.at(1)->calculatePartialConstraints(right);
			li = left.find(-1); ri = right.find(-1);
			if((left.size()==1 && li!=left.end()) && (right.size()==1 && ri!=right.end()))
			{// const ^ const
				partials.insert(pair<int,SyntaxNode*>(-1,new SyntaxNodeOP(POWER,li->second,ri->second)));
			}
			else if(left.size()==1 && li!=left.end())
			{//const ^ all other
				SyntaxNode* rnode = NULL;
				for(ri=right.begin();ri!=right.end();ri++){
					rnode = rnode==NULL?ri->second:new SyntaxNodeOP(PLUS,rnode,ri->second);
				}
				assert(rnode!=NULL);
				SyntaxNodeOP* newnode = new SyntaxNodeOP(POWER,li->second,rnode);
				for(ri=right.begin();ri!=right.end();ri++){
					partials.insert(pair<int,SyntaxNode*>(ri->first,newnode));
				}
			}
			else if(right.size()==1 && ri!=right.end())
			{//all other ^ const
				SyntaxNode* lnode = NULL;
				for(li=left.begin();li!=left.end();li++){
					lnode = lnode==NULL?li->second:new SyntaxNodeOP(PLUS,lnode,li->second);
				}
				assert(lnode!=NULL);
				SyntaxNodeOP* newnode= new SyntaxNodeOP(POWER,lnode,ri->second);
				for(li=left.begin();li!=left.end();li++){
					partials.insert(pair<int,SyntaxNode*>(li->first,newnode));
				}
			}
			else
			{
				SyntaxNode* rnode = NULL;
				for(ri=right.begin();ri!=right.end();ri++){
					rnode = rnode==NULL?ri->second:new SyntaxNodeOP(PLUS,rnode,ri->second);
				}
				SyntaxNode* lnode = NULL;
				for(li=left.begin();li!=left.end();li++){
					lnode = lnode==NULL?li->second:new SyntaxNodeOP(PLUS,lnode,li->second);
				}
				SyntaxNode* newnode = new SyntaxNodeOP(POWER,lnode,rnode);
				for(ri=right.begin();ri!=right.end();ri++)
				{
					if(partials.find(ri->first)==partials.end()) partials.insert(pair<int,SyntaxNode*>(ri->first,newnode));
				}
				for(li=left.begin();li!=left.end();li++)
				{
					if(partials.find(li->first)==partials.end()) partials.insert(pair<int,SyntaxNode*>(li->first,newnode));
				}
			}
		}
		else if(this->opCode == DIVID)
		{
			assert(nchild()==2);
			boost::unordered_map<int,SyntaxNode*> left;
			boost::unordered_map<int,SyntaxNode*> right;
			boost::unordered_map<int,SyntaxNode*>::iterator li;
			boost::unordered_map<int,SyntaxNode*>::iterator ri;
			this->values.at(0)->calculatePartialConstraints(left);
			this->values.at(1)->calculatePartialConstraints(right);
			li = left.find(-1); ri = right.find(-1);
			if((left.size()==1 && li!=left.end()) && (right.size()==1 && ri!=right.end()))
			{// const / const
				partials.insert(pair<int,SyntaxNode*>(-1,new SyntaxNodeOP(DIVID,li->second,ri->second)));
			}
			else if(left.size()==1 && li!=left.end())
			{//const / all other
				SyntaxNode* rnode = NULL;
				for(ri=right.begin();ri!=right.end();ri++){
					rnode = rnode==NULL?ri->second:new SyntaxNodeOP(PLUS,rnode,ri->second);
				}
				assert(rnode!=NULL);
				SyntaxNodeOP* newnode = new SyntaxNodeOP(DIVID,li->second,rnode);
				for(ri=right.begin();ri!=right.end();ri++){
					partials.insert(pair<int,SyntaxNode*>(ri->first,newnode));
				}
			}
			else if(right.size()==1 && ri!=right.end())
			{//all other / const
				for(li=left.begin();li!=left.end();li++){
					SyntaxNode* newnode = new SyntaxNodeOP(DIVID,li->second,ri->second);
					partials.insert(pair<int,SyntaxNode*>(li->first,newnode));
				}
			}
			else
			{
				SyntaxNode* rnode = NULL;
				for(ri=right.begin();ri!=right.end();ri++){
					rnode = rnode==NULL?ri->second:new SyntaxNodeOP(PLUS,rnode,ri->second);
				}
				SyntaxNode* lnode = NULL;
				for(li=left.begin();li!=left.end();li++){
					lnode = lnode==NULL?li->second:new SyntaxNodeOP(PLUS,lnode,li->second);
				}
				SyntaxNode* newnode = new SyntaxNodeOP(DIVID,lnode,rnode);
				for(ri=right.begin();ri!=right.end();ri++)
				{
					if(partials.find(ri->first)==partials.end()) partials.insert(pair<int,SyntaxNode*>(ri->first,newnode));
				}
				for(li=left.begin();li!=left.end();li++)
				{
					if(partials.find(li->first)==partials.end()) partials.insert(pair<int,SyntaxNode*>(li->first,newnode));
				}
			}
		}
		else if(this->opCode == TIMES)
		{//binary operator  -- non-separable
			assert(nchild()==2);                                            //level*level
			boost::unordered_map<int,SyntaxNode*> left;                     //    -1   0    1    2
			boost::unordered_map<int,SyntaxNode*> right;                    //-1  -1   0    1    2
			boost::unordered_map<int,SyntaxNode*>::iterator li;				// 0   0   0    0,1  0,2
			boost::unordered_map<int,SyntaxNode*>::iterator ri;				// 1   1   0,1  1    1,2
			this->values.at(0)->calculatePartialConstraints(left);			// 2   2   0,2  1,2  2
			this->values.at(1)->calculatePartialConstraints(right);
			assert(left.size()>=1);
			assert(right.size()>=1);
			//the constant node locate at level=-1

			//for each levels
			for(int i=-1;i<=AmplModel::MAX_LEVEL; i++)
			{
				for(int j=-1;j<=AmplModel::MAX_LEVEL;j++)
				{
					li = left.find(i);
					ri = right.find(j);
					SyntaxNode* ln = li==left.end()?NULL:li->second;
					SyntaxNode* rn = ri==right.end()?NULL:ri->second;
					SyntaxNode* newnode = NULL;
					if(ln!=NULL && rn!=NULL)  newnode = new SyntaxNodeOP(TIMES,ln,rn);
					if(i==j)
					{//put on i/j level
						//create node
						boost::unordered_map<int,SyntaxNode*>::iterator it=partials.find(i);
						if(newnode!=NULL && it!=partials.end()) {
							newnode = new SyntaxNodeOP(PLUS,it->second,newnode);
						}
						if(newnode!=NULL) partials[j] = newnode;
					}
					else if(i==-1 && j!=-1)
					{//put on j level
						boost::unordered_map<int,SyntaxNode*>::iterator it = partials.find(j);
						if(newnode!=NULL && it!=partials.end()) {
							newnode = new SyntaxNodeOP(PLUS,it->second,newnode);
						}
						if(newnode!=NULL) partials[j] = newnode;
					}
					else if(i!=-1 && j==-1)
					{//put on i level
						boost::unordered_map<int,SyntaxNode*>::iterator it = partials.find(i);
						if(newnode!=NULL && it!=partials.end()){
							newnode = new SyntaxNodeOP(PLUS,it->second,newnode);
						}
						if(newnode!=NULL) partials[i] = newnode;
					}
					else
					{//put on both i ,j level
						boost::unordered_map<int,SyntaxNode*>::iterator iti = partials.find(i);
						if(newnode!=NULL && iti!=partials.end()) newnode = new SyntaxNodeOP(PLUS,iti->second,newnode);
						if(newnode!=NULL) partials[i] = newnode;

						boost::unordered_map<int,SyntaxNode*>::iterator itj = partials.find(j);
						if(newnode!=NULL && itj!=partials.end()) newnode = new SyntaxNodeOP(PLUS,itj->second,newnode);
						if(newnode!=NULL) partials[j] = newnode;
					}
				}
			}
		}
		else if(this->opCode == COS || this->opCode == SIN)
		{
			assert(this->nchild()==1);
			boost::unordered_map<int,SyntaxNode*> conspart;
			boost::unordered_map<int,SyntaxNode*>::iterator i;
			this->values.at(0)->calculatePartialConstraints(conspart);

			SyntaxNode* lnode = NULL;
			for(i=conspart.begin();i!=conspart.end();i++){
				lnode = lnode==NULL?i->second:new SyntaxNodeOP(PLUS,lnode,i->second);
			}
			assert(lnode!=NULL);
			SyntaxNodeOP* newnode = new SyntaxNodeOP(opCode,lnode);
			if(conspart.size()==1 && conspart.find(-1)!=conspart.end())
			{//const only cos/sin(const_)
				partials.insert(pair<int,SyntaxNode*>(-1,newnode));
			}
			else
			{
				for(i=conspart.begin();i!=conspart.end();i++){//can't be a const anymore . ie. cos(const+TVAR....) is on TVAR's level
					if(i->first!=-1) partials.insert(pair<int,SyntaxNode*>(i->first,newnode));
				}
			}
		}
		else
		{
			LOG("calculatePartialConstraints --- opCode["<<opCode<<"] not yet implemented!");
			assert(false);
		}
	}
}

/*
 * end of constraints separable calculation
 */


//! locateCtx for DOT expression
//! return the context that corresponding to the lowest level context for this expression
//! In lp problem the return value can be null if the context is not initialized
ModelContext* SyntaxNode::locateCtx(ModelContext* rowctx, ModelContext* currCtx)
{
	if(currCtx == NULL)
	{
		return NULL;
	}

	ModelContext* rval = NULL;
	if(this->opCode == DOT)
	{
		assert(this->values[0]->opCode == IDREFM);
		assert(this->values[1]->opCode == IDREFM);
		rval = this->values[0]->locateCtx(rowctx,currCtx);
		rval = this->values[1]->locateCtx(rowctx,rval);
	}
	else if(this->opCode == IDREFM)
	{
		SyntaxNodeIDREFM* idrefm = static_cast<SyntaxNodeIDREFM*>(this);
		AmplModel* model = idrefm->ref;
		SyntaxNode* expr_list = idrefm->values[1];
		assert(expr_list->opCode == COMMA && expr_list->nchild()==1 ); //TODO: only 1 index supported for now
		string modeldummy = static_cast<SyntaxNodeID*>(expr_list->values[0])->id;
		string modeldummyval = rowctx->getDummyValue(modeldummy);
		//loacate the model context of the model dummy value is equal to modeldummyval.
		// ie. Model{j in Set} . when j = "A" , returns the ctx of Model["A"]
		rval = currCtx->em->locateCtx(model,modeldummyval);
	}
	return rval;
}

//TODO - Deisgn decision
// emcol is need only for sum{j in Set: Condition}(expression) optimization.
// when the sum{j in Set}  x emcol's model index {j in Set}, it assume the expression
// to be summed over Set only evaluate onece for j = emcol's model dummy value.
// To enable this optimization of building AutoDiff node for constraint
//  1. 	The condition expression in the index set expression in summation should be NULL.
//		This is because the indexing expression of a model should not have a condition or
//		is not yet supported.
//	2.	The summation SetComp should be the same as the SetComp in the indexing expression of the emcol's model index.
//	3.	Finally, the string name of dummy variable declared in the sum indexing expression should be the
//		same as the string name of dummy variable used in the emcol's model index expression.
//		This condition is placed intentionally so that the modeller will be able explictly enable this optimization routine.
AutoDiff::Node* SyntaxNode::buildAutoDiffDAG(ExpandedModel* emrow,ExpandedModel* emcol, bool isLP)
{
	LOG("enter - createAutoDiffConsDAG  - emrow["<<emrow->name<<"] emcol["<<(emcol==NULL?"null":emcol->name)<<"] --- "<<this->print());
	ModelContext* rowctx = emrow->ctx;
	AutoDiff::Node* con = NULL;
	if(this->opCode==ASSIGN)
	{
		con = this->values[0]->buildAutoDiffDAG(emrow,emcol, isLP);
	}
	else if(this->opCode == IDREF)
	{	//implicit idref lookup, - will lookup from emrow->ctx and it's parent recursively.
		//for linear constraint, because the constraint attribute is split into partials according to declared level
		//therefore, the idref (TVAR) type only generates a AutoDiff::VNode (adv) by looking up var initialized in emcol->ctx,
		//otherwise indicating an error made in partial constraint computation.
		//for nonlinear constraint, the implicit variable can be anything from rowctx or above.
		if(isLP==true)
		{//IMPLICIT idref look up in LP
			SyntaxNodeIDREF* idref = static_cast<SyntaxNodeIDREF*>(this);
			ModelComp* ref = idref->ref;
			if(ref->type == TVAR )
			{	//varaible must be in found in emcol. use emcolctx to lookup variable
				assert(idref->ref->model == emcol->model); //must be in emcol
				boost::unordered_map<ModelComp*,CompDescr*>::iterator itt = emcol->ctx->compValueMap.find(ref);
				assert(itt!=emcol->ctx->compValueMap.end());
				Var* var = static_cast<Var*>(itt->second);
				assert(var!=NULL);
				string varIndex = "";
				if (idref->nchild() == 2) {//build varIndex if there is one
					idref->values[1]->getIndiciesKey(rowctx, varIndex);
				}
				var_multi_map_by_indicies::iterator it = var->varMultiMap.get<1>().find(varIndex);
				if (it != var->varMultiMap.get<1>().end()) {
					con = (*it)->adv;
				}
				else {
					LOG("can't find AutoDiff var in ["<<var->name<<"] for varIndex["<<varIndex<<"]");
					LOG("please check the model!");
					assert(false);
				}
			}
			else if(ref->type == TPARAM)
			{ 	//a parameter could be declared on it's parent level, therefore recursively lookup from rowctx
				CompDescr* compdescr = rowctx->getCompValue(ref);
				if (typeid(*compdescr) == typeid(ParamMult)) {
					string key = "";
					assert(idref->nchild() == 2);
					idref->values[1]->getIndiciesKey(rowctx, key);
					ParamMult* theParam = static_cast<ParamMult*>(compdescr);
					double pval = static_cast<PValueValue*>(theParam->findParamValue(key))->value;
					con = AutoDiff::create_param_node(pval);
				}
				else {
					assert(typeid(*compdescr) == typeid(ParamSingle));
					double pval = static_cast<PValueValue*>(static_cast<ParamSingle*>(compdescr)->value)->value;
					con = AutoDiff::create_param_node(pval);
				}
			}
			else
			{
				assert(false); //never reach here .
			}
			assert(con!=NULL); //should be initialised always, (parent variable should not be in the partial constarint at emcol->model->level
		}
		else
		{//implicit look up for nonlinear-
		 //nonlinear case variable can be referenced from any level in rowctx or above
			SyntaxNodeIDREF* idref = static_cast<SyntaxNodeIDREF*>(this);
			ModelComp* ref = idref->ref;
			CompDescr* compdescr = rowctx->getCompValue(ref);
			if(ref->type == TVAR)
			{
				Var* var = static_cast<Var*>(compdescr); //variable can be referenced from any level in rowctx or above, therefore, recusively lookup from rowctx
				assert(var!=NULL);
				string varIndex = "";
				if (idref->nchild() == 2) {//build varIndex if there is one
					idref->values[1]->getIndiciesKey(rowctx, varIndex);
				}
				var_multi_map_by_indicies::iterator it = var->varMultiMap.get<1>().find(varIndex);
				if (it != var->varMultiMap.get<1>().end()) {
					con = (*it)->adv;
				}
				else {
					LOG("can't find AutoDiff var in ["<<var->name<<"] for varIndex["<<varIndex<<"]");
					LOG("please check the model!");
					assert(false);
				}
			}
			else if(ref->type == TPARAM)
			{
				if (typeid(*compdescr) == typeid(ParamMult)) {
					string key = "";
					assert(idref->nchild() == 2);
					idref->values[1]->getIndiciesKey(rowctx, key);
					ParamMult* theParam = static_cast<ParamMult*>(compdescr);
					double pval = static_cast<PValueValue*>(theParam->findParamValue(key))->value;
					con = AutoDiff::create_param_node(pval);
				}
				else {
					assert(typeid(*compdescr) == typeid(ParamSingle));
					double pval = static_cast<PValueValue*>(static_cast<ParamSingle*>(compdescr)->value)->value;
					con = AutoDiff::create_param_node(pval);
				}
			}
			assert(con!=NULL); //should be initialised always.
		}
	}
	else if(this->opCode == DOT)
	{//explict model reference look up
//		//loacate the model context of the model dummy value is equal to modeldummyval.
//		// ie. Model{j in Set} . when j = "A" , returns the ctx of Model["A"]
		assert(this->nchild()==2);
		ModelContext* explicitCtx = this->values[0]->locateCtx(rowctx,rowctx);
		assert(this->values[1]->opCode == IDREF);
		SyntaxNodeIDREF* idref = static_cast<SyntaxNodeIDREF*>(this->values[1]);
		ModelComp* ref = idref->ref;
		CompDescr* compdescr = explicitCtx->compValueMap.find(ref)->second;
		if(isLP==true){
			if(ref->type == TVAR)
			{
				assert(explicitCtx==emcol->ctx); //if this refere an variable, the context must be point to emcol->ctx for LP case, using partial attributes
				Var* var = static_cast<Var*>(compdescr);
				assert(var!=NULL);
				string varIndex = "";
				if (idref->nchild() == 2) {
					idref->values[1]->getIndiciesKey(rowctx, varIndex);
				}
				var_multi_map_by_indicies::iterator it = var->varMultiMap.get<1>().find(varIndex);
				if (it != var->varMultiMap.get<1>().end()) {
					con = (*it)->adv;
				}
				else {
					LOG("can't find AutoDiff var in ["<<var->name<<"] for varIndex["<<varIndex<<"]");
					LOG("please check the model!");
					assert(false);
				}
			}
			else
			{
				assert(ref->type == TPARAM);
				assert(false); //TODO: not sure how to due with parameter declared in a lower context.
								//      the context in some case, may not be initialized locally.
			}
		}
		else
		{// for Nonlinear attributes of NLP, the context must be initialised
			assert(explicitCtx != NULL);
			if (ref->type == TVAR) {
				Var* var = static_cast<Var*>(compdescr);
				assert(var!=NULL);
				string varIndex = "";
				if (idref->nchild() == 2) {
					idref->values[1]->getIndiciesKey(rowctx, varIndex);
				}
				var_multi_map_by_indicies::iterator it = var->varMultiMap.get<1>().find(varIndex);
				if (it != var->varMultiMap.get<1>().end()) {
					con = (*it)->adv;
				}
				else {
					LOG("can't find AutoDiff var in ["<<var->name<<"] for varIndex["<<varIndex<<"]");
					LOG("please check the model!");
					assert(false);
				}
			}
			else if(ref->type==TPARAM)
			{
				if (typeid(*compdescr) == typeid(ParamMult)) {
					string key = "";
					assert(idref->nchild() == 2);
					idref->values[1]->getIndiciesKey(rowctx, key);
					ParamMult* theParam = static_cast<ParamMult*>(compdescr);
					double pval = static_cast<PValueValue*>(theParam->findParamValue(key))->value;
					con = AutoDiff::create_param_node(pval);
				}
				else {
					assert(typeid(*compdescr) == typeid(ParamSingle));
					double pval = static_cast<PValueValue*>(static_cast<ParamSingle*>(compdescr)->value)->value;
					con = AutoDiff::create_param_node(pval);
				}
			}
		}
	}
	else if(this->opCode == SUM)
	{
		SyntaxNode* indexing_set = this->values[0];
		assert(indexing_set->opCode == LBRACE);
		SyntaxNode* indexing_set_expr_list = indexing_set->values[0]->values[0];
		SyntaxNode* condition_opt = indexing_set->values[0]->nchild()==2?indexing_set->values[0]->values[1] : NULL;

		assert(indexing_set_expr_list->nchild()==1); //TODO: support 1-index dummy for sum only
		SyntaxNode* set_expr = indexing_set_expr_list->values[0];
		assert(set_expr->opCode == IN);
		assert(set_expr->values[0]->opCode == COMMA);
		assert(set_expr->values[0]->nchild() == 1); //TODO: support 1-dim set ie. not yet support {(i,j) IN SET }

		string dummySum = static_cast<SyntaxNodeID*>(set_expr->values[0]->values[0])->id;
		ModelComp* compSum = static_cast<SyntaxNodeIDREF*>(set_expr->values[1])->ref;
		assert(dummySum.compare("")!=0 && compSum!=NULL);
		ModelComp* compEmcol = NULL;
		string dummyEmcol = "";
		if(emcol!=NULL && emcol->model->indexing!=NULL){ //there is indexingset for emcol's model
			compEmcol = emcol->dummyMap.begin()->second.first;
			dummyEmcol = emcol->dummyMap.begin()->first;
		}

		if(compSum == compEmcol && dummySum.compare(dummyEmcol)==0 && condition_opt == NULL)
		{	//the emcol 's model indexset exact matches the sum indexset- ie. block ID {a in Arcs} {...} v.s. sum{a in Arcs : NULL}(...);
			//this design is to speedup the summation handling for sum constraints applied on serveral sibling variables.
			//note: 1. sibling variables can't be non-additively separable.
			//		2. to enable this optimisation, the model fille has to be in exact form as stated above.
			string& dummyval = emcol->dummyMap.begin()->second.second;
			LOG("Exact matches found for {"<<dummySum<<" in "<<compSum->name<<"} == {"<<dummyEmcol<<" in "<<compEmcol->name<<"} - Sum explict ["<<dummySum<<" -> ["<<dummyval<<"]");
			rowctx->addDummyCompValueMapTemp(dummySum, compSum, dummyval);
			con = this->values[1]->buildAutoDiffDAG(emrow,emcol, isLP);
			rowctx->removeDummySetValueMapTemp(dummySum);
		}
		else
		{
			LOG("All SUM over full set {"<<dummySum<<" in "<<compSum->name<<":"<<condition_opt<<"}");
			compEmcol!=NULL? LOG("Emcol over {"<<dummyEmcol<<" in "<<compEmcol->name<<"}") : LOG("Emcol[null]");

			SyntaxNode* index_set = this->values[0];
			IndexSet* iset = NULL;
			if (!rowctx->getCalcSumSet(index_set, &iset))
			{
				iset = this->values[0]->createIndexSet(rowctx);
				rowctx->addCalcSumSet(index_set, iset);
			}
			assert(iset!=NULL);

			//build auto diff DAG sum expressions
			queue<AutoDiff::Node*> nodes;
			assert(iset->tuples.size() ==  1); //TODO: only for one dummy variables set for sum expression
			iset_tuple& tuple = *(iset->tuples.begin());

			string dummy = tuple.dummyVar;
			Set* set = tuple.set;
			ModelComp* comp = tuple.setcomp;
			vector<string>::iterator setv = set->setValues_data_order.begin();
			for(;setv!=set->setValues_data_order.end();setv++)
			{

				string value = *setv;
				rowctx->addDummyCompValueMapTemp(dummy, comp, value);
				AutoDiff::Node* n = this->values[1]->buildAutoDiffDAG(emrow,emcol, isLP);
				assert(n!=NULL);
				nodes.push(n);
				rowctx->removeDummySetValueMapTemp(dummy);
			}

			if(nodes.size()>=1)
			{	//sum { n1, n2, n3 .... } where n1, n2, n3... is on a queue. construct a summation tree with the queue.
				while(nodes.size()>1)
				{
					AutoDiff::Node* n1 = nodes.front();
					nodes.pop();
					AutoDiff::Node* n2 = nodes.front();
					nodes.pop();
					AutoDiff::Node* n = create_binary_op_node(AutoDiff::OP_PLUS,n1,n2);
					nodes.push(n);
				}
				con = nodes.front();
				assert(nodes.size()==1);
			}
			else
			{	//if the queue is empty which means there is no element constructed over the summation set. Therefore
				//create a 0 node.
				con = AutoDiff::create_param_node(0);
			}
		}
	}
	else if(this->opCode == SUMEXP)
	{
		assert(nchild()==1);
		SyntaxNodeSumExp* sumexpn = static_cast<SyntaxNodeSumExp*>(this);
		if(emcol!=NULL) // using partial attribute, nProbs must equals to emcol's level -1
		{
			assert(sumexpn->nProbs == emcol->model->level-1);
			assert(emrow->model->level == 1); //must be the stage0 node. because constraint that contains exp[expr] will be moved to root stage.
			//adding dummy from emcol's model dummy
			int num = 0;
			assert(emrow->ctx->dummyEmcolTempMap.size()==0);
			for(ExpandedModel* c = emcol;c!=emrow;c=c->parent) {
				assert(c->dummyMap.size()==1); //assume only 1 model dummy support!
				string dv = c->dummyMap.begin()->first;
				string& dval = c->dummyMap.begin()->second.second;
				emrow->ctx->addDummyEmcolTempMap(dv,dval);
				num++;
			}
			assert(num==sumexpn->nProbs); //sanity check!
			//then, build con by using expr (this->values[0]);
			con = this->values[0]->buildAutoDiffDAG(emrow,emcol,isLP);
			//finally, remove the dummy from emcol's model dummy to restore emrow
			for(ExpandedModel* c = emcol;c!=emrow;c=c->parent) {
				assert(c->dummyMap.size()==1); //assume only 1 model dummy support!
				string dv = c->dummyMap.begin()->first;
				emrow->ctx->removeDummyEmcolTempMap(dv);
			}
			assert(emrow->ctx->dummyEmcolTempMap.size()==0);
		}
		else
		{	//using full constraint attributes
			//1. find out the stage X of expr of EXP[expr].
			//2. create a summation express that using each instance in the corresponding contexts from the stage X

			assert(false); //indicating building constraint using Full attributes
			con = NULL;
		}
	}
	else if(this->opCode == VALUE)
	{
		SyntaxNodeValue* valn = static_cast<SyntaxNodeValue*>(this);
		con = AutoDiff::create_param_node(valn->val);
	}
	else if(this->opCode == MINUS)
	{
		if(this->values.size()==1)
		{
			assert(this->values[0]!=NULL);
			AutoDiff::Node* left = this->values[0]->buildAutoDiffDAG(emrow,emcol, isLP);
			con = create_uary_op_node(AutoDiff::OP_NEG,left);
		}
		else
		{
			assert(this->values.size()==2);
			AutoDiff::Node* left = this->values[0]->buildAutoDiffDAG(emrow,emcol, isLP);
			AutoDiff::Node* right = this->values[1]->buildAutoDiffDAG(emrow,emcol, isLP);
			con = create_binary_op_node(AutoDiff::OP_MINUS,left,right);
		}
	}
	else if(this->opCode == PLUS)
	{
		if(this->values.size()==1)
		{
			assert(this->values[0]!=NULL);
			con = this->values[0]->buildAutoDiffDAG(emrow,emcol, isLP);
		}
		else
		{
			assert(this->values.size()==2);
			AutoDiff::Node* left = this->values[0]->buildAutoDiffDAG(emrow,emcol, isLP);
			AutoDiff::Node* right = this->values[1]->buildAutoDiffDAG(emrow,emcol, isLP);
			con = create_binary_op_node(AutoDiff::OP_PLUS,left,right);
		}
	}
	else if(this->opCode == TIMES)
	{
		assert(this->nchild()==2);
		AutoDiff::Node* left = this->values[0]->buildAutoDiffDAG(emrow,emcol, isLP);
		AutoDiff::Node* right = this->values[1]->buildAutoDiffDAG(emrow,emcol, isLP);
		con = create_binary_op_node(AutoDiff::OP_TIMES,left,right);
	}
	else if(this->opCode == DIVID)
	{
		assert(this->nchild()==2);
		AutoDiff::Node* left = this->values[0]->buildAutoDiffDAG(emrow,emcol, isLP);
		AutoDiff::Node* right = this->values[1]->buildAutoDiffDAG(emrow,emcol, isLP);
		con = create_binary_op_node(AutoDiff::OP_DIVID,left,right);
	}
	else if(this->opCode == POWER)
	{
		assert(this->nchild()==2);
		AutoDiff::Node* left = this->values[0]->buildAutoDiffDAG(emrow,emcol, isLP);
		AutoDiff::Node* right = this->values[1]->buildAutoDiffDAG(emrow,emcol, isLP);
		con = AutoDiff::create_binary_op_node(AutoDiff::OP_POW,left,right);
	}
	else if(this->opCode == COS)
	{
		assert(this->nchild()==1);
		AutoDiff::Node* left = this->values[0]->buildAutoDiffDAG(emrow,emcol, isLP);
		con = AutoDiff::create_uary_op_node(AutoDiff::OP_COS,left);
	}
	else if(this->opCode == SIN)
	{
		assert(this->nchild()==1);
		AutoDiff::Node* left = this->values[0]->buildAutoDiffDAG(emrow,emcol, isLP);
		con = AutoDiff::create_uary_op_node(AutoDiff::OP_SIN,left);
	}
	else
	{
		LOG("opCode["<<this->opCode<<"] is not yet implemented! -- "<<this->print());
		assert(false);
	}
	LOG("exit - createAutoDiffConsDAG  - emrow["<<emrow->name<<"] emcol["<<(emcol==NULL?"null":emcol->name)<<"]");
	return con;
}

void SyntaxNode::calculateMemoryUsage(unsigned long& size) {
	size += sizeof(SyntaxNode);
	this->calculateBaseValueVector(size);
}

void SyntaxNode::calculateBaseValueVector(unsigned long& size) {
	for(vector<SyntaxNode*>::iterator it = values.begin();it != values.end();it++) {
		size += sizeof(SyntaxNode*);
		(*it)->calculateMemoryUsage(size);
	}
}


