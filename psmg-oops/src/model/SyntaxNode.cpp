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
#include "SyntaxNode.h"
#include "SyntaxNodeIDREF.h"
#include "SyntaxNodeIDREFM.h"
#include "SyntaxNodeValue.h"
#include "SyntaxNodeID.h"
#include "SyntaxNodeOP.h"
#include "SetComp.h"
#include "AmplModel.h"
#include "ParamComp.h"
#include "../context/SetSimple.h"
#include "../context/SetOrdered.h"
#include "../context/ParamMult.h"
#include "../context/ParamSingle.h"
#include "../context/PValueValue.h"
#include "../context/PValueSym.h"
#include "../context/IndexSet.h"
#include "../context/Var.h"
#include "../context/VarSingle.h"
#include "../util/global_util_functions.h"
#include "../context/Param.h"
#include "../util/DummyVariableGenerator.h"
#include "../context/ExpandedModel.h"
#include "../parser/sml.tab.h"

//#include "../st_model/StochModel.h"

/** Add an item to the back */
using namespace std;

SyntaxNode *SyntaxNode::push_back(SyntaxNode *newitem) {
	values.push_back(newitem);
	return this;
}

/** Add an item to the front */
SyntaxNode *SyntaxNode::push_front(SyntaxNode *newitem) {
	int nval = values.size();
	values.resize(++nval);
	for(int i = nval;--i > 0;)
		values[i] = values[i - 1];
	values[0] = newitem;
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
SyntaxNode::SyntaxNode(const SyntaxNode &src) :opCode(src.opCode), values(src.values) {
	LOG("Creating syntaxNode opCode["<<src.opCode<<"]");
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
			s << **(++i);
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
			s<<**(++i);
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
	assert(iset->dummyCompMap.size()== 1); //only support one dummy index for now
	card = iset->dummySetMap.begin()->second->card;
	delete iset;
}
void SyntaxNode::calculateVarDimCard(ModelContext* ctx, uint& dim, uint& card) {
	LOG(this->print());
	assert(this->opCode == LBRACE);
	assert(this->values[0]->opCode == COLON);
	SyntaxNode* setexpr_list = this->values[0]->values[0];
	assert(setexpr_list->nchild()==1); //only support one dummy index
	IndexSet* iset = this->createIndexSet(ctx);
	assert(iset->dummyCompMap.size()== 1); //only support one dummy index for now
	dim = iset->dummyCompMap.size();
	card = iset->dummySetMap.begin()->second->card;
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
			assert(nchild()==2);
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
			string dummy = DummyVariableGenerator::nextDummy();
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
		assert(iset->dummySetMap.size()==1); // support only 1 dummy index for now.
		rval = new SetSimple(Set::TMP,1);
		rval->copyFromSet(iset->dummySetMap.begin()->second);
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
			rval->addSetValue(oss);
		}
	}
	else if (this->opCode == SETOF){
		IndexSet* iset = this->values[0]->createIndexSet(context);
		assert(iset->dummySetMap.size()==1); //setof indexset only has one dimensional
		string dummy = (iset->dummySetMap.begin())->first;
		Set* set = (iset->dummySetMap.begin())->second;
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
	assert(this->nchild() == 1);
	IndexSet* iset = new IndexSet(IndexSet::TMP);
	assert(this->values[0]->opCode == COLON);
	SyntaxNode* setexpr_list = this->values[0]->values[0];
	SyntaxNode* cond = this->values[0]->nchild()==2?this->values[0]->values[1]:NULL;

	assert(setexpr_list->opCode == COMMA);
	assert(setexpr_list->nchild() == 1); //TODO: support 1 dummy index for now only!

	if(setexpr_list->values[0]->opCode == IN){
		assert(setexpr_list->values[0]->values[0]->opCode == COMMA);
		assert(setexpr_list->values[0]->values[0]->nchild()==1); //TODO: support 1 dummy var only for now . ie. not allow (i,j) IN SET

		string dummy = static_cast<SyntaxNodeID*>(setexpr_list->values[0]->values[0]->values[0])->id;
		ModelComp* comp = static_cast<SyntaxNodeIDREF*>(setexpr_list->values[0]->values[1])->ref;
		assert(comp->type == TSET);
		Set* aSet = static_cast<Set*>(context->getCompValue(comp));
		Set* set = NULL;
		if(cond!=NULL)
		{
			set= new SetSimple(Set::TMP,1);
			vector<string>::iterator it = aSet->setValues_data_order.begin();
			for(;it != aSet->setValues_data_order.end();it++) {
				string val = (*it);
				LOG("dummy val["<<val<<"]");
				context->addDummyCompValueMapTemp(dummy, comp, val);
				if (cond->evalBool(context)) {
					set->addSetValue(val);
				}
				context->removeDummySetValueMapTemp(dummy);
			}
		}
		else
		{
			set = aSet;
		}
		iset->addSet(dummy,set,static_cast<SetComp*>(comp));
	}
	else if(setexpr_list->values[0]->opCode == IDREF)
	{
		string dummy = DummyVariableGenerator::nextDummy();
		ModelComp* comp = static_cast<SyntaxNodeIDREF*>(setexpr_list->values[0])->ref;
		assert(comp->type == TSET);
		Set* set = static_cast<Set*>(context->getCompValue(comp));
		iset->addSet(dummy,set,static_cast<SetComp*>(comp));
	}
	else
	{
		assert(setexpr_list->values[0]->opCode == ID);
		SyntaxNodeID* idn = static_cast<SyntaxNodeID*>(setexpr_list->values[0]);
		Set* set = new SetSimple(Set::TMP,1);
		ostringstream oss(ostringstream::out);
		string val = context->getDummyValue(idn->id);
		ModelComp* comp = context->getDummyComp(idn->id);
		set->addSetValue(val);
		iset->addSet(idn->id,set,static_cast<SetComp*>(comp));
	}
	LOG("createIndexSet -- return iset  -- ["<<iset->toString()<<"]");
	return iset;
}

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
	else if (this->opCode == SUM) {
		assert(this->values[0]->values[0]->values[0]->opCode == ID);
		assert(this->values[0]->values[0]->values[1]->opCode == IDREF);
		SyntaxNodeID* idNode = static_cast<SyntaxNodeID*>(this->values[0]->values[0]->values[0]);
		SyntaxNodeIDREF* refNode = static_cast<SyntaxNodeIDREF*>(this->values[0]->values[0]->values[1]);
		string dummy = idNode->id;
		SetSimple* aSet = static_cast<SetSimple*>(context->getCompValue(refNode->ref));
		vector<string>::iterator it = aSet->setValues_data_order.begin();

		PValue* sum = new PValueValue((double)0);
		for(;it != aSet->setValues_data_order.end();it++) {
			string value = (*it);
			context->addDummyCompValueMapTemp(dummy, refNode->ref, value);
			PValue* tmp = new PValueValue();
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


SyntaxNode* SyntaxNode::findChildNode( int op) {
	SyntaxNode* ret = NULL;
	BOOST_FOREACH(SyntaxNode* node, this->values)
	{
		if(op == node->opCode)
		{
			ret = node; break;
		}
	}
	return ret;
}

bool SyntaxNode::isDepend(vector<ModelComp*> varComps) {
	LOG("isDepend -- "<<this->print());
	if (this->opCode == 0) {
		assert(this->values.size() == 1);
		return this->values[0]->isDepend(varComps);
	}
	else if (this->opCode == IDREF) {
		SyntaxNodeIDREF* refnode = static_cast<SyntaxNodeIDREF*>(this);
		ModelComp* comp = refnode->ref;
		for(vector<ModelComp*>::iterator it = varComps.begin();it != varComps.end();it++) {
			if (*it == comp) {
				return true;
			}
		}
		return false;
	}
	else {
		bool ret = false;
		for(int i = 0;i < this->values.size() && !ret;i++) {
			ret = this->values[i]->isDepend(varComps);
		}
		return ret;
	}
}

void SyntaxNode::getIndiciesKey(ModelContext* ctx, string& idxkey)
{
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

string SyntaxNode::printVector(vector<double>& v) {
	ostringstream oss;
	vector<double>::iterator it = v.begin();
	oss << "[";
	for(;it != v.end();it++) {
		oss << *it << " ";
	}
	oss << "]";
	return oss.str();
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


AutoDiff::OPCODE SyntaxNode::opCodeTranslateToAutoDiffOp(int opCode) {
	AutoDiff::OPCODE code;
	switch (opCode)
	{
		case PLUS:
			code = AutoDiff::OP_PLUS;
			break;
		case MINUS:
			code = AutoDiff::OP_MINUS;
			break;
		case TIMES:
			code = AutoDiff::OP_TIMES;
			break;
		case DIVID:
			code = AutoDiff::OP_DIVID;
			break;
//		case <this sin operator>:
//			code = AutoDiff::SIN;
//			break;
//			TIMES, DIVID, SIN, SQRT, POW
//		case <this sqrt operator>:
//			code = AutoDiff::SQRT;
//			break;
//		case <this pow operator>:
//			code = AutoDiff::POW;
//			break;
		default:
			LOG("opCode ["<<opCode<<"] haven't map translate yet!");
			assert(false);
			break;
	}
	return code;
}

/*
 * end of constraints separability calculation
 */

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
	else if (this->opCode == VALUE)
	{
		int level  = -1;
		partials.insert(pair<int,SyntaxNode*>(level,this));
	}
	else if(this->opCode == SUM)
	{
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
	else if(this->opCode == 0)
	{
		assert(false);//no 0 node anymore
		assert(this->values.size()==1);
		this->values[0]->calculatePartialConstraints(partials);
	}
	else if(this->opCode == ASSIGN)
	{
		assert(this->values.size()==2);
		this->values[0]->calculatePartialConstraints(partials);
	}
	else if(this->opCode == LSBRACKET)
	{
		assert(this->values[0]->opCode == IDREF);
		this->values[0]->calculatePartialConstraints(partials);
	}
	else
	{
		if(this->opCode == PLUS || this->opCode == MINUS )
		{//binary operators - additivly separable
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
					SyntaxNode* newNode = NULL;
					if(it_left == child_left.end() && it_right != child_right.end())
					{
						newNode = new SyntaxNodeOP(opCode,(*it_right).second);
						partials.insert(pair<int,SyntaxNode*>(i, newNode));
					}
					else if(it_right==child_right.end() && it_left != child_left.end())
					{
	//					newNode = new OpNode(opCode,(*it_left).second);
						partials.insert(pair<int,SyntaxNode*>(i, (*it_left).second));
					}
					else if(it_right!=child_right.end() && it_left!=child_left.end())
					{
						newNode = new SyntaxNodeOP(opCode,(*it_left).second,(*it_right).second);
						partials.insert(pair<int,SyntaxNode*>(i, newNode));
					}
					else
					{
						assert(newNode==NULL);
					}

				}
			}
			else if(nchild()==1)
			{
				assert(this->opCode == MINUS);
				boost::unordered_map<int,SyntaxNode*> child_left;
				this->values.at(0)->calculatePartialConstraints(child_left);
				assert(child_left.size()>=1);
				for(int i=-1;i<=AmplModel::MAX_LEVEL ; i++)
				{
					boost::unordered_map<int,SyntaxNode*>::iterator it_left = child_left.find(i);
					SyntaxNode* newNode = NULL;
					if(it_left != child_left.end())
					{
						newNode = new SyntaxNodeOP(MINUS,(*it_left).second);
						partials.insert(pair<int,SyntaxNode*>(i, newNode));
					}
				}
			}
			else
			{
				assert(false); // not possible
			}
		}
		else if(this->opCode == TIMES || this->opCode == DIVID || this->opCode == POWER)
		{//binary operator  -- non-separable
			assert(nchild()==2);
			boost::unordered_map<int,SyntaxNode*> child_left;
			boost::unordered_map<int,SyntaxNode*> child_right;
			this->values.at(0)->calculatePartialConstraints(child_left);
			this->values.at(1)->calculatePartialConstraints(child_right);
			assert(child_left.size()>=1);
			assert(child_right.size()>=1);
			int max_level;
			for(int i=AmplModel::MAX_LEVEL;i>=0;i--)
			{
				boost::unordered_map<int,SyntaxNode*>::iterator it_left;
				boost::unordered_map<int,SyntaxNode*>::iterator it_right;
				it_left = child_left.find(i);
				it_right = child_right.find(i);
				if(it_left!=child_left.end()||it_right!=child_right.end())
				{
					max_level = i;
					break;
				}
			}
			boost::unordered_map<int,SyntaxNode*>::iterator cur;
			cur = child_left.begin();
			SyntaxNode* leftNode = (*cur).second;
			cur++;
			while(cur!=child_left.end())
			{
				leftNode = new SyntaxNodeOP(PLUS,leftNode,(*cur).second);
				cur++;
			}

			cur= child_right.begin();
			SyntaxNode* rightNode = (*cur).second;
			cur++;
			while(cur!=child_left.end())
			{
				rightNode = new SyntaxNodeOP(PLUS,rightNode,(*cur).second);
				cur++;
			}

			SyntaxNode* newNode = new SyntaxNodeOP(opCode,leftNode, rightNode);
			partials.insert(pair<int,SyntaxNode*>(max_level,newNode));
		}
		else if(this->opCode == COS || this->opCode == SIN)
		{
			boost::unordered_map<int,SyntaxNode*> child_left;
			this->values.at(0)->calculatePartialConstraints(child_left);
			int max_level;
			for(int i=AmplModel::MAX_LEVEL;i>=0;i--)
			{
				boost::unordered_map<int,SyntaxNode*>::iterator it_left;
				it_left = child_left.find(i);
				if(it_left!=child_left.end())
				{
					max_level = i;
					break;
				}
			}
			boost::unordered_map<int,SyntaxNode*>::iterator cur;
			cur = child_left.begin();
			SyntaxNode* leftNode = (*cur).second;
			cur++;
			while(cur!=child_left.end())
			{
				leftNode = new SyntaxNodeOP(PLUS,leftNode,(*cur).second);
				cur++;
			}
			SyntaxNode* newNode = new SyntaxNodeOP(opCode,leftNode);
			partials.insert(pair<int,SyntaxNode*>(max_level,newNode));
		}
		else
		{
			LOG("calculatePartialConstraints --- opCode["<<opCode<<"] not yet implemented!");
			assert(false);
		}
	}
}

AutoDiff::Node* SyntaxNode::createAutoDiffConIDREF(ModelContext* ctx)
{
	LOG("enter createAutoDiffConIDREF   ["<<this->print()<<"]");
	AutoDiff::Node* con = NULL;
	SyntaxNodeIDREF* refn = static_cast<SyntaxNodeIDREF*>(this);
	ModelComp* comp = refn->ref;
	assert(comp->type == TVAR || comp->type == TPARAM);
	if (comp->type == TVAR) {
		Var* var = static_cast<Var*>(ctx->getCompValue(comp));
		assert(var!=NULL);
		string varIndex = "";
		if (refn->nchild() == 2) {
			refn->values[1]->getIndiciesKey(ctx, varIndex);
		}
		else
			assert(refn->nchild() == 1);
		var_multi_map_by_indicies::iterator it = var->varMultiMap.get<1>().find(varIndex);
		if (it != var->varMultiMap.get<1>().end()) {
			con = it->adv;
		}
		else {
			LOG("can't find AutoDiff var in ["<<var->name<<"] for varIndex["<<varIndex<<"]");
			LOG("please check the model!");
		}
	}
	else {
		assert(comp->type == TPARAM);
		SyntaxNodeIDREF* refn = static_cast<SyntaxNodeIDREF*>(this);
		CompDescr* compdescr = ctx->getCompValue(comp);
		if (typeid(*compdescr) == typeid(ParamMult)) {
			string key = "";
			assert(refn->nchild() == 2);
			refn->values[1]->getIndiciesKey(ctx, key);
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
	assert(con != NULL);
	LOG("exit createAutoDiffConIDREF ["<<this->print()<<"]----->AutoDiff:["<<con->toString(0)<<"]");
	return con;
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
AutoDiff::Node* SyntaxNode::buildAutoDiffDAG(ExpandedModel* emrow,ExpandedModel* emcol)
{
	LOG("enter - createAutoDiffConsDAG  - emrow["<<emrow->name<<"] emcol["<<emcol->name<<"] --- "<<this->print());
	ModelContext* ctx = emrow->ctx;
	AutoDiff::Node* con = NULL;
	if(this->opCode==ASSIGN)
	{
		con = this->values[0]->buildAutoDiffDAG(emrow,emcol);
	}
	else if(this->opCode == IDREF)
	{
		con = createAutoDiffConIDREF(ctx);
	}
	else if(this->opCode == DOT)
	{
		SyntaxNodeIDREFM* idrefm = static_cast<SyntaxNodeIDREFM*>(this->values[0]);
		AmplModel* model = idrefm->ref;
		string modeldummy = static_cast<SyntaxNodeID*>(this->values[0]->values[1]->values[0])->id;
		string modeldummyval = ctx->getDummyValue(modeldummy);
		//loacate the model context of the model dummy value is equal to modeldummyval.
		// ie. Model{j in Set} . when j = "A" , returns the ctx of Model["A"]
		ModelContext* context = ctx->em->locateCtx(model,modeldummyval);
		con = this->values[1]->createAutoDiffConIDREF(context);
	}
	else if(this->opCode == SUM)
	{
		SyntaxNode* indexing_set = this->values[0];
		assert(indexing_set->opCode == LBRACE);
		SyntaxNode* indexing_set_expr_list = indexing_set->values[0]->values[0];
		SyntaxNode* condition_opt = indexing_set->values[0]->values[1];

		assert(indexing_set_expr_list->nchild()==1); //support 1-index for now only
		SyntaxNode* set_expr = indexing_set_expr_list->values[0];
		assert(set_expr->opCode == IN);
		assert(set_expr->values[0]->opCode == COMMA);
		assert(set_expr->values[0]->nchild() == 1); //support 1-dim set ie. not yet support {(i,j) IN SET }
		string dummySum = static_cast<SyntaxNodeID*>(set_expr->values[0]->values[0])->id;
		ModelComp* compSum = static_cast<SyntaxNodeIDREF*>(set_expr->values[1])->ref;
		ModelComp* compEmcol = (emcol->model->indexing==NULL)?
				NULL : emcol->dummySetMap.begin()->second;
		string dummyEmcol = (emcol->model->indexing==NULL)?
				"" : emcol->dummySetMap.begin()->first;

		if(compSum == compEmcol && dummySum.compare(dummyEmcol)==0 && condition_opt == NULL)
		{
			LOG("SUM over {"<<dummySum<<" in "<<compSum->name<<"} -- Emcol over {"<<dummyEmcol<<" in "<<compEmcol->name<<"}");
			ctx->addDummyCompValueMapTemp(dummySum, compSum, (emcol->dummyValueMap.begin())->second);
			con = this->values[1]->buildAutoDiffDAG(emrow,emcol);
			ctx->removeDummySetValueMapTemp(dummySum);
		}
		else
		{
			LOG("SUM over {"<<dummySum<<" in "<<compSum->name<<"}");
			if(compEmcol!=NULL)
			{
				LOG("Emcol over {"<<dummyEmcol<<" in "<<compEmcol->name<<"}");
			}
			else
			{
				LOG("Emcol["<<emcol->name<<"] *NO* indexing");
			}
			SyntaxNode* index_set = this->values[0];
			IndexSet* iset = NULL;
			if (!ctx->getCalcSumSet(index_set, &iset))
			{
				iset = this->values[0]->createIndexSet(ctx);
				ctx->addCalcSumSet(index_set, iset);
			}
			assert(iset!=NULL);

			//build auto diff DAG sum expressions
			queue<AutoDiff::Node*> nodes;
			assert(iset->dummySetMap.size() ==  1); // only for one dummy variables set for sum expression
			boost::unordered_map<string,Set*>::iterator it = iset->dummySetMap.begin();

			Set* set = it->second;
			string dummy = it->first;
			vector<string>::iterator setv = set->setValues_data_order.begin();
			for(;setv!=set->setValues_data_order.end();setv++)
			{

				string value = *setv;
				ModelComp* comp = iset->dummyCompMap.find(dummy)->second;
				ctx->addDummyCompValueMapTemp(dummy, comp, value);
				AutoDiff::Node* n = this->values[1]->buildAutoDiffDAG(emrow,emcol);
				assert(n!=NULL);
				nodes.push(n);
				ctx->removeDummySetValueMapTemp(dummy);
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
			AutoDiff::Node* left = this->values[0]->buildAutoDiffDAG(emrow,emcol);
			con = create_uary_op_node(AutoDiff::OP_NEG,left);
		}
		else
		{
			assert(this->values.size()==2);
			AutoDiff::Node* left = this->values[0]->buildAutoDiffDAG(emrow,emcol);
			AutoDiff::Node* right = this->values[1]->buildAutoDiffDAG(emrow,emcol);
			con = create_binary_op_node(AutoDiff::OP_MINUS,left,right);
		}
	}
	else if(this->opCode == PLUS)
	{
		if(this->values.size()==1)
		{
			assert(this->values[0]!=NULL);
			con = this->values[0]->buildAutoDiffDAG(emrow,emcol);
		}
		else
		{
			assert(this->values.size()==2);
			AutoDiff::Node* left = this->values[0]->buildAutoDiffDAG(emrow,emcol);
			AutoDiff::Node* right = this->values[1]->buildAutoDiffDAG(emrow,emcol);
			con = create_binary_op_node(AutoDiff::OP_PLUS,left,right);
		}
	}
	else if(this->opCode == TIMES)
	{
		assert(this->nchild()==2);
		AutoDiff::Node* left = this->values[0]->buildAutoDiffDAG(emrow,emcol);
		AutoDiff::Node* right = this->values[1]->buildAutoDiffDAG(emrow,emcol);
		con = create_binary_op_node(AutoDiff::OP_TIMES,left,right);
	}
	else if(this->opCode == DIVID)
	{
		assert(this->nchild()==2);
		AutoDiff::Node* left = this->values[0]->buildAutoDiffDAG(emrow,emcol);
		AutoDiff::Node* right = this->values[1]->buildAutoDiffDAG(emrow,emcol);
		con = create_binary_op_node(AutoDiff::OP_DIVID,left,right);
	}
	else if(this->opCode == POWER)
	{
		assert(this->nchild()==2);
		AutoDiff::Node* left = this->values[0]->buildAutoDiffDAG(emrow,emcol);
		AutoDiff::Node* right = this->values[1]->buildAutoDiffDAG(emrow,emcol);
		con = AutoDiff::create_binary_op_node(AutoDiff::OP_POW,left,right);
	}
	else if(this->opCode == COS)
	{
		assert(this->nchild()==1);
		AutoDiff::Node* left = this->values[0]->buildAutoDiffDAG(emrow,emcol);
		con = AutoDiff::create_uary_op_node(AutoDiff::OP_COS,left);
	}
	else if(this->opCode == SIN)
	{
		assert(this->nchild()==1);
		AutoDiff::Node* left = this->values[0]->buildAutoDiffDAG(emrow,emcol);
		con = AutoDiff::create_uary_op_node(AutoDiff::OP_SIN,left);
	}
	else
	{
		LOG("opCode["<<this->opCode<<"] is not yet implemented! -- "<<this->print());
		assert(false);
	}
	LOG("exit - createAutoDiffConsDAG  - emrow["<<emrow->name<<"] emcol"<<emcol->name<<"]");
	return con;
}


SyntaxNode* SyntaxNode::moveConsToLeft()
{
	LOG("enter moveToLHSConstraints - - ["<<this->print());
	assert(this->opCode == ASSIGN);
	SyntaxNode* r1 = this->values[0];
	SyntaxNode* l1 = this->values[1];
	SyntaxNode* left = new SyntaxNodeOP(MINUS,r1,l1);
	SyntaxNode* ret = new SyntaxNode(ASSIGN,left,new SyntaxNodeValue(0));
	LOG("exit moveToLHSConstraints - - ["<<ret->print());
	return ret;
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


//legacy!!

//bool SyntaxNode::evalBool(boost::unordered_map<string, string>& dummyValueMap, boost::unordered_map<string, SetComp*>& paramIndicies, ModelContext* context) {
//	LOG("evalBool  ---  opCode["<<this->opCode<<"]  "<<this->print());
//	bool rval = false;
//	if (this->opCode == EQ) {
//		double r0 = this->values[0]->evalParamValueTerm(dummyValueMap, paramIndicies, context);
//		LOG("--- value for r0["<<r0<<"]");
//		double r1 = this->values[1]->evalParamValueTerm(dummyValueMap, paramIndicies, context);
//		LOG("--- value for r1["<<r1<<"]");
//		if (r0 == r1) {
//			rval = true;
//		}
//		else {
//			rval = false;
//		}
//	}
//	else if (this->opCode == GE) {
//		assert(false);
//		LOG("evalBool -- opCode[GE] not yet implmemented ");
//	}
//	else if (this->opCode == LE) {
//		assert(false);
//		LOG("evalBool -- opCode[LE] not yet implmemented ");
//	}
//	else if (this->opCode == LT) {
//		assert(false);
//		LOG("evalBool -- opCode[LT] not yet implmemented ");
//	}
//	else if (this->opCode == GT) {
//		LOG("evalBool -- opCode[GT] not yet implmemented ");
//		assert(false);
//	}
//	else if (this->opCode == LBRACKET) {
//		rval = this->values[0]->evalBool(dummyValueMap, paramIndicies, context);
//	}
//	return rval;
//}
//
//double SyntaxNode::evalParamValueTerm(boost::unordered_map<string, string>& dummyValueMap, boost::unordered_map<string, SetComp*>& paramIndicies, ModelContext* context) {
//	LOG("evalTerm  ---  opCode["<<this->opCode<<"]  "<<this->print());
//	double rval = 0;
//	if (this->opCode == IDREF) {
//		assert(this->values.size() <= 2);
//		SyntaxNodeIDREF* refn = static_cast<SyntaxNodeIDREF*>(this);
//		assert(refn->ref->type == TPARAM);
//		ModelComp* comp = refn->ref;
//		ValueParam* theParam = static_cast<ValueParam*>(context->getCompValue(comp));
//		vector<SyntaxNode*>::iterator it = this->values.begin();
//		ostringstream key(ostringstream::out);
//		for(;it != this->values.end();it++) {
//			SyntaxNodeID* ind = static_cast<SyntaxNodeID*>((*it)->values[0]);
//			assert(ind->opCode == ID);
//			string dummy = ind->id;
//			LOG("  ----  dummy name is ["<<dummy<<"]");
//			if (dummyValueMap.find(dummy) == dummyValueMap.end()) {
//				key << (context->getDummyValueTemp(dummy));
//			}
//			else {
//				key << (dummyValueMap.find(dummy)->second);
//			}
//			LOG("  -----  key is ["<<key.str()<<"]");
//		}
//		rval = theParam->findParamValue(key.str());
//	}
//	else if (this->opCode == ORD) {
//		assert(this->values[0]->opCode == LBRACKET);
//		assert(this->values[0]->values[0]->opCode==COMMA);
//		assert(this->values[0]->values[0]->values[0]->opCode == 0);
//		assert(this->values[0]->values[0]->values[0]->values[0]->opCode==ID);
//		SyntaxNodeID* in = static_cast<SyntaxNodeID*>(this->values[0]->values[0]->values[0]->values[0]);
//		string dummy = in->id;
//		ModelComp* comp = paramIndicies.find(dummy)->second;
//		OrderedSet* theSet = static_cast<OrderedSet*>(context->getCompValue(comp));
//		rval = (double) (theSet->setOrder(dummyValueMap.find(dummy)->second));
//	}
//	else if (this->opCode == 42) {
//		assert(this->values.size() == 2);
//		double left = this->values[0]->calculateParamValue(dummyValueMap, paramIndicies, context);
//		double right = this->values[1]->calculateParamValue(dummyValueMap, paramIndicies, context);
//		rval = left * right;
//	}
//	else if (this->opCode == 0) {
//		rval = this->values[0]->evalParamValueTerm(dummyValueMap, paramIndicies, context);
//	}
//	else {
//		LOG("evalTerm -- opCode["<<this->opCode<<"] not yet implmemented ");
//		assert(false);
//	}
//	return rval;
//}

//string SyntaxNode::setOrgSetComp(ModelComp** orgSetComp) {
//	LOG("setOrgSetComp - opCode["<<this->opCode<<"] "<<this->print());
//	string dummy = static_cast<SyntaxNodeID*>(this->values[0])->id;
//	*orgSetComp = static_cast<SyntaxNodeIDREF*>(this->values[1])->ref;
//	LOG(""<<static_cast<SyntaxNodeIDREF*>(this->values[1])->ref);
//	LOG("end setOrgSetComp - dummy["<<dummy<<"] ref["<<(*orgSetComp)->id<<"|"<<*orgSetComp<<"]");
//	return dummy;
//}

/*
 * construct an expression tree in autodiff library that represents a partial tree
 * in this syntaxnode expression. The partial tree is defined by the non-separable
 * intersection of emcol with this contraint evaluate at current ctx.
 */
//AutoDiff::Node* SyntaxNode::constructAutoDiffNode(ModelContext* ctx, Block* emb, ExpandedModel* emcol)
//{
//	AutoDiff::Node* node = NULL;
//	if(this->opCode == SUM)
//	{
//		//one-dim sum index only
//		ModelComp* compSum = static_cast<SyntaxNodeIDREF*>(SyntaxNode::findSyntaxNodeChild(this,IN)->values[1])->ref;
//		ModelComp* compCol = emcol->model->node->indexing==NULL?NULL:static_cast<SyntaxNodeIDREF*>(emcol->model->node->indexing->values[0]->values[1])->ref;
//		if(compCol!=NULL && compSum == compCol)
//		{
//			LOG("Sum same as Col over ["<<compCol->id<<"] Sum over["<<compSum->id<<"]");
//			string dummyVar = static_cast<SyntaxNodeID*>(SyntaxNode::findSyntaxNodeChild(this,IN)->values[0])->id;
//			assert(dummyVar.compare((emcol->dummySetMap.begin())->first)==0);
//			assert(compCol == (emcol->dummySetMap.begin())->second );
//			ctx->addDummySetValueMapTemp(dummyVar, compCol , (emcol->dummyValueMap.begin())->second);
//			node = this->values[1]->constructAutoDiffNode(ctx, emb, emcol);
//			ctx->removeDummySetValueMapTemp(dummyVar);
//		}
//		else
//		{
//			if(compCol!=NULL)
//			{
//				LOG("Sum -- Col over ["<<compCol->id<<"] Sum over["<<compSum->id<<"]");
//			}
//			else
//			{
//				LOG("Sum -- emcol["<<emcol->name<<"] has indexing --  Sum over["<<compSum->id<<"]");
//			}
//			IndexSet* iset = NULL;
//			ostringstream oss(ostringstream::out);
//			oss //<< rowContext->getModelDummyValAsKey() //don't need this one since context is hirechical structured! -Feng
//			<< ctx->getConsDummyValAsKey() << (void*) ((this->values[0])) << emcol->ctx;
//			string hashKey = oss.str();
//			ctx->getCalcSumSet(hashKey, &iset);
//			assert(iset!=NULL);
//			//build auto diff tree
//			queue<AutoDiff::Node*> nodes;
//			vector< vector<string> >::iterator it_setval = iset->setIndicies.begin();
//			for(;it_setval != iset->setIndicies.end() ;it_setval++)
//			{
//				vector<string> sval = *it_setval;
//				assert(iset->comps.size()== sval.size() && iset->comps.size() == iset->dummyVarNames.size());
//				vector<ModelComp*>::iterator it_comp = iset->comps.begin();
//				vector<string>::iterator it_dummyVar = iset->dummyVarNames.begin();
//				vector<string>::iterator it_sval = sval.begin();
//				for(;it_comp != iset->comps.end();it_comp++, it_dummyVar++,it_sval++) {
//					ctx->addDummySetValueMapTemp((*it_dummyVar), *it_comp, *it_sval);
//				}
//
//				AutoDiff::Node* n = this->values[1]->constructAutoDiffNode(ctx, emb, emcol);
//				if(n!=NULL)
//				{
//					nodes.push(n);
//					LOG(" -- push one ! ");
//				}
//				it_dummyVar = iset->dummyVarNames.begin();
//				for(;it_dummyVar != iset->dummyVarNames.end();it_dummyVar++) {
//					ctx->removeDummySetValueMapTemp(*it_dummyVar);
//				}
//			}
//
//			while(nodes.size()!=1)
//			{
//				LOG("sum over ["<<nodes.size()<<"] elements");
//				AutoDiff::Node* n1 = nodes.front();
//				nodes.pop();
//				AutoDiff::Node* n2 = nodes.front();
//				nodes.pop();
//				AutoDiff::Node* n = create_binary_op_node(PLUS,n1,n2);
//				nodes.push(n);
//			}
//			node = nodes.front();
//			assert(nodes.size()==1);
//		}
//	}
//	else if(this->opCode == PLUS)
//	{
//		assert(this->values.size()==2);
//		bool left = this->values[0]->isContainVariablesInEm2(ctx,emcol);
//		bool right= this->values[1]->isContainVariablesInEm2(ctx,emcol);
//		if(left && right)
//		{
//			AutoDiff::Node* n1 = this->values[0]->constructAutoDiffNode(ctx,emb,emcol);
//			AutoDiff::Node* n2 = this->values[1]->constructAutoDiffNode(ctx,emb,emcol);
//			node = create_binary_op_node(PLUS,n1,n2);
//		}
//		else if(left)
//		{
//			node = this->values[0]->constructAutoDiffNode(ctx,emb,emcol);
//		}
//		else if(right)
//		{
//			node = this->values[1]->constructAutoDiffNode(ctx,emb,emcol);
//		}
//	}
//	else if(this->opCode == MINUS)
//	{
//		assert(this->values.size()==2);
//		bool left = this->values[0]->isContainVariablesInEm2(ctx,emcol);
//		bool right= this->values[1]->isContainVariablesInEm2(ctx,emcol);
//		if(left && right)
//		{
//			AutoDiff::Node* n1 = this->values[0]->constructAutoDiffNode(ctx,emb,emcol);
//			AutoDiff::Node* n2 = this->values[1]->constructAutoDiffNode(ctx,emb,emcol);
//			node = create_binary_op_node(MINUS,n1,n2);
//		}
//		else if(left)
//		{
//			node = this->values[0]->constructAutoDiffNode(ctx,emb,emcol);
//		}
//		else if(right)
//		{
//			node = this->values[1]->constructAutoDiffNode(ctx,emb,emcol);
//		}
//	}
//	else if(this->opCode == TIMES)
//	{
//		assert(this->values.size()==2);
//		bool left = this->values[0]->isContainVariablesInEm2(ctx,emcol);
//		bool right= this->values[1]->isContainVariablesInEm2(ctx,emcol);
//		if(left && right)
//		{
//			AutoDiff::Node* n1 = this->values[0]->constructAutoDiffNode(ctx,emb);
//			AutoDiff::Node* n2 = this->values[1]->constructAutoDiffNode(ctx,emb);
//			node = create_binary_op_node(TIMES,n1,n2);
//		}
//		else if(left)
//		{
//			AutoDiff::Node* n1 = this->values[0]->constructAutoDiffNode(ctx,emb,emcol);
//			AutoDiff::Node* n2 = this->values[1]->constructAutoDiffNode(ctx,emb);
//			node = create_binary_op_node(TIMES,n1,n2);
//		}
//		else if(right)
//		{
//			AutoDiff::Node* n1 = this->values[1]->constructAutoDiffNode(ctx,emb,emcol);
//			AutoDiff::Node* n2 = this->values[0]->constructAutoDiffNode(ctx,emb);
//			node = create_binary_op_node(TIMES,n1,n2);
//		}
//	}
//	else if(this->opCode == DIVID)
//	{
//		assert(this->values.size()==2);
//		bool left = this->values[0]->isContainVariablesInEm2(ctx,emcol);
//		bool right= this->values[1]->isContainVariablesInEm2(ctx,emcol);
//		if(left && right)
//		{
//			AutoDiff::Node* n1 = this->values[0]->constructAutoDiffNode(ctx,emb);
//			AutoDiff::Node* n2 = this->values[1]->constructAutoDiffNode(ctx,emb);
//			node = create_binary_op_node(DIVID,n1,n2);
//		}
//		else if(left)
//		{
//			AutoDiff::Node* n1 = this->values[0]->constructAutoDiffNode(ctx,emb,emcol);
//			AutoDiff::Node* n2 = this->values[1]->constructAutoDiffNode(ctx,emb);
//			node = create_binary_op_node(DIVID,n1,n2);
//		}
//		else if(right)
//		{
//			AutoDiff::Node* n1 = this->values[1]->constructAutoDiffNode(ctx,emb,emcol);
//			AutoDiff::Node* n2 = this->values[0]->constructAutoDiffNode(ctx,emb);
//			node = create_binary_op_node(DIVID,n1,n2);
//		}
//	}
//	else if(this->opCode == IDREF)
//	{
//		node = this->constructAutoDiffNode(ctx,emb);
//	}
//	else if(this->opCode == ASSIGN)
//	{
//		assert(this->values.size()==2);
//		node = this->values[0]->constructAutoDiffNode(ctx,emb,emcol);
//	}
//	else if(this->opCode == 0)
//	{
//		assert(this->values.size()==1);
//		node = this->values[0]->constructAutoDiffNode(ctx,emb,emcol);
//	}
//	else if(this->opCode == LBRACKET)
//	{
//		assert(this->values.size()==1);
//		node = this->values[0]->constructAutoDiffNode(ctx,emb,emcol);
//	}
//	else if(this->opCode == COMMA)
//	{
//		assert(this->values.size()==1);
//		node = this->values[0]->constructAutoDiffNode(ctx,emb,emcol);
//	}
//	else
//	{
//		LOG(" -- constructAutoDiffNode -- opCode["<<opCode<<"] is not implmeneted yet!");
//		assert(false);
//	}
//	return node;
//}

/*
 * helper function for constructAutoDiffNode
 */
//AutoDiff::Node* SyntaxNode::constructAutoDiffNode(ModelContext* ctx, Block* emb)
//{//build the auto diff node tree (no check of separability)
//	AutoDiff::Node* node = NULL;
//	if(this->opCode == IDREF)
//	{
//		SyntaxNodeIDREF* refn = static_cast<SyntaxNodeIDREF*>(this);
//		ModelComp* comp = refn->ref;
//		if(comp->type == TVAR)
//		{
//			string varKey="";
//			SyntaxNode::getIndiciesKey(varKey,refn,ctx);
//			varKey = comp->id +"_" + varKey;
//			node = emb->findVariable(varKey);
//		}
//		else
//		{
//			assert(comp->type == TPARAM);
//			SyntaxNodeIDREF* refn = static_cast<SyntaxNodeIDREF*>(this);
//			Param* theParam = static_cast<Param*>(ctx->getCompValue(comp));
//			string key;
//			SyntaxNode::getIndiciesKey(key,refn,ctx);
//			node = create_param_node(theParam->findParamValue(key));
//		}
//		assert(node != NULL);
//	}
//	else if(this->opCode == 0)
//	{
//		assert(this->values.size()==1);
//		node = this->values[0]->constructAutoDiffNode(ctx,emb);
//	}
//	else if(this->opCode == LBRACKET)
//	{
//		assert(this->values.size()==1);
//		node = this->values[0]->constructAutoDiffNode(ctx,emb);
//	}
//	else if(this->opCode == COMMA)
//	{
//		assert(this->values.size()==1);
//		node = this->values[0]->constructAutoDiffNode(ctx,emb);
//	}
//	else if(this->opCode == PLUS)
//	{
//		assert(this->values.size()==2);
//		AutoDiff::Node* n1 = this->values[0]->constructAutoDiffNode(ctx,emb);
//		AutoDiff::Node* n2 = this->values[1]->constructAutoDiffNode(ctx,emb);
//		node = create_binary_op_node(PLUS,n1,n2);
//	}
//	else if(this->opCode == MINUS)
//	{
//		assert(this->values.size()==2);
//		AutoDiff::Node* n1 = this->values[0]->constructAutoDiffNode(ctx,emb);
//		AutoDiff::Node* n2 = this->values[1]->constructAutoDiffNode(ctx,emb);
//		node = create_binary_op_node(MINUS,n1,n2);
//	}
//	else if(this->opCode == -99)
//	{
//		SyntaxNodeValue* valn = static_cast<SyntaxNodeValue*>(this);
//		double start = valn->value;
//		node = create_param_node(start);
//	}
//	else
//	{
//		LOG(" -- constructAutoDiffNode - no check sep -- opCode["<<opCode<<"] is not implmeneted yet!");
//		assert(false);
//	}
//	return node;
//}

/*
 * This SyntaxNode expression could contain multiple variable definitions.
 * Those variables defined levels will be filled into levels.
 */
//void SyntaxNode::calcVarDefinedLevels(set<int>& levels)
//{
////	LOG("calcConsVarDependicies - "<<this->print());
//	if(this->opCode == IDREF)
//	{
//		SyntaxNodeIDREF* refn = static_cast<SyntaxNodeIDREF*>(this);
//		ModelComp* comp = refn->ref;
//		if(comp->type == TVAR)
//		{
//			LOG("id["<<comp->id<<"] model["<<comp->model->name<<"] level["<<comp->model->level<<"]");
//			if(levels.find(comp->model->level) == levels.end())
//			{//not found -- not already in levels collection
//				levels.insert(comp->model->level);
//			}
//		}
//	}
//	else
//	{
//		vector<SyntaxNode*>::iterator it = this->values.begin();
//		for(;it!=this->values.end();it++)
//		{
//			(*it)->calcVarDefinedLevels(levels);
//		}
//	}
//}

/* given a level and fill the dependent levels in deps for evaluating derivatives
 * this must be an attribute for a constraint or objective
*/
//void SyntaxNode::calcSeparability(int level, set<int>& deps)
//{
//	//LOG("calcSeparability - "<<this->print());
//	if(this->opCode == PLUS || this->opCode == MINUS)
//	{
//		if(this->values[0]->containsVarDefInLevel(level))
//		{
//			this->values[0]->calcSeparability(level,deps);
//		}
//		if(this->values[1]->containsVarDefInLevel(level))
//		{
//			this->values[1]->calcSeparability(level,deps);
//		}
//	}
//	else if(this->opCode == TIMES || this->opCode == DIVID)
//	{
//		bool left = this->values[0]->containsVarDefInLevel(level);
//		bool right = this->values[1]->containsVarDefInLevel(level);
//		if(left && right)
//		{
//			this->values[1]->calcVarDefinedLevels(deps);
//			this->values[0]->calcVarDefinedLevels(deps);
//		}
//		else if(left)
//		{
//			this->values[1]->calcVarDefinedLevels(deps);
//			this->values[0]->calcSeparability(level,deps);
//		}
//		else if(right)
//		{
//			this->values[0]->calcVarDefinedLevels(deps);
//			this->values[1]->calcSeparability(level,deps);
//		}
//		else
//		{
//			assert(false);
//			//one of them must constain level variable used.
//		}
//	}
//	else if(this->opCode == IDREF)
//	{
//		SyntaxNodeIDREF* refn = static_cast<SyntaxNodeIDREF*>(this);
//		ModelComp* comp = refn->ref;
//		if(comp->type == TVAR)
//		{
//			//LOG("id["<<comp->id<<"] model["<<comp->model->name<<"] level["<<comp->model->level<<"]");
//			if(deps.find(comp->model->level) == deps.end())
//			{//not found
//				deps.insert(comp->model->level);
//			}
//		}
//	}
//	else if(this->opCode == SUM)
//	{
//		this->values[1]->calcSeparability(level,deps);
//	}
//	else if(this->opCode == ASSIGN)
//	{
//		this->values[0]->calcSeparability(level,deps);
//	}
//	else if(this->opCode == -99)
//	{//value node do nothing!
//
//	}
//	else if (this->opCode == 0)
//	{
//		this->values[0]->calcSeparability(level,deps);
//	}
//	else if(this->opCode == LBRACKET)
//	{
//		this->values[0]->calcSeparability(level,deps);
//	}
//	else if(this->opCode == COMMA)
//	{
//		this->values[0]->calcSeparability(level,deps);
//	}
//	else
//	{
//		LOG(" -- calcSeparability -- opCode["<<opCode<<"] is not implmeneted yet!");
//		assert(false);
//	}
//}

/* return true if this syntax tree contains TVAR defined at level
 * otherwise, return false.
 */
//bool SyntaxNode::containsVarDefInLevel(int level)
//{
//	if(this->opCode == IDREF)
//	{
//		SyntaxNodeIDREF* refn = static_cast<SyntaxNodeIDREF*>(this);
//		ModelComp* comp = refn->ref;
//		if(comp->type == TVAR)
//		{
//			return comp->model->level == level;
//		}
//	}
//
//	bool rval = false;
//	vector<SyntaxNode*>::iterator it = this->values.begin();
//	for(;it!=this->values.end();it++)
//	{
//		rval = (*it)->containsVarDefInLevel(level);
//		if(rval)
//		{
//			return true;
//		}
//	}
//	return rval;
//}


/*
 * return true if this synatxnode expression contains variables specified in emcol
 * otherwise, return false.
 */
//bool SyntaxNode::isContainVariablesInEm2(ModelContext* ctx,ExpandedModel* emcol)
//{
//	bool rval = false;
//	if(this->opCode == SUM)
//	{
//		IndexSet* iset = NULL;
//		ostringstream oss(ostringstream::out);
//		oss //<< rowContext->getModelDummyValAsKey() //don't need this one since context is hirechical structured! -Feng
//		<< ctx->getConsDummyValAsKey() << (void*) ((this->values[0])) << emcol->ctx;
//		string hashKey = oss.str();
//		if (!ctx->getCalcSumSet(hashKey, &iset))
//		{
//			iset = new IndexSet("TEMP_SUM");
//			this->values[0]->calcSumSetComp(ctx, &iset);
//			LOG("sum { dummyVar } over "<<iset->toString());
//			LOG("-- Add Temp Set - hashKey["<<hashKey<<"] overSet "<<iset->toString());
//			ctx->addCalcSumSet(hashKey, iset);
//		}
//
//		vector< vector<string> >::iterator it_setval = iset->setIndicies.begin();
//		for(;it_setval != iset->setIndicies.end() && rval == false ;it_setval++) {
//			vector<string> sval = *it_setval;
//			assert(iset->comps.size()== sval.size() && iset->comps.size() == iset->dummyVarNames.size());
//			vector<ModelComp*>::iterator it_comp = iset->comps.begin();
//			vector<string>::iterator it_dummyVar = iset->dummyVarNames.begin();
//			vector<string>::iterator it_sval = sval.begin();
//			for(;it_comp != iset->comps.end();it_comp++, it_dummyVar++,it_sval++) {
//				ctx->addDummySetValueMapTemp((*it_dummyVar), *it_comp, *it_sval);
//			}
//
//			rval = this->values[1]->isContainVariablesInEm2(ctx, emcol);
//
//			it_dummyVar = iset->dummyVarNames.begin();
//			for(;it_dummyVar != iset->dummyVarNames.end();it_dummyVar++) {
//				ctx->removeDummySetValueMapTemp(*it_dummyVar);
//			}
//		}
//	}
//	else if(this->opCode == IDREF)
//	{
//		SyntaxNodeIDREF* refn = static_cast<SyntaxNodeIDREF*>(this);
//		ModelComp* comp = refn->ref;
//		if(comp->type == TVAR)
//		{
//			string varKey="";
//			SyntaxNode::getIndiciesKey(varKey,refn,ctx);
//			rval = SyntaxNode::isContainsInEm2(varKey,emcol);
//		}
//	}
//	else{
//
//		vector<SyntaxNode*>::iterator it = this->values.begin();
//		for(;it!=this->values.end() && rval == false;it++)
//		{
//			rval = (*it)->isContainVariablesInEm2(ctx,emcol);
//		}
//	}
//	return rval;
//}

//void SyntaxNode::evalDiff(ModelContext* rowContext, ModelContext* colContext, vector<double>& jcobs) {
//	LOG("evalDiff --- opCode["<<this->opCode<<"] -- jcobs.size["<<jcobs.size()<<"] -- "<<this->print());
//	if (this->opCode == ASSIGN) {
//		assert(this->values.size() == 2);
//		this->values[0]->evalDiff(rowContext, colContext, jcobs);
//		//right hand side ignored for diff calculation
//	}
//	else if (this->opCode == SUM) {
//		handleSum(rowContext, jcobs, colContext);
//	}
//	else if (this->opCode == MINUS) {
//		if (values.size() == 1) {
//			this->values[0]->evalDiff(rowContext, colContext, jcobs);
//			this->negateVector(jcobs);
//		}
//		else {
//			assert(this->values.size() == 2);
//			vector<double> rightJcobs(jcobs.size(), 0);
//			vector<double> leftJcobs(jcobs.size(), 0);
//			this->values[0]->evalDiff(rowContext, colContext, leftJcobs);
//			this->values[1]->evalDiff(rowContext, colContext, rightJcobs);
//			assert(jcobs.size() == leftJcobs.size() && leftJcobs.size() == rightJcobs.size());
//			this->minusVector(leftJcobs, rightJcobs, jcobs);
//		}
//	}
//	else if (this->opCode == PLUS) {
//		if (values.size() == 1) {
//			this->values[0]->evalDiff(rowContext, colContext, jcobs);
//		}
//		else {
//			assert(this->values.size() == 2);
//			vector<double> leftJcobs(jcobs.size(), 0);
//			vector<double> rightJcobs(jcobs.size(), 0);
//			this->values[0]->evalDiff(rowContext, colContext, leftJcobs);
//			this->values[1]->evalDiff(rowContext, colContext, rightJcobs);
//			assert(jcobs.size() == leftJcobs.size() && leftJcobs.size() == rightJcobs.size());
//			this->plusVector(leftJcobs, rightJcobs, jcobs);
//		}
//	}
//	else if (this->opCode == 0) {
//		this->values[0]->evalDiff(rowContext, colContext, jcobs);
//	}
//	else if (this->opCode == IDREF) {
//		SyntaxNodeIDREF* refn = static_cast<SyntaxNodeIDREF*>(this);
//		ModelComp* comp = refn->ref;
//		if (comp->type == TPARAM) {
//			LOG("got a parameter type rval = 0");
//		}
//		else if (comp->type == TVAR) {
//			ExpandedModel* em2 = colContext->em2;
//			vector<Var*>::iterator it_var = em2->localVars.begin();
//			vector<ModelComp*>::iterator it_mc = em2->localVarComps.begin();
//			vector<double>::iterator it_jcobs = jcobs.begin();
//			assert(jcobs.size() == em2->numLocalVars);
//			string currVarKey = "";
//			bool currVarKeyReady = false;
//			bool found = false;
//			while(!found && it_jcobs != jcobs.end()) {
//				ModelComp* varComp = *it_mc;
//				if (varComp == comp) {
//					if (!currVarKeyReady) {
//						LOG("["<<varComp->id<<"] matches - now calculate varKey");
//						SyntaxNode::getIndiciesKey(currVarKey, refn, rowContext);
//						currVarKeyReady = true;
//						LOG("current variable key - var_key["<<currVarKey<<"]");
//					}
//
//					//now checking varKey in Var set if matches any
//					vector<string>::iterator it2 = (*it_var)->indicies.begin();
//
//					while(!found && it2 != (*it_var)->indicies.end()) {
//						string& varKey = (*it2);
//						if (varKey.compare(currVarKey) == 0) {
//							*it_jcobs = 1;
//							found = true;
//							LOG("["<<varComp->id<<"] matches ["<<varKey<<"] matches ");
//						}
//						else {
//							LOG("["<<varComp->id<<"] matches ["<<varKey<<"] not match ");
//							*it_jcobs = 0;
//						}
//						it_jcobs++;
//						it2++;
//					}
//				}
//				else {
//					LOG("["<<varComp->id<<"] not match - go to next varComp -- card["<<(*it_var)->card<<"]");
//					for(int j = 0;j < (*it_var)->card;j++, it_jcobs++) {
//						*it_jcobs = 0;
//					}
//				}
//				//increment to next varComp
//				it_var++;
//				it_mc++;
//			}
//			while(it_jcobs != jcobs.end()) {
//				*it_jcobs = 0;
//				it_jcobs++;
//			}
//		}
//	}
//	else if (this->opCode == 42) //42 is times -- handles only linear case
//	{
//		assert(this->values.size() == 2);
//		vector<double> leftJcobs(jcobs.size(), 0);
//		vector<double> rightJcobs(jcobs.size(), 0);
//		this->values[0]->evalDiff(rowContext, colContext, leftJcobs);
//		this->values[1]->evalDiff(rowContext, colContext, rightJcobs);
//		assert(jcobs.size() == leftJcobs.size() && leftJcobs.size() == rightJcobs.size());
//		assert(this->isZeroVector(leftJcobs) || this->isZeroVector(rightJcobs));
//		if (this->isZeroVector(leftJcobs) && !this->isZeroVector(rightJcobs)) {
//			double val = this->values[0]->evalTerm(rowContext);
//			this->multVectorScalar(rightJcobs, val, jcobs);
//		}
//		else if (!this->isZeroVector(leftJcobs) && this->isZeroVector(rightJcobs)) {
//			double val = this->values[1]->evalTerm(rowContext);
//			this->multVectorScalar(leftJcobs, val, jcobs);
//		}
//	}
//	else if (this->opCode == LBRACKET) {
//		this->values[0]->evalDiff(rowContext, colContext, jcobs);
//	}
//	else if (this->opCode == COMMA) {
//		this->values[0]->evalDiff(rowContext, colContext, jcobs);
//	}
//	else if (this->opCode == -99) { //a value AutoDiff::
//
//	}
//	else {
//		LOG(" -- opCode["<<opCode<<"] is not implmeneted yet for automatic differentiation!");
//		assert(false);
//	}
//}

//void SyntaxNode::handleSum(ModelContext* rowContext, vector<double>& jcobs, ModelContext* colContext) {
////	vector<ModelComp*> comps;
////	Set* aSet = NULL;
////	vector<string> dummyVars;
////	ostringstream oss;
////	oss //<< rowContext->getModelDummyValAsKey() //don't need this one since context is hirechical structured! -Feng
////			<< rowContext->getConsDummyValAsKey()
////			<< (void*) ((this->values[0]));
////	string hashKey = oss.str();
////	if (!rowContext->getCalcTempSet(hashKey, comps, &aSet, dummyVars))
////	{
////		this->values[0]->calcTempSetComp(rowContext, comps, &aSet, dummyVars);
////		LOG("sum { dummyVar } over "<<aSet->toString());
////		LOG(
////				"-- Add Temp Set - hashKey["<<hashKey<<"] overSet "<<aSet->toString());
////		rowContext->addCalcTempSet(hashKey, comps, aSet, dummyVars);
////	}
////	foreachSetValue(comps, dummyVars, aSet, rowContext, jcobs, colContext);
//
//	ExpandedModel* rowEm2 = rowContext->em2;
//	ExpandedModel* colEm2 = colContext->em2;
//
//	SyntaxNode* cons_sum_node = SyntaxNode::findSyntaxNodeChild(this, IN);
//	SyntaxNode* col_index_node = NULL;
//	if (colEm2->model->node != NULL && colEm2->model->node->indexing != NULL) {
//		col_index_node = SyntaxNode::findSyntaxNodeChild(colEm2->model->node->indexing, IN);
//	}
//
//	SyntaxNodeIDREF* consSumRef = static_cast<SyntaxNodeIDREF*>(cons_sum_node->values[1]);
//	if (col_index_node != NULL && consSumRef->ref == static_cast<SyntaxNodeIDREF*>(col_index_node->values[1])->ref) {
//		LOG("EQUAL - CONS SUM Indexing["<<cons_sum_node->print()<<"] Col Model Indexing["<<col_index_node->print()<<"]");
//		string dummyVar = (colEm2->dummySetMap.begin())->first;
//		rowContext->addDummySetValueMapTemp(dummyVar, (colEm2->dummySetMap.begin())->second, (colEm2->dummyValueMap.begin())->second);
//		this->values[1]->evalDiff(rowContext, colContext, jcobs);
//		rowContext->removeDummySetValueMapTemp(dummyVar);
//	}
//	else {
//		LOG("NOT - CONS SUM Indexing["<<cons_sum_node->print()<<"] Col Model Indexing["<<col_index_node->print()<<"]");
//		if (this->values[1]->isDepend(colEm2->localVarComps)) {
////			vector<ModelComp*> comps;
////			vector<string> dummyVars;
//			IndexSet* iset = NULL;
//			ostringstream oss(ostringstream::out);
//			oss //<< rowContext->getModelDummyValAsKey() //don't need this one since context is hirechical structured! -Feng
//			<< rowContext->getConsDummyValAsKey() << (void*) ((this->values[0])) << colContext;
//			string hashKey = oss.str();
//			if (!rowContext->getCalcSumSet(hashKey, &iset))
//			{
//				iset = new IndexSet("TEMP_SUM");
//				this->values[0]->calcSumSetComp(rowContext, &iset);
//				LOG("sum { dummyVar } over "<<iset->toString());
//				LOG("-- Add Temp Set - hashKey["<<hashKey<<"] overSet "<<iset->toString());
//				rowContext->addCalcSumSet(hashKey, iset);
//			}
//
//			vector< vector<string> >::iterator it_setval = iset->setIndicies.begin();
//			for(;it_setval != iset->setIndicies.end();it_setval++) {
//				vector<string> sval = *it_setval;
//				assert(iset->comps.size()== sval.size() && iset->comps.size() == iset->dummyVarNames.size());
//				vector<ModelComp*>::iterator it_comp = iset->comps.begin();
//				vector<string>::iterator it_dummyVar = iset->dummyVarNames.begin();
//				vector<string>::iterator it_sval = sval.begin();
//				for(;it_comp != iset->comps.end();it_comp++, it_dummyVar++,it_sval++) {
//					rowContext->addDummySetValueMapTemp((*it_dummyVar), *it_comp, *it_sval);
//				}
//
//				vector<double> currJcobs(jcobs.size(), 0);
//				this->values[1]->evalDiff(rowContext, colContext, currJcobs);
//				assert(currJcobs.size() == jcobs.size());
//				this->plusVector(jcobs, currJcobs, jcobs);
//
//				it_dummyVar = iset->dummyVarNames.begin();
//				for(;it_dummyVar != iset->dummyVarNames.end();it_dummyVar++) {
//					rowContext->removeDummySetValueMapTemp(*it_dummyVar);
//				}
//			}
//		}
//		else {
//			LOG("["<<this->values[1]->print()<<"] not depend on ["<<colEm2->model->name<<"]'s localvar");
//		}
//	}
//}

//void SyntaxNode::negateVector(vector<double>& v) {
//	LOG("negateVector - "<<printVector(v));
//	vector<double>::iterator it1 = v.begin();
//	for(;it1 != v.end();it1++) {
//		(*it1) = -(*it1);
//	}
//	LOG("end negateVector - "<<printVector(v));
//}

//bool SyntaxNode::isZeroVector(vector<double>& v) {
//	double sum = 0;
//	for(vector<double>::iterator it = v.begin();it != v.end();it++) {
//		sum += *it;
//	}
//	bool rval = sum == 0.0;
//	LOG("isZeroVector - "<<printVector(v)<<" -- "<<rval);
//	return rval;
//}
//
//void SyntaxNode::multVectorScalar(vector<double>& v, double scalar, vector<double>& result) {
//	LOG("multVectorScalar - "<<printVector(v)<<" - scalar["<<scalar<<"]");
//	vector<double>::iterator it1 = v.begin();
//	vector<double>::iterator it3 = result.begin();
//	for(;it1 != v.end();it1++, it3++) {
//		(*it3) = (*it1) * scalar;
//	}
//	LOG("multVectorScalar - "<<printVector(result)<<"");
//}
//
//void SyntaxNode::plusVector(vector<double>& left, vector<double>& right, vector<double>& result) {
//	LOG("plusVector - "<<printVector(left)<<"");
//	LOG("plusVector - "<<printVector(right)<<"");
//	vector<double>::iterator it1 = left.begin();
//	vector<double>::iterator it2 = right.begin();
//	vector<double>::iterator it3 = result.begin();
//	for(;it1 != left.end();it1++, it2++, it3++) {
//		(*it3) = (*it1) + (*it2);
//	}
//	LOG("plusVector - "<<printVector(result)<<"");
//}
//void SyntaxNode::minusVector(vector<double>& left, vector<double>& right, vector<double>& result) {
//	LOG("minusVector - "<<printVector(left)<<"");
//	LOG("minusVector - "<<printVector(right)<<"");
//	vector<double>::iterator it1 = left.begin();
//	vector<double>::iterator it2 = right.begin();
//	vector<double>::iterator it3 = result.begin();
//	for(;it1 != left.end();it1++, it2++, it3++) {
//		(*it3) = (*it1) - (*it2);
//	}
//	LOG("minusVector - "<<printVector(result)<<"");
//}


//Set* SyntaxNode::calculateTempSet(ModelContext* context)
//{
//	Set* value = NULL;
//	if (this->opCode == ID) {
//		LOG(" id node here ");
////		SyntaxNodeID* idn = static_cast<SyntaxNodeID*>(this);
////		value = new SimpleSet(Set::TMP,1);
////		string dummy = idn->id;
////		ostringstream oss(ostringstream::out);
////		oss<<context->getDummyValue(dummy);
////		value->addSetValue(oss);
//	}
//	else if (this->opCode == DOTDOT) {
//
//	}
//	else if (this->opCode == LBRACE) {
//		assert(this->nchild() == 1);
//		value = this->values[0]->calculateSetValue(context);
//	}
//
//	return value;
//}

///* --------------------------------------------------------------------------
// SyntaxNode *SyntaxNode::deep_copy()
// ---------------------------------------------------------------------------- */
//SyntaxNode* SyntaxNode::deep_copy() {
//	SyntaxNode *newn = new SyntaxNode(opCode);
//
//	if (opCode == IDREF || opCode == IDREFM) {
//		cerr << "IDREF SyntaxNodes need to be cloned differently\n";
//		exit(1);
//	}
//
//	/* Values are copied depending on the type of the SyntaxNode */
//	/* ID/IDREF/INT_VAL/FLOAT_VAL/IDREFM are treated differently */
//	if (opCode == ID) {
//		cerr << "Called deep_copy for ID" << endl;
//		throw exception();
//	}
//
//	for(SyntaxNode::iterator i = begin();i != end();++i)
//		newn->push_back((*i)->deep_copy());
//	return newn;
//}
///* --------------------------------------------------------------------------
// SyntaxNode *SyntaxNode::clone()
// ---------------------------------------------------------------------------- */
//SyntaxNode* SyntaxNode::clone() {
//	SyntaxNode *newn = new SyntaxNode(opCode);
//
//	if (opCode == IDREF) {
//		cerr << "IDREF SyntaxNodes need to be cloned differently\n";
//		exit(1);
//	}
//	newn->values = values;
//
//	return newn;
//}

///* --------------------------------------------------------------------------
// SyntaxNode::findIDREF(list<ModelComp> *lmc)
// ---------------------------------------------------------------------------- */
///** Find all the IDREF nodes at or below the current node */
//void SyntaxNode::findIDREF(list<ModelComp*>& lmc) const {
//
//	if (opCode == ID)
//		return;
//
//	if (opCode == IDREF) {
//		//printf("%s\n",getGlobalName((ModelComp*)this->values[0],
//		//				NULL, NULL, NOARG));
//		lmc.push_back(((SyntaxNodeIDREF*) this)->ref);
//	}
//	else
//	{
//		for(SyntaxNode::iterator i = begin();i != end();++i)
//		{
//			if (*i)
//			{
//				(*i)->findIDREF(lmc);
//
//			}
//		}
//	}
//}
//
///* --------------------------------------------------------------------------
// SyntaxNode::findIDREF(list<SyntaxNode *> *lnd)
// ---------------------------------------------------------------------------- */
///** Find all the IDREF nodes at or below the current node */
//void SyntaxNode::findIDREF(list<SyntaxNode*> *lnd) {
//
//	findOpCode(IDREF, lnd);
//}
//
///* --------------------------------------------------------------------------
// SyntaxNode::findOpCode(int oc, list<SyntaxNode *> *lnd)
// ---------------------------------------------------------------------------- */
///** Find all nodes of opCode @a oc at or below the current node */
//void SyntaxNode::findOpCode(int oc, list<SyntaxNode*> *lnd) {
//
//	// if terminal then return
//	if (opCode == ID)
//		return;
//
//	if (opCode == oc) {
//		//printf("%s\n",getGlobalName((ModelComp*)this->values[0],
//		//				NULL, NULL, NOARG));
//		lnd->push_back(this);
//	}
//	else {
//		for(SyntaxNode::iterator i = begin();i != end();++i)
//			if (*i)
//				(*i)->findOpCode(oc, lnd);
//	}
//}
