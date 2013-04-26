/*
 * SyntaxNode1.cpp
 *
 *  Created on: 28 Sep 2011
 *      Author: s0965328
 */

#include "SyntaxNode.h"
#include "SyntaxNodeIx.h"
#include "SyntaxNodeIDREF.h"
#include "ValueNode.h"
#include "ListNode.h"
#include "IDNode.h"
#include "../parser/sml.tab.h"
#include "../util/global_util_functions.h"
#include "../context/Param.h"
#include "../util/DummyVariableGenerator.h"
#include "../context/ExpandedModel2.h"

//int SyntaxNode::use_global_names=0;
AmplModel *SyntaxNode::default_model = NULL;

extern int n_indexing;
extern SyntaxNodeIx *list_of_indexing[20];

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

string SyntaxNode::print() const {
	ostringstream ost;
	ost << (*this);
	return ost.str();
}

void SyntaxNode::dump(ostream& fout) const {
	fout << print_SyntaxNodesymb(this) << "\n";
}

/* ==========================================================================
 SyntaxNode Methods to follow
 ============================================================================*/
// constructors:
SyntaxNode::SyntaxNode(const SyntaxNode &src) :
		opCode(src.opCode), values(src.values) {
	LOG("Creating syntaxNode opCode["<<src.opCode<<"]");

}

SyntaxNode::SyntaxNode(int code, SyntaxNode *val1, SyntaxNode *val2, SyntaxNode* val3) :
		opCode(code) {
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

/* --------------------------------------------------------------------------
 SyntaxNode *SyntaxNode::deep_copy()
 ---------------------------------------------------------------------------- */
SyntaxNode* SyntaxNode::deep_copy() {
	SyntaxNode *newn = new SyntaxNode(opCode);

	if (opCode == IDREF || opCode == IDREFM) {
		cerr << "IDREF SyntaxNodes need to be cloned differently\n";
		exit(1);
	}

	/* Values are copied depending on the type of the SyntaxNode */
	/* ID/IDREF/INT_VAL/FLOAT_VAL/IDREFM are treated differently */
	if (opCode == ID) {
		cerr << "Called deep_copy for ID" << endl;
		throw exception();
	}

	for(SyntaxNode::iterator i = begin();i != end();++i)
		newn->push_back((*i)->deep_copy());
	return newn;
}
/* --------------------------------------------------------------------------
 SyntaxNode *SyntaxNode::clone()
 ---------------------------------------------------------------------------- */
SyntaxNode* SyntaxNode::clone() {
	SyntaxNode *newn = new SyntaxNode(opCode);

	if (opCode == IDREF) {
		cerr << "IDREF SyntaxNodes need to be cloned differently\n";
		exit(1);
	}
	newn->values = values;

	return newn;
}

/* --------------------------------------------------------------------------
 char *SyntaxNode::printDummyVar()
 ---------------------------------------------------------------------------- */
/* Assumes that the current SyntaxNode is the dummy variable in an indexing
 expression: that is it, is either ID or LBRACKET (COMMA (ID1 .. IDn)) */

string SyntaxNode::printDummyVar() const {
	if (opCode == ID) {
		return this->print();
	}
	else {
		SyntaxNode *list;
		// this must be LBRACKET
		if (opCode != LBRACKET) {
			cerr << "printDummyVar: dummy var must be ID or (ID1,..,IDn)\n";
			cerr << "current opCode is " + opCode;
			exit(1);
		}
		list = front();
		if (list->opCode == ID)
			return list->print();
		if (list->opCode != COMMA) {
			cerr << "printDummyVar: dummy var must be ID or (ID1,..,IDn)\n";
			cerr << "current opCode is " + list->opCode;
			exit(1);
		}
		return list->print();
	}
}

/* --------------------------------------------------------------------------
 SyntaxNode::findIDREF(list<ModelComp> *lmc)
 ---------------------------------------------------------------------------- */
/** Find all the IDREF nodes at or below the current node */
void SyntaxNode::findIDREF(list<ModelComp*>& lmc) const {

	if (opCode == ID)
		return;

	if (opCode == IDREF) {
		//printf("%s\n",getGlobalName((ModelComp*)this->values[0],
		//				NULL, NULL, NOARG));
		lmc.push_back(((SyntaxNodeIDREF*) this)->ref);
	}
	else
	{
		for(SyntaxNode::iterator i = begin();i != end();++i)
		{
			if (*i)
			{
				(*i)->findIDREF(lmc);

			}
		}
	}
}

/* --------------------------------------------------------------------------
 SyntaxNode::findIDREF(list<SyntaxNode *> *lnd)
 ---------------------------------------------------------------------------- */
/** Find all the IDREF nodes at or below the current node */
void SyntaxNode::findIDREF(list<SyntaxNode*> *lnd) {

	findOpCode(IDREF, lnd);
}

/* --------------------------------------------------------------------------
 SyntaxNode::findOpCode(int oc, list<SyntaxNode *> *lnd)
 ---------------------------------------------------------------------------- */
/** Find all nodes of opCode @a oc at or below the current node */
void SyntaxNode::findOpCode(int oc, list<SyntaxNode*> *lnd) {

	// if terminal then return
	if (opCode == ID)
		return;

	if (opCode == oc) {
		//printf("%s\n",getGlobalName((ModelComp*)this->values[0],
		//				NULL, NULL, NOARG));
		lnd->push_back(this);
	}
	else {
		for(SyntaxNode::iterator i = begin();i != end();++i)
			if (*i)
				(*i)->findOpCode(oc, lnd);
	}
}

/* --------------------------------------------------------------------------
 SyntaxNode::findModelComp()
 ---------------------------------------------------------------------------- */
/** Find the ModelComp (if it exists) referred to by this SyntaxNode.
 *
 *  @return The ModelComp only if the expression given by this SyntaxNode is
 *          an immediate reference to a ModelComp, otherwise NULL.
 */
ModelComp *SyntaxNode::findModelComp() const {
	const SyntaxNode *on = this;
	while((on->opCode == LBRACKET || on->opCode == LBRACE) && on->nchild() == 1) {
		on = on->values[0];
	}

	if (opCode == IDREF) {
		const SyntaxNodeIDREF *onref = dynamic_cast<const SyntaxNodeIDREF*>(this);
		return onref->ref;
	}
	return NULL;
}

/* --------------------------------------------------------------------------
 SyntaxNode::getArgumentList()
 ---------------------------------------------------------------------------- */
/** This is for a SyntaxNode of type IDREF (and should eventually be moved
 *  to SyntaxNodeIDREF:getArgumentList()).
 *
 *  @return A comma separated list of the arguments (the bit in [..] brackets).
 */
string SyntaxNode::getArgumentList() const {
	const SyntaxNodeIDREF *on;
	string arglist = "";
	if (getOpCode() != IDREF) {
		cerr << "Can only call getArgumentList for SyntaxNodes of type IDREF\n";
		exit(1);
	}

	// see if this is actually an IDREF node
	on = dynamic_cast<const SyntaxNodeIDREF*>(this);
	if (on == NULL) {
		cout << "WARNING: This is an IDREF SyntaxNode not of type SyntaxNodeIDREF\n";
		if (nchild() > 0) {
			arglist += values[1]->print();
			for(int i = 1;i < nchild();++i)
				arglist += "," + values[1 + i]->print();
		}
	}
	else {
		if (nchild() > 0) {
			SyntaxNode::iterator i = begin();
			arglist += (*i)->print();
			while(++i != end())
				arglist += "," + (*i)->print();
		}
	}
	return arglist;
}

/** Merges the values list of src into that of this object.
 *
 *  The items from src are prepended to this object's values.
 */
SyntaxNode* SyntaxNode::merge(const SyntaxNode *src) {

	int nval = values.size();
	int srcnval = src->nchild();
	values.resize(srcnval + nval);

	// copy this object's values to the end
	for(int i = srcnval + nval;--i > 0;)
		values[i] = values[i - srcnval];

	// copy src's values to the beginning
	for(int i = 0;i < srcnval;++i)
		values[i] = src->values[i];

	return this;
}

ostream& SyntaxNode::put(ostream&s) const {
	static int level = 0;

	if (this == NULL)
		return s;

	SyntaxNode::iterator i = this->begin();
	/*if(s!=cout) {
	 for(int j=0; j<level; ++j) cout << " ";
	 if(level!=0) cout << "-";
	 level++;
	 cout << "here " << this->getOpCode() << "(" << node << ")\n";
	 }*/

	switch (this->getOpCode())
	{
		case 0:
			s << **i;
			break;
			/* these are lots of simple binary operators */
		case ID:
			//s << (const char*)*i;               break;
			cerr << "ID put bad." << endl;
			throw exception();
			break;
		case ' ':
			if (this->nchild() > 1)
				s << **(i++);
			s << ' ' << **i;
			break;
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
		case LSBRACKET:
			if (this->nchild() == 1)
				s << "[" << **i << "]";
			else {
				s << **i << "[";
				s << **(++i) << "]";
			}
			break;
		case LBRACE:
			s << "{" << *i << "}";
			break;
		case LBRACKET:
			s << "(" << **i << ")";
			break;
		case COLON:
			if (this->nchild() > 1)
				s << **(i++);
			s << ": " << **i;
			break;
		case IF:
			s << "if " << **i;
			s << " then " << **(++i);
			if (this->nchild() == 3)
				s << " else " << **(++i);
			break;
		case IDREF:
		case IDREFM:
			const SyntaxNodeIDREF *onidref;
			if (!(onidref = (const SyntaxNodeIDREF*) (this))) {
				cerr << "Cast of node to SyntaxNodeIDREF failed!\n";
				exit(1);
			}
			s << onidref;
			break;
		case LOGICAL_AND:
			s << **i;
			s << " && ";
			s << **(++i);
			break;
		case NE:
			s << **i;
			s << " != ";
			s << **(++i);
			break;
		case -99: // template<class T> ValueNode
			cerr << "FAIL(-99)";
			throw exception();
			break;
		default:
			s << endl;
			cerr << "Unknown opcode " << this->getOpCode() << "\n";
			cerr << ".nval = " << this->nchild() << "\n";
			for(;i != this->end();++i) {
				cerr << "val[" << **i << "]\n";
			}
			throw exception();
			exit(1);
	}
	if (s != cout)
		level--;
	return s;
}

//Feng
int SyntaxNode::calculateVarDim() {
	LOG(this->print());
	int dim = 0;
	if (this->opCode == LBRACE) {
		dim = this->values[0]->calculateVarDim();
	}
	else if (this->opCode == COMMA) {
		LOG("calculateVarDim - ListNode size["<<this->values.size()<<"]");
		vector<SyntaxNode*>::iterator i;
		for(i = this->values.begin();i != this->values.end();i++) {
			dim += (*i)->calculateVarDim();
		}
	}
	else if (this->opCode == IDREF) {
		SyntaxNodeIDREF* nodeIdRef = static_cast<SyntaxNodeIDREF*>(this);
		assert(nodeIdRef->ref->type == TSET);
		dim = nodeIdRef->ref->getSetDim();
	}
	else {
		LOG("opCode["<<this->opCode<<"] was not yet implemented --- calculateVarDim");
		assert(false);
	}
	return dim;
}

int SyntaxNode::calculateSetDim() {
	LOG(this->print());
	int dim = 0;
	if (this->opCode == DEFINED) { //OpNode
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
		dim = nodeIdRef->ref->getSetDim();
	}
	else if (this->opCode == ID) { //IDNode
		dim = 1;
	}
	else if (this->opCode == LBRACE) { //ListSet  a,b,c,d
		LOG("size -- "<<this->values.size());
		dim = this->values[0]->calculateSetDim();
		assert(this->values.size() == 1);
	}
	else if (this->opCode == COMMA) { //ListNode
		dim = this->values[0]->calculateSetDim();
		dim += this->values[1] != NULL ? this->values[1]->calculateSetDim() : 0;
		dim += this->values[2] != NULL ? this->values[2]->calculateSetDim() : 0;
		dim += this->values[3] != NULL ? this->values[3]->calculateSetDim() : 0;
		assert(this->values[4]==NULL);
	}
	else if (this->opCode == DOTDOT) { //SimpleSet
		dim = 1;
	}
	else if (this->opCode == ORDERED) { //ordered property -- will not use without ampl
		dim = 1;
	}
	else if (this->opCode == 32) {
		dim = this->values[0]->calculateSetDim();
	}
	else if (this->opCode == COLON) {
		dim = this->values[0]->calculateSetDim();
	}
	else if (this->opCode == IN) { //id ref at second elements
		dim = this->values[1]->calculateSetDim();
	}
	else {
		LOG("opCode["<<this->opCode<<"] was not yet implemented --- calculateSetDim");
		assert(false);
	}

	return dim;
}

void SyntaxNode::calculateParamIndicies(vector<string>& paramIndiciesDummy, vector<ModelComp*>& paramIndiciesComp, hash_map<string, ModelComp*>& paramIndiciesMap) {
	LOG("opCode["<<this->opCode<<"]  "<<this->print());
	if (this->opCode == IDREF) {
		SyntaxNodeIDREF* refn = static_cast<SyntaxNodeIDREF*>(this);
		assert(refn->ref->type == TSET);
		string dummy = DummyVariableGenerator::nextDummy();
		paramIndiciesDummy.push_back(dummy);
		paramIndiciesComp.push_back(refn->ref);
		paramIndiciesMap.insert(pair<string, ModelComp*>(dummy, refn->ref));
	}
	else if (this->opCode == LBRACE) {
		LOG("this ListSet size["<<this->values.size()<<"]");
		assert(this->values.size() == 1);
		this->values[0]->calculateParamIndicies(paramIndiciesDummy, paramIndiciesComp, paramIndiciesMap);
	}
	else if (this->opCode == COMMA) {
		LOG("this ListNode size["<<this->values.size()<<"]");
		vector<SyntaxNode*>::iterator i;
		for(i = this->values.begin();i != this->values.end();i++) {
			(*i)->calculateParamIndicies(paramIndiciesDummy, paramIndiciesComp, paramIndiciesMap);
		}
	}
	else if (this->opCode == IN) {
		IDNode* idn = static_cast<IDNode*>(this->values[0]);
		SyntaxNodeIDREF* refn = static_cast<SyntaxNodeIDREF*>(this->values[1]);
		assert(refn->ref->type == TSET);
		assert(idn!=NULL && refn!=NULL);
		paramIndiciesDummy.push_back(idn->id());
		paramIndiciesComp.push_back(refn->ref);
		paramIndiciesMap.insert(pair<string, ModelComp*>(idn->id(), refn->ref));
		LOG("add parameter indices ["<<idn->id()<<"]--->["<<refn->ref->id<<"]");
		assert(this->values.size() == 2);
		//should have no more
	}
	else {
		LOG("opCode["<<this->opCode<<"] was not yet implemented -- calculateNumParamIndicies");
		assert(false);
	}
}

Set* SyntaxNode::calculateSetValue(ModelContext* context) {
	Set* value = NULL;
	LOG(this->print());
	if (this->opCode == DEFINED) {
		value = this->values[0]->calculateSetValue(context);
	}
	else if (this->opCode == CROSS) {
		Set* left = this->values[0]->calculateSetValue(context);
		Set* right = this->values[1]->calculateSetValue(context);
		if (left != NULL && right != NULL) {
			value = left->setCross(right);
		}
		delete left;
		delete right;
	}
	else if (this->opCode == DIFF) {
		Set* left = this->values[0]->calculateSetValue(context);
		Set* right = this->values[1]->calculateSetValue(context);
		if (left != NULL && right != NULL) {
			value = left->setDiff(right);
		}
		delete left;
		delete right;
	}
	else if (this->opCode == LBRACE) {
		assert(this->values.size() == 1);
		value = this->values[0]->calculateSetValue(context);
	}
	else if (this->opCode == COMMA) {
		assert(this->values.size() == 1);
		//so far, the list noly allow 1 in size
		value = this->values[0]->calculateSetValue(context);
	}
	else if (this->opCode == IDREF) {
		SyntaxNodeIDREF* nodeIdRef = static_cast<SyntaxNodeIDREF*>(this);
		assert(nodeIdRef->ref!=NULL);
		if (nodeIdRef->ref->type == TSET) {
			ModelComp* comp = nodeIdRef->ref;
			Set* setValRef = static_cast<Set*>(context->getCompValue(comp));
			value = new Set(*setValRef);
		}
		else if (nodeIdRef->ref->type == TPARAM) {
			Param* aParam = static_cast<Param*>(context->getCompValue(nodeIdRef->ref));
			assert(aParam->card == 1);
			int i = aParam->paramValues[""]->value;
			value = new Set(1, "TMP");
			ostringstream oss;
			oss << i;
			SetValue* val = new SetValue(oss.str());
			value->addSetValue(val);
		}
		else {
			LOG("type ["<<nodeIdRef->ref->type<<"not suppose to be here!");
			assert(false);
		}
	}
	else if (this->opCode == 0) {
		value = this->values[0]->calculateSetValue(context);
	}
	else if (this->opCode == ID) {
		LOG(" id node here ");
		IDNode* idn = static_cast<IDNode*>(this);
		value = new Set(1, "DUM");
		string dummy = idn->id();
		SetValue* val = new SetValue(context->getDummyValue(dummy));
		value->addSetValue(val);
	}
	else if (this->opCode == 32) {
		LOG(this->values[0]);
		value = this->values[1]->calculateSetValue(context);
	}
	else if (this->opCode == DOTDOT) {
		LOG(this->values.size());
		int start;
		int end;
		if (this->values[0]->opCode == -99) {
			LOG(this->values[0]->opCode);
			ValueNode* valn = static_cast<ValueNode*>(this->values[0]);
			start = valn->value;
			LOG("set starting ["<<start<<"]");
		}

		if (this->values[1]->opCode == -99) {
			LOG(this->values[1]->opCode);
			ValueNode* valn = static_cast<ValueNode*>(this->values[1]);
			end = valn->value;
			LOG("set ending ["<<end<<"]");
		}
		else if (this->values[1]->values[0]->opCode == IDREF) {
			SyntaxNodeIDREF* refn = static_cast<SyntaxNodeIDREF*>(this->values[1]->values[0]);
			assert(refn->ref->type == TPARAM);
			Param* aParam = static_cast<Param*>(context->getCompValue(refn->ref));
			assert(aParam->card == 1);
			end = aParam->paramValues[""]->value;
			LOG("set end ["<<end<<"]");
		}
		else {
			LOG("case not yet implmented.");
			assert(false);
		}

		value = new Set(1, "TMP");
		for(int i = start;i <= end;i++) {
			ostringstream oss;
			oss << i;
			SetValue* val = new SetValue(oss.str());
			value->addSetValue(val);
		}

	}
	else if (this->opCode == -99) {
		ValueNode* valn = static_cast<ValueNode*>(this);
		ostringstream oss;
		int i = valn->value;
		oss << i;
		SetValue* val = new SetValue(oss.str());
		value = new Set(1, "TMP");
		value->addSetValue(val);
	}
	else if (this->opCode == COLON) {
		value = this->evalSet(context);
	}
	else {
		LOG("calculateSetValue -- op not yet implmented -- opCode["<<this->opCode<<"]");
		assert(false);
	}
	return value;
}

Set* SyntaxNode::evalSet(ModelContext* context) {
	LOG("evalSet -- "<<this->print());
	Set* value = new Set(1, "TMP");
	assert(this->opCode == COLON);
	assert(this->values[0]->opCode == IN);
	assert(this->values[0]->values[0]->opCode == ID);

	IDNode* idn = static_cast<IDNode*>(this->values[0]->values[0]);
	string dummy = idn->id();
	LOG("dummy index name["<<dummy<<"]");

	assert(this->values[0]->values[1]->opCode == IDREF);
	SyntaxNodeIDREF* refn = static_cast<SyntaxNodeIDREF*>(this->values[0]->values[1]);
	LOG("ModelComp is ["<<refn->ref->id<<"]");
	Set* aSet = static_cast<Set*>(context->getCompValue(refn->ref));
	vector<SetValue*>::iterator it = aSet->setValues_data_order.begin();
	for(;it != aSet->setValues_data_order.end();it++) {
		assert((*it)->valueList.size() == 1);
		string val = (*it)->valueList.front();
		LOG("dummy val["<<val<<"]");
		context->addDummySetValueMapTemp(dummy, refn->ref, val);
		if (this->values[1]->evalBool(context)) {
			SetValue* setVal = new SetValue(val);
			value->addSetValue(setVal);
		}
		context->removeDummySetValueMapTemp(dummy);
	}
	return value;
}

double SyntaxNode::calculateParamValue(hash_map<string, string>& dummyValueMap, hash_map<string, ModelComp*>& paramIndicies, ModelContext* context) {
	LOG("calculateParamValue  ---  opCode["<<this->opCode<<"]  "<<this->print());
	double rval = 0;
	if (this->opCode == DEFINED) {
		rval = this->values[0]->calculateParamValue(dummyValueMap, paramIndicies, context);
	}
	else if (this->opCode == COMMA) {
		rval = this->values[0]->calculateParamValue(dummyValueMap, paramIndicies, context);
	}
	else if (this->opCode == -99) {
		ValueNode* vnode = static_cast<ValueNode*>(this);
		rval = vnode->value;
	}
	else if (this->opCode == IF) {
		bool cond = this->values[0]->evalBool(dummyValueMap, paramIndicies, context);
		rval = cond == true ? this->values[1]->calculateParamValue(dummyValueMap, paramIndicies, context) : this->values[2]->calculateParamValue(dummyValueMap, paramIndicies, context);
	}
	else if (this->opCode == 45) {
		if (this->values.size() == 1) {
			rval = -this->values[0]->calculateParamValue(dummyValueMap, paramIndicies, context);
		}
		else if (this->values.size() == 2) {
			double left = this->values[0]->calculateParamValue(dummyValueMap, paramIndicies, context);
			double right = this->values[1]->calculateParamValue(dummyValueMap, paramIndicies, context);
			rval = left - right;
		}
		else {
			LOG("can't have more than two for '-' operator");
			assert(false);
		}
	}
	else if (this->opCode == IDREF) {
		rval = this->evalTerm(dummyValueMap, paramIndicies, context);
	}
	else if (this->opCode == 0) {
		rval = this->evalTerm(dummyValueMap, paramIndicies, context);
	}
	else if (this->opCode == SUM) {
		assert(this->values[0]->values[0]->values[0]->opCode == ID);
		assert(this->values[0]->values[0]->values[1]->opCode == IDREF);
		IDNode* idNode = static_cast<IDNode*>(this->values[0]->values[0]->values[0]);
		SyntaxNodeIDREF* refNode = static_cast<SyntaxNodeIDREF*>(this->values[0]->values[0]->values[1]);
		string dummy = idNode->id();
		Set* aSet = static_cast<Set*>(context->getCompValue(refNode->ref));
		vector<SetValue*>::iterator it = aSet->setValues_data_order.begin();
		double sumVal = 0;
		for(;it != aSet->setValues_data_order.end();it++) {
			assert((*it)->valueList.size() == 1);
			string value = (*it)->valueList.front();
			context->addDummySetValueMapTemp(dummy, refNode->ref, value);
			sumVal += this->values[1]->evalTerm(dummyValueMap, paramIndicies, context);
			context->removeDummySetValueMapTemp(dummy);
		}
		rval = sumVal;
	}
	else if (this->opCode == CARD) {
		assert(this->values.size() == 1);
		assert(this->values[0]->values.size() == 1);
		assert(this->values[0]->values[0]->values[0]->values[0]->opCode == IDREF);
		SyntaxNodeIDREF* refNode = static_cast<SyntaxNodeIDREF*>(this->values[0]->values[0]->values[0]->values[0]);
		Set* aSet = static_cast<Set*>(context->getCompValue(refNode->ref));
		rval = (double) aSet->getCard();
	}
	else if (this->opCode == 47) //'/' operator
			{
		double numerator = this->values[0]->calculateParamValue(dummyValueMap, paramIndicies, context);
		double denominator = this->values[1]->calculateParamValue(dummyValueMap, paramIndicies, context);
		rval = numerator / denominator;
	}
	else {
		LOG("calculateParamValue ---  opCode["<<this->opCode<<"] not yet implemented");
		assert(false);
	}
	return rval;
}

bool SyntaxNode::evalBool(hash_map<string, string>& dummyValueMap, hash_map<string, ModelComp*>& paramIndicies, ModelContext* context) {
	LOG("evalBool  ---  opCode["<<this->opCode<<"]  "<<this->print());
	bool rval = false;
	if (this->opCode == EQ) {
		double r0 = this->values[0]->evalTerm(dummyValueMap, paramIndicies, context);
		LOG("--- value for r0["<<r0<<"]");
		double r1 = this->values[1]->evalTerm(dummyValueMap, paramIndicies, context);
		LOG("--- value for r1["<<r1<<"]");
		if (r0 == r1) {
			rval = true;
		}
		else {
			rval = false;
		}
	}
	else if (this->opCode == GE) {
		assert(false);
		LOG("evalBool -- opCode[GE] not yet implmemented ");
	}
	else if (this->opCode == LE) {
		assert(false);
		LOG("evalBool -- opCode[LE] not yet implmemented ");
	}
	else if (this->opCode == LT) {
		assert(false);
		LOG("evalBool -- opCode[LT] not yet implmemented ");
	}
	else if (this->opCode == GT) {
		LOG("evalBool -- opCode[GT] not yet implmemented ");
		assert(false);
	}
	else if (this->opCode == LBRACKET) {
		rval = this->values[0]->evalBool(dummyValueMap, paramIndicies, context);
	}
	return rval;
}

double SyntaxNode::evalTerm(hash_map<string, string>& dummyValueMap, hash_map<string, ModelComp*>& paramIndicies, ModelContext* context) {
	LOG("evalTerm  ---  opCode["<<this->opCode<<"]  "<<this->print());
	double rval = 0;
	if (this->opCode == IDREF) {
		assert(this->values.size() <= 2);
		SyntaxNodeIDREF* refn = static_cast<SyntaxNodeIDREF*>(this);
		assert(refn->ref->type == TPARAM);
		ModelComp* comp = refn->ref;
		Param* theParam = static_cast<Param*>(context->getCompValue(comp));
		vector<SyntaxNode*>::iterator it = this->values.begin();
		ostringstream key(ostringstream::out);
		for(;it != this->values.end();it++) {
			IDNode* ind = static_cast<IDNode*>((*it)->values[0]);
			assert(ind->opCode == ID);
			string dummy = ind->id();
			LOG("  ----  dummy name is ["<<dummy<<"]");
			if (dummyValueMap.find(dummy) == dummyValueMap.end()) {
				key << (context->getDummyValueTemp(dummy));
			}
			else {
				key << (dummyValueMap.find(dummy)->second);
			}
			LOG("  -----  key is ["<<key.str()<<"]");
		}
		rval = theParam->findParamValue(key.str());
	}
	else if (this->opCode == ORD) {
		assert(this->values[0]->opCode == LBRACKET);
		assert(this->values[0]->values[0]->opCode==COMMA);
		assert(this->values[0]->values[0]->values[0]->opCode == 0);
		assert(this->values[0]->values[0]->values[0]->values[0]->opCode==ID);
		IDNode* in = static_cast<IDNode*>(this->values[0]->values[0]->values[0]->values[0]);
		string dummy = in->id();
		ModelComp* comp = paramIndicies.find(dummy)->second;
		Set* theSet = static_cast<Set*>(context->getCompValue(comp));
		rval = (double) (theSet->getSetOrder(dummyValueMap.find(dummy)->second));
	}
	else if (this->opCode == 42) {
		assert(this->values.size() == 2);
		double left = this->values[0]->calculateParamValue(dummyValueMap, paramIndicies, context);
		double right = this->values[1]->calculateParamValue(dummyValueMap, paramIndicies, context);
		rval = left * right;
	}
	else if (this->opCode == 0) {
		rval = this->values[0]->evalTerm(dummyValueMap, paramIndicies, context);
	}
	else {
		LOG("evalTerm -- opCode["<<this->opCode<<"] not yet implmemented ");
		assert(false);
	}
	return rval;
}

void SyntaxNode::foreachSetValue(vector<ModelComp*> comps, vector<string>& dummyVars, Set* aSet, ModelContext* rowContext, vector<double>& jcobs, ModelContext* colContext) {
	vector<SetValue*>::iterator it_setval = aSet->setValues_data_order.begin();
	for(;it_setval != aSet->setValues_data_order.end();it_setval++) {
		SetValue* sval = *it_setval;
		assert(sval->valueList.size() == comps.size() && comps.size() == dummyVars.size());
		vector<string>::iterator it_sval = sval->valueList.begin();
		vector<ModelComp*>::iterator it_comp = comps.begin();
		vector<string>::iterator it_dummyVar = dummyVars.begin();
		for(;it_comp != comps.end();it_comp++, it_dummyVar++, it_sval++) {
			rowContext->addDummySetValueMapTemp((*it_dummyVar), *it_comp, *it_sval);
		}

		vector<double> currJcobs(jcobs.size(), 0);
		this->values[1]->evalDiff(rowContext, colContext, currJcobs);
		assert(currJcobs.size() == jcobs.size());
		this->plusVector(jcobs, currJcobs, jcobs);

		it_dummyVar = dummyVars.begin();
		for(;it_dummyVar != dummyVars.end();it_dummyVar++) {
			rowContext->removeDummySetValueMapTemp(*it_dummyVar);
		}
	}
}

void SyntaxNode::findSyntaxNodeChild(SyntaxNode** node, int op) {
	if (this->opCode == op) {
		*node = this;
		return;
	}
	if (this->values[0] != NULL) {
		this->values[0]->findSyntaxNodeChild(node, op);
	}
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

void SyntaxNode::handleSum(ModelContext* rowContext, vector<double>& jcobs, ModelContext* colContext) {
//	vector<ModelComp*> comps;
//	Set* aSet = NULL;
//	vector<string> dummyVars;
//	ostringstream oss;
//	oss //<< rowContext->getModelDummyValAsKey() //don't need this one since context is hirechical structured! -Feng
//			<< rowContext->getConsDummyValAsKey()
//			<< (void*) ((this->values[0]));
//	string hashKey = oss.str();
//	if (!rowContext->getCalcTempSet(hashKey, comps, &aSet, dummyVars))
//	{
//		this->values[0]->calcTempSetComp(rowContext, comps, &aSet, dummyVars);
//		LOG("sum { dummyVar } over "<<aSet->toString());
//		LOG(
//				"-- Add Temp Set - hashKey["<<hashKey<<"] overSet "<<aSet->toString());
//		rowContext->addCalcTempSet(hashKey, comps, aSet, dummyVars);
//	}
//	foreachSetValue(comps, dummyVars, aSet, rowContext, jcobs, colContext);

	ExpandedModel2* rowEm2 = static_cast<ExpandedModel2*>(rowContext->em);
	ExpandedModel2* colEm2 = static_cast<ExpandedModel2*>(colContext->em);

	SyntaxNode* cons_sum_node = NULL;
	this->findSyntaxNodeChild(&cons_sum_node, IN);
	SyntaxNode* col_index_node = NULL;
	if (colEm2->model->node != NULL && colEm2->model->node->indexing != NULL) {
		colEm2->model->node->indexing->findSyntaxNodeChild(&col_index_node, IN);
	}

	SyntaxNodeIDREF* consSumRef = static_cast<SyntaxNodeIDREF*>(cons_sum_node->values[1]);
	if (col_index_node != NULL && consSumRef->ref == static_cast<SyntaxNodeIDREF*>(col_index_node->values[1])->ref) {
		LOG("EQUAL - CONS SUM Indexing["<<cons_sum_node->print()<<"] Col Model Indexing["<<col_index_node->print()<<"]");
		string dummyVar = (colContext->dummySetMap.begin())->first;
		rowContext->addDummySetValueMapTemp(dummyVar, (colContext->dummySetMap.begin())->second, (colContext->dummyValueMap.begin())->second);
		this->values[1]->evalDiff(rowContext, colContext, jcobs);
		rowContext->removeDummySetValueMapTemp(dummyVar);
	}
	else {
		LOG("NOT - CONS SUM Indexing["<<cons_sum_node->print()<<"] Col Model Indexing["<<col_index_node->print()<<"]");
		if (this->values[1]->isDepend(colEm2->localVarComps)) {
			vector<ModelComp*> comps;
			Set* aSet = NULL;
			vector<string> dummyVars;
			ostringstream oss;
			oss //<< rowContext->getModelDummyValAsKey() //don't need this one since context is hirechical structured! -Feng
			<< rowContext->getConsDummyValAsKey() << (void*) ((this->values[0])) << colContext;
			string hashKey = oss.str();
			if (!rowContext->getCalcTempSet(hashKey, comps, &aSet, dummyVars)) {
				this->values[0]->calcTempSetComp(rowContext, comps, &aSet, dummyVars);
				LOG("sum { dummyVar } over "<<aSet->toString());
				LOG("-- Add Temp Set - hashKey["<<hashKey<<"] overSet "<<aSet->toString());
				rowContext->addCalcTempSet(hashKey, comps, aSet, dummyVars);
			}
			vector<SetValue*>::iterator it_setval = aSet->setValues_data_order.begin();
			for(;it_setval != aSet->setValues_data_order.end();it_setval++) {
				SetValue* sval = *it_setval;
				assert(sval->valueList.size() == comps.size() && comps.size() == dummyVars.size());
				vector<string>::iterator it_sval = sval->valueList.begin();
				vector<ModelComp*>::iterator it_comp = comps.begin();
				vector<string>::iterator it_dummyVar = dummyVars.begin();
				for(;it_comp != comps.end();it_comp++, it_dummyVar++, it_sval++) {
					rowContext->addDummySetValueMapTemp((*it_dummyVar), *it_comp, *it_sval);
				}

				vector<double> currJcobs(jcobs.size(), 0);
				this->values[1]->evalDiff(rowContext, colContext, currJcobs);
				assert(currJcobs.size() == jcobs.size());
				this->plusVector(jcobs, currJcobs, jcobs);

				it_dummyVar = dummyVars.begin();
				for(;it_dummyVar != dummyVars.end();it_dummyVar++) {
					rowContext->removeDummySetValueMapTemp(*it_dummyVar);
				}
			}
		}
		else {
			LOG("["<<this->values[1]->print()<<"] not depend on ["<<colEm2->model->name<<"]'s localvar");
		}
	}
}

void SyntaxNode::getVarKey(string& varKey, SyntaxNodeIDREF* refn, ModelContext* rowContext) {
	ModelComp* comp = refn->ref;
	int numIndicies = comp->getNumVarIndicies();
	string last_key = "";
	LOG("num indicies for var["<<comp->id<<"] is "<<numIndicies);
	for(int i = 0;i < refn->values.size();i++) {
		//hack!
		IDNode* ind;
		if (refn->values[i]->values.size() != 0) {
			//for msnd model, extra comma opcode is at refn->values[i]
			ind = static_cast<IDNode*>(refn->values[i]->values[0]);
		}
		else {	//for alm model, id node just follows
			ind = static_cast<IDNode*>(refn->values[i]);
		}

		string dummy = ind->id();
		string dummyval = rowContext->getDummyValue(dummy);
		if (!dummyval.empty()) {
			//for multi-dimensional variable declaration!
			last_key += dummyval;
			numIndicies--;
		}
	}
	varKey = rowContext->getModelDummyValAsKey(numIndicies) + last_key;

}

void SyntaxNode::evalDiff(ModelContext* rowContext, ModelContext* colContext, vector<double>& jcobs) {
	LOG("evalDiff --- opCode["<<this->opCode<<"] -- jcobs.size["<<jcobs.size()<<"] -- "<<this->print());
	if (this->opCode == ASSIGN) {
		assert(this->values.size() == 2);
		this->values[0]->evalDiff(rowContext, colContext, jcobs);
		//right hand side ignored for diff calculation
	}
	else if (this->opCode == SUM) {
		handleSum(rowContext, jcobs, colContext);
	}
	else if (this->opCode == '-') {
		if (values.size() == 1) {
			this->values[0]->evalDiff(rowContext, colContext, jcobs);
			this->negateVector(jcobs);
		}
		else {
			assert(this->values.size() == 2);
			vector<double> rightJcobs(jcobs.size(), 0);
			vector<double> leftJcobs(jcobs.size(), 0);
			this->values[0]->evalDiff(rowContext, colContext, leftJcobs);
			this->values[1]->evalDiff(rowContext, colContext, rightJcobs);
			assert(jcobs.size() == leftJcobs.size() && leftJcobs.size() == rightJcobs.size());
			this->minusVector(leftJcobs, rightJcobs, jcobs);
		}
	}
	else if (this->opCode == '+') {
		if (values.size() == 1) {
			this->values[0]->evalDiff(rowContext, colContext, jcobs);
		}
		else {
			assert(this->values.size() == 2);
			vector<double> leftJcobs(jcobs.size(), 0);
			vector<double> rightJcobs(jcobs.size(), 0);
			this->values[0]->evalDiff(rowContext, colContext, leftJcobs);
			this->values[1]->evalDiff(rowContext, colContext, rightJcobs);
			assert(jcobs.size() == leftJcobs.size() && leftJcobs.size() == rightJcobs.size());
			this->plusVector(leftJcobs, rightJcobs, jcobs);
		}
	}
	else if (this->opCode == 0) {
		this->values[0]->evalDiff(rowContext, colContext, jcobs);
	}
	else if (this->opCode == IDREF) {
		SyntaxNodeIDREF* refn = static_cast<SyntaxNodeIDREF*>(this);
		ModelComp* comp = refn->ref;
		if (comp->type == TPARAM) {
			LOG("got a parameter type rval = 0");
		}
		else if (comp->type == TVAR) {
			ExpandedModel2* em2 = static_cast<ExpandedModel2*>(colContext->em);
			vector<Var*>::iterator it_var = em2->localVars.begin();
			vector<ModelComp*>::iterator it_mc = em2->localVarComps.begin();
			vector<double>::iterator it_jcobs = jcobs.begin();
			assert(jcobs.size() == em2->numLocalVars);
			string currVarKey = "";
			bool currVarKeyReady = false;
			bool found = false;
			while(!found && it_jcobs != jcobs.end()) {
				ModelComp* varComp = *it_mc;
				if (varComp == comp) {
					if (!currVarKeyReady) {
						LOG("["<<varComp->id<<"] matches - now calculate varKey");
						this->getVarKey(currVarKey, refn, rowContext);
						currVarKeyReady = true;
						LOG("current variable key - var_key["<<currVarKey<<"]");
					}

					//now checking varKey in Var set if matches any
					vector<VarValue*>::iterator it2 = (*it_var)->varList.begin();

					while(!found && it2 != (*it_var)->varList.end()) {
						string& varKey = (*it2)->varKey;
						if (varKey.compare(currVarKey) == 0) {
							*it_jcobs = 1;
							found = true;
							LOG("["<<varComp->id<<"] matches ["<<varKey<<"] matches ");
						}
						else {
							LOG("["<<varComp->id<<"] matches ["<<varKey<<"] not match ");
							*it_jcobs = 0;
						}
						it_jcobs++;
						it2++;
					}
				}
				else {
					LOG("["<<varComp->id<<"] not match - go to next varComp -- card["<<(*it_var)->card<<"]");
					for(int j = 0;j < (*it_var)->card;j++, it_jcobs++) {
						*it_jcobs = 0;
					}
				}
				//increment to next varComp
				it_var++;
				it_mc++;
			}
			while(it_jcobs != jcobs.end()) {
				*it_jcobs = 0;
				it_jcobs++;
			}
		}
	}
	else if (this->opCode == 42) //42 is times -- handles only linear case
			{
		assert(this->values.size() == 2);
		vector<double> leftJcobs(jcobs.size(), 0);
		vector<double> rightJcobs(jcobs.size(), 0);
		this->values[0]->evalDiff(rowContext, colContext, leftJcobs);
		this->values[1]->evalDiff(rowContext, colContext, rightJcobs);
		assert(jcobs.size() == leftJcobs.size() && leftJcobs.size() == rightJcobs.size());
		assert(this->isZeroVector(leftJcobs) || this->isZeroVector(rightJcobs));
		if (this->isZeroVector(leftJcobs) && !this->isZeroVector(rightJcobs)) {
			double val = this->values[0]->evalTerm(rowContext);
			this->multVectorScalar(rightJcobs, val, jcobs);
		}
		else if (!this->isZeroVector(leftJcobs) && this->isZeroVector(rightJcobs)) {
			double val = this->values[1]->evalTerm(rowContext);
			this->multVectorScalar(leftJcobs, val, jcobs);
		}
	}
	else if (this->opCode == LBRACKET) {
		this->values[0]->evalDiff(rowContext, colContext, jcobs);
	}
	else if (this->opCode == COMMA) {
		this->values[0]->evalDiff(rowContext, colContext, jcobs);
	}
	else if (this->opCode == -99) { //a value Node

	}
	else {
		LOG(" -- opCode["<<opCode<<"] is not implmeneted yet for automatic differentiation!");
		assert(false);
	}
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

void SyntaxNode::negateVector(vector<double>& v) {
	LOG("negateVector - "<<printVector(v));
	vector<double>::iterator it1 = v.begin();
	for(;it1 != v.end();it1++) {
		(*it1) = -(*it1);
	}
	LOG("end negateVector - "<<printVector(v));
}

bool SyntaxNode::isZeroVector(vector<double>& v) {
	double sum = 0;
	for(vector<double>::iterator it = v.begin();it != v.end();it++) {
		sum += *it;
	}
	bool rval = sum == 0.0;
	LOG("isZeroVector - "<<printVector(v)<<" -- "<<rval);
	return rval;
}

void SyntaxNode::multVectorScalar(vector<double>& v, double scalar, vector<double>& result) {
	LOG("multVectorScalar - "<<printVector(v)<<" - scalar["<<scalar<<"]");
	vector<double>::iterator it1 = v.begin();
	vector<double>::iterator it3 = result.begin();
	for(;it1 != v.end();it1++, it3++) {
		(*it3) = (*it1) * scalar;
	}
	LOG("multVectorScalar - "<<printVector(result)<<"");
}

void SyntaxNode::plusVector(vector<double>& left, vector<double>& right, vector<double>& result) {
	LOG("plusVector - "<<printVector(left)<<"");
	LOG("plusVector - "<<printVector(right)<<"");
	vector<double>::iterator it1 = left.begin();
	vector<double>::iterator it2 = right.begin();
	vector<double>::iterator it3 = result.begin();
	for(;it1 != left.end();it1++, it2++, it3++) {
		(*it3) = (*it1) + (*it2);
	}
	LOG("plusVector - "<<printVector(result)<<"");
}
void SyntaxNode::minusVector(vector<double>& left, vector<double>& right, vector<double>& result) {
	LOG("minusVector - "<<printVector(left)<<"");
	LOG("minusVector - "<<printVector(right)<<"");
	vector<double>::iterator it1 = left.begin();
	vector<double>::iterator it2 = right.begin();
	vector<double>::iterator it3 = result.begin();
	for(;it1 != left.end();it1++, it2++, it3++) {
		(*it3) = (*it1) - (*it2);
	}
	LOG("minusVector - "<<printVector(result)<<"");
}

double SyntaxNode::evalRhs(ModelContext* context) {
	double rval = 0;
	if (this->opCode == ASSIGN) {
		rval = this->values[1]->evalTerm(context); //ingore right hand side
	}
	else {
		LOG("Right-hand-side expression not yet supported! "<<this->print());
		assert(false);
	}

	return rval;
}

void SyntaxNode::calcTempSetComp(ModelContext* context, vector<ModelComp*>& comps, Set** theSet, vector<string>& dummyNames) {
	LOG("calcTempSetComp --- opCode["<<this->opCode<<"]  "<<this->print());
	if (this->opCode == LBRACE) {
		assert(this->values.size() == 1);
		this->values[0]->calcTempSetComp(context, comps, theSet, dummyNames);
	}
	else if (this->opCode == IN) //the simple only one dimension case.
	{
		string dummyName = static_cast<IDNode*>(this->values[0])->id();
		ModelComp* aComp = static_cast<SyntaxNodeIDREF*>(this->values[1])->ref;
		Set* aSet = NULL;
		Set* setCopy = NULL;
		assert(this->values[1]->values.size() == 0);
		aSet = static_cast<Set*>(context->getCompValue(aComp));

		//creating set copy
//		setCopy = new Set(*aSet);
//		*theSet = setCopy;
		//non copy version
		*theSet = aSet;

		assert((*theSet)->dim == 1);
		comps.push_back(aComp);
		dummyNames.push_back(dummyName);

	}
	else if (this->opCode == COLON) {
		SyntaxNode* val0 = this->values[0];
		string dummyName = static_cast<IDNode*>(val0->values[0])->id();
		ModelComp* aComp = static_cast<SyntaxNodeIDREF*>(val0->values[1])->ref;

		Set* aSet = static_cast<Set*>(context->getCompValue(aComp));
		Set* newSet = new Set(aSet->dim, "TMP");

		vector<SetValue*>::iterator it = aSet->setValues_data_order.begin();
		for(;it != aSet->setValues_data_order.end();it++) {
			string dummyVal = (*it)->getKey();
			context->addDummySetValueMapTemp(dummyName, aComp, dummyVal);
			bool isIn = this->values[1]->evalBool(context);
			if (isIn) {
				LOG("add ["<<dummyVal<<"] to temp set");
				newSet->addSetValue(new SetValue(dummyVal));
			}
			else {
				LOG("   ["<<dummyVal<<"] is not in temp set");
			}
			context->removeDummySetValueMapTemp(dummyName);
		}
		*theSet = newSet;
		comps.push_back(aComp);
		dummyNames.push_back(dummyName);
	}
	else if (this->opCode == COMMA) {
		assert(this->values.size() <= 3 && this->values.size() != 0);

		ExpandedModel2* em2 = static_cast<ExpandedModel2*>(context->em);
		vector<ExpandedModel2*> em2LevelList;
		em2->levelTraversal(em2LevelList, context->moveUpLevel);
		for(vector<SyntaxNode*>::iterator it = this->values.begin();it != this->values.end();it++) {
			LOG("COMMA List -- handling ["<<(*it)->print()<<"]");
			(*it)->calculateCompsDummyNames(context, comps, dummyNames);
		}
		assert(comps.size() == this->values.size() && comps.size() == dummyNames.size());

		*theSet = new Set(this->values.size(), "TMP_UP" + context->moveUpLevel);
		for(vector<ExpandedModel2*>::iterator em2LevelIt = em2LevelList.begin();em2LevelIt != em2LevelList.end();em2LevelIt++) {
			this->calculateSet((*em2LevelIt)->context, comps, dummyNames, *theSet);
		}
	}
	else {
		LOG("calcTempSetComp -- opCode not yet implemented ..");
		assert(false);
		//opcode not yet implemented
	}
}

void SyntaxNode::calculateCompsDummyNames(ModelContext* context, vector<ModelComp*>&comps, vector<string>& dummyNames) {
	string dummyName = static_cast<IDNode*>(this->values[0])->id();
	ModelComp* aComp = static_cast<SyntaxNodeIDREF*>(this->values[1])->ref;
	comps.push_back(aComp);
	dummyNames.push_back(dummyName);
	LOG("calculateCompsDummyNames -- adding ModelComp["<<aComp->id<<"] dummyName["<<dummyName<<"]");
}

void SyntaxNode::calculateSet(ModelContext* context, vector<ModelComp*>& comps, vector<string>& dummyNames, Set* theSet) {
	LOG("calculateSet --- opCode["<<this->opCode<<"]  "<<this->print());
	assert(this->opCode == COMMA);
	vector<string> valueList;
	for(vector<string>::iterator it = dummyNames.begin();it != dummyNames.end();it++) {
		string dummyName = *it;
		string dummyVal = context->getDummyValue(dummyName);
		LOG("calculateSet -- dummyName["<<dummyName<<"]  --->  dummyVal["<<dummyVal<<"]");
		valueList.push_back(dummyVal);
	}
	theSet->addSetValue(new SetValue(valueList));
}

bool SyntaxNode::evalBool(ModelContext* context) {
	LOG("evalBool - single - opCode["<<this->opCode<<"] "<<this->print());
	bool rval = false;
	if (this->opCode == EQ) {
		double r0 = this->values[0]->evalTerm(context);
		LOG("--- value for r0["<<r0<<"]");
		double r1 = this->values[1]->evalTerm(context);
		LOG("--- value for r1["<<r1<<"]");
		if (r0 - r1 == 0) {
			rval = true;
		}
		else {
			rval = false;
		}
	}
	else if (this->opCode == NE) {
		double r0 = this->values[0]->evalTerm(context);
		LOG("--- value for r0["<<r0<<"]");
		double r1 = this->values[1]->evalTerm(context);
		LOG("--- value for r1["<<r1<<"]");
		if (r0 - r1 == 0) {
			rval = false;
		}
		else {
			rval = true;
		}
	}
	else if (this->opCode == IN) {
		int r0 = this->values[0]->evalTerm(context);
		LOG("--- value for r0["<<r0<<"]");
		SyntaxNodeIDREF* refn = static_cast<SyntaxNodeIDREF*>(this->values[1]);
		assert(refn->ref->type == TSET);
		Set* aSet = static_cast<Set*>(context->getCompValue(refn->ref));
		vector<SetValue*>::iterator it = aSet->setValues_data_order.begin();
		for(;it != aSet->setValues_data_order.end();it++) {
			assert((*it)->valueList.size() == 1);
			int r1 = atoi((*it)->valueList.front().c_str());
			LOG("--- value for r1["<<r1<<"]");
			if (r0 == r1) {
				rval = true;
				break;
			}
		}
	}
	else if (this->opCode == LOGICAL_AND) {
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
	return rval;
}

double SyntaxNode::evalTerm(ModelContext* context) {
	LOG("evalTerm -- single - opCode["<<this->opCode<<"] "<<this->print());
	double rval;
	if (this->opCode == IDREF) {
		SyntaxNodeIDREF* refn = static_cast<SyntaxNodeIDREF*>(this);
		assert(refn->ref->type == TPARAM);
		ModelComp* comp = refn->ref;
		Param* theParam = static_cast<Param*>(context->getCompValue(comp));

//		for(int i=0;i<refn->values.size();i++)
//		{
//			LOG(refn->values[i]->opCode);
//			LOG(refn->values[i]->print());
//		}

		string key = "";
		for(vector<SyntaxNode*>::iterator it = refn->values.begin();it != refn->values.end();it++) {
			//hack!! -- same as IDREF case in evalDiff
			IDNode* ind;
			if ((*it)->values.size() == 0) {
				ind = static_cast<IDNode*>(*it);
			}
			else {
				ind = static_cast<IDNode*>((*it)->values[0]);
			}
			assert(ind->opCode == ID);
			string dummy = ind->id();
			LOG("  ----  dummy name is ["<<dummy<<"]");
			key += context->getDummyValue(dummy);
			LOG("  -----  key is ["<<key<<"]");
		}
		rval = theParam->findParamValue(key);
	}
	else if (this->opCode == ORD) {
		assert(this->values[0]->opCode == LBRACKET);
		assert(this->values[0]->values[0]->opCode==COMMA);
		assert(this->values[0]->values[0]->values[0]->opCode == 0);
		assert(this->values[0]->values[0]->values[0]->values[0]->opCode==ID);
		IDNode* in = static_cast<IDNode*>(this->values[0]->values[0]->values[0]->values[0]);
		string dummy = in->id();
		ModelComp* comp = context->getDummySet(dummy);
		Set* theSet = static_cast<Set*>(context->getCompValue(comp));
		rval = (double) (theSet->getSetOrder(context->getDummyValue(dummy)));
	}
	else if (this->opCode == 0) {
		rval = this->values[0]->evalTerm(context);
	}

	else if (this->opCode == LSBRACKET) {
		assert(this->values.size() == 2);
		SyntaxNodeIDREF* refn = static_cast<SyntaxNodeIDREF*>(this->values[0]);
		assert(refn->ref->type == TPARAM);
		ModelComp* comp = refn->ref;
		Param* aParam = static_cast<Param*>(context->getCompValue(comp));

		assert(this->values[1]->opCode == COMMA);
		assert(this->values[1]->values.size() == 1);
		IDNode* idn = static_cast<IDNode*>(this->values[1]->values[0]);
		string dummyVar = idn->id();
		string dummyVal = context->getDummyValue(dummyVar);
		rval = aParam->findParamValue(dummyVal);
		LOG("aParam["<<aParam->getName()<<"] dummy["<<idn->id()<<"] dummyVal["<<dummyVal<<"] --> rval["<<rval<<"]");
	}
	else if (this->opCode == ID) {
		IDNode* idn = static_cast<IDNode*>(this);
		string idname = idn->id();
		if (idname.compare("\"null\"") == 0) {
			rval = 0;
		}
		else //it's a dummyvar
		{
			string dummyVar = idname;
			string dummyVal = context->getDummyValue(dummyVar);
			rval = atoi(dummyVal.c_str());
		}
	}
	else if (this->opCode == -99) {
		ValueNode* vn = static_cast<ValueNode*>(this);
		rval = vn->value;
	}
	else if (this->opCode == LBRACKET) {
		assert(this->values.size() == 1);
		rval = this->values.front()->evalTerm(context);
	}
	else if (this->opCode == COMMA) {
		assert(this->values.size() == 1);
		rval = this->values.front()->evalTerm(context);
	}
	else if (this->opCode == 43) //for +
			{
		assert(this->values.size() == 2);
		double left = this->values.at(0)->evalTerm(context);
		double right = this->values.at(1)->evalTerm(context);
		rval = left + right;
	}
	else if (this->opCode == 45) //for -
			{ //this should be handled by change the parser to distinguish unary-op minus and binary-op minus.
		if (this->values.size() == 2) {
			double left = this->values.at(0)->evalTerm(context);
			double right = this->values.at(1)->evalTerm(context);
			rval = left - right;
		}
		else if (this->values.size() == 1) {
			rval = -this->values.front()->evalTerm(context);
		}
		else {
			assert(false);
		}
	}
	else if (this->opCode == 42) //for *
			{
		assert(this->values.size() == 2);
		double left = this->values.at(0)->evalTerm(context);
		double right = this->values.at(1)->evalTerm(context);
		rval = left * right;
	}
	else {
		LOG("evalTerm -- single -- opCode["<<this->opCode<<"] not yet implmemented -- value_size["<<this->values.size()<<"]");
		assert(false);
	}
	LOG("evalTerm -- rval["<<rval<<"]");
	return rval;
}

string SyntaxNode::setOrgSetComp(ModelComp** orgSetComp) {
	LOG("setOrgSetComp - opCode["<<this->opCode<<"] "<<this->print());
	string dummy = static_cast<IDNode*>(this->values[0])->id();
	*orgSetComp = static_cast<SyntaxNodeIDREF*>(this->values[1])->ref;
	LOG(""<<static_cast<SyntaxNodeIDREF*>(this->values[1])->ref);
	LOG("end setOrgSetComp - dummy["<<dummy<<"] ref["<<(*orgSetComp)->id<<"|"<<*orgSetComp<<"]");
	return dummy;
}

//End Feng

string SyntaxNode::print_SyntaxNodesymb(const SyntaxNode *node) {
	//const ValueNode<long> *inode;
	const ValueNode* dnode;
	ostringstream ost;

	if (node == NULL) {
		return "NULL";
	}
	if (node->getOpCode() == ID)
		return "(ID T)";
//  if ((inode = dynamic_cast<const ValueNode<long> *>(node))) {
//    string temp = "T:";
//    temp += inode->getValue();
//    return temp;
//  }
	if ((dnode = dynamic_cast<const ValueNode*>(node))) {
		string temp = "T:";
		temp += dnode->getValue();
		return temp;
	}

	// start new version
	// print node symbol
	switch (node->getOpCode())
	{
		case IDREF: {
			const SyntaxNodeIDREF *onir = dynamic_cast<const SyntaxNodeIDREF*>(node);
			if (onir == NULL) {
				cerr << "Some IDREF node still not SyntaxNodeIDREF\n";
				exit(1);
			}
			ModelComp *mc = onir->ref;
			ost << "IDREF(" << (void*) node << ":" << mc->id << "(" << (void*) mc << "))";
		}
			break;
		case ASSIGN:
			ost << "ASSIGN";
			break;
		case IN:
			ost << "IN";
			break;
		case SUM:
			ost << "SUM";
			break;
		case LBRACE:
			ost << "LBR{";
			break;
		case LBRACKET:
			ost << "LBR(";
			break;
		case COMMA:
			ost << "COMMA";
			break;
		case COLON:
			ost << "COLON";
			break;
		case '+':
			ost << "\"+\"";
			break;
		case '*':
			ost << "\"*\"";
			break;
		default:
			ost << "\"" << node->getOpCode() << "\"";
			break;
	}
	ost << "(";
	for(SyntaxNode::iterator i = node->begin();i != node->end();++i) {
		if (i != node->begin())
			ost << ", ";
		ost << print_SyntaxNodesymb(*i);
	}
	ost << ")";

	return ost.str();
}

/* ----------------------------------------------------------------------------
 findKeywordinTree
 ---------------------------------------------------------------------------- */
/** Traverses down the tree and returns the topmost reference to the keyword
 *  in the Tree.
 */
SyntaxNode* SyntaxNode::findKeywordinTree(SyntaxNode *root, int oc) {
	if (root->getOpCode() == oc)
		return root;

	SyntaxNode *found, *res;
	found = NULL;
	for(SyntaxNode::iterator i = root->begin();i != root->end();++i) {
		res = findKeywordinTree(*i, oc);
		if (res && found) {
			cerr << "Found keyword " << oc << "at least twice in " << root << "\n";
			exit(1);
		}
		found = res;
	}
	return found;
}

/* ---------------------------------------------------------------------------
 find_var_ref_in_context
 ---------------------------------------------------------------------------- */
/** This routine does the work of putting together dot'd variable names
 *  'root' is a SyntaxNode of type ID that points to the left hand part
 *  of the dot'd expression parsed so far. 'ref' is the new part that
 *  should be added.
 *
 *  @param context
 *         A pointer to the current AmplModel that defines the scope in which
 *         the ID expressions should be resolved.
 *  @param ref
 *         A pointer to an expression that evaluates to a ModelComp: this can
 *         be given by an ID, a dotted expression ID.ID or a reference to a
 *         parent stage (in StochProg) such as ID(-1;...).
 *         It can also carry an indexing expressinon ID[.,.,.] in which case
 *         the indexing is attached to the returned IDREF node.
 *
 *  @return A SyntaxNode of type IDREF that points to the correct ModelComp.
 *
 *  @bug Should return a SyntaxNodeIDREF*.
 *
 *  A SyntaxNode of type IDREF looks like this
 *       ->opCode = IDREF;
 *       ->nval = # of arguments
 *       ->values[0] = pointer to entity in model list
 *       ->values[1 - n] = arguments
 */
SyntaxNode* SyntaxNode::find_var_ref_in_context(AmplModel *context, SyntaxNode *ref) {
	/* 'ref' is a SyntaxNode representing an iditem.
	 This can be either
	 - a ID node where values[0] simply points to a name
	 - an ID node which is actually SyntaxNodeID and has stochparent set
	 - a LSBRACKET node, where values[0] is ID and values[1] is CSL
	 in the second case the CSL should be added as further arguments
	 to the resulting IDREF node
	 */

	/* returns: pointer */
	LOG("Finding ref:"<<ref<<" in AmplModel(context):"<<context->name);
	SyntaxNode *tmp;
	ListNode *argNode;
	IDNode *idNode;
	SyntaxNodeIDREF *ret;
	int stochparent = 0;

	/* and now scan through the whole of the local context to see if we
	 find any matches */
	/* the local context is
	 - all vars
	 - all constraints
	 - all objectives
	 - all sets
	 - all parameters
	 - all submodels
	 - all temporary variables (this list needs to be set up somewhere)
	 */

	// split the expression 'ref' into an id part and an argument list
	if (ref->getOpCode() == ID) {
		LOG("  -- ["<<ref<<"] has an ID opCode");
		idNode = (IDNode *) ref;
		argNode = NULL;
	}
	else {
		assert(ref->getOpCode() == LSBRACKET || ref->getOpCode() == LBRACKET);
		LOG("  -- ["<<ref<<"] has an LSBRACKET or LBRACKET opCode");
		SyntaxNode::iterator i = ref->begin();
		idNode = (IDNode*) *i;
		argNode = (ListNode*) *(++i);
		assert(idNode->getOpCode() == ID);
		assert(argNode->getOpCode() == COMMA);
	}

	// Test if this ID node is actually of type SyntaxNodeID and if so remember
	// the value of stochparent
	{
		if (idNode->getStochParent() != 0)
			// there is an extra argument, which is the stochparent
			stochparent = idNode->getStochParent();
	}

	LOG("--> now Search for matches of " << idNode->id() <<" in current active indexings");

	// see if this matches a dummy variable
	tmp = SyntaxNode::find_var_ref_in_indexing(idNode->id());
	if (tmp) {
		LOG("    "<<idNode->id() << " is matched by dummy var in " << *tmp);
		return ref;
	}

	// try to find a match in the local context
	LOG("--> continue searching in this_context["<<context->name<<"]");
	ret = context->find_var_ref_in_context(idNode);

	// ret could be NULL if it is actually a STAGE or NODE dummy variable
	if (!ret) {
		LOG("--> not found in either current index or current context's modelComp");
		if (argNode) {
			cerr << "dummy index of stageset or nodeset and argNode=true not "
					"yet handled!" << endl;
			exit(1);
		}
		return idNode; // return something at least vaguely meaningful
	}

	if (argNode) {
		LOG("Adding argument list to node: " << *argNode);
		//free(idNode->values); // jdh - what does this do?
		for(ListNode::iterator i = argNode->begin();i != argNode->end();++i)
			ret->push_back(*i);
		if (ref->getOpCode() == LBRACKET) {
			// this is old code to deal with ancestor(1).ID declarations. To go
			cerr << "Executing old code to deal with ancestor(1).ID "
					"declarations\n";
			exit(1);

			// This is a reference indexed by '(..)'. In this case we are in
			// a stoch block and the first argument refers to the stage
			//      ret->stochrecourse = (SyntaxNode*)ret->values[0];
			//for (int i = 1; i < ret->nchild(); ++i) {
			//ret->values[i-1] = ret->values[i];
			//}
			//ret->nval--;
		}
		argNode->clear();
	}

	ret->setStochParent(stochparent);
	return ret;
}

/* ---------------------------------------------------------------------------
 find_var_ref_in_indexing
 ---------------------------------------------------------------------------- */
/** Scan through the current set of active indexing expressions and see if
 *  any of them defines the dummy variable given by 'name'.
 *
 *  @param name
 *         The name of identifier to look for.
 *
 *   int n_indexing             the currently active indexing expressions
 *   SyntaxNode *list_of_indexing
 *  @return The Indexing expression in which the name occurs (or NULL if there
 *          is no match).
 */
SyntaxNode* SyntaxNode::find_var_ref_in_indexing(const string& name) {

	SyntaxNodeIx *tmp;
	SyntaxNode *ret = NULL;

	for(int i = 0;i < n_indexing;++i) {
		/* have a look at all the indexing expressions */
		/* an indexing expression is a '{' node followed by a 'Comma' node */
		tmp = list_of_indexing[i];
		if (tmp != NULL) {
			tmp->splitExpression();
			ret = tmp->hasDummyVar(name);
			if (ret)
				return ret;
		}
	}
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

/* --------------------------------------------------------------------------
 SyntaxNode::print()
 --------------------------------------------------------------------------- */
/** Recursively prints the expression rooted at the current node in the
 *  expression tree.
 *
 *  @note use_global_names influences how nodes of type IDREF are printed.
 */
ostream& operator<<(ostream&s, const SyntaxNode *node) {
	if (node == NULL)
		return s;
	return node->put(s);
}

ostream& operator<<(ostream&s, const SyntaxNode &node) {
	if (&node == NULL)
		return s;
	return node.put(s);
}
