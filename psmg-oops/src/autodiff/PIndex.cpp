/*
 * PIndex.cpp
 *
 *  Created on: 20 Sep 2014
 *      Author: s0965328
 */

#include "PIndex.h"
#include "../context/ExpandedModel.h"

namespace AutoDiff {

PIndex::PIndex(uint idx):PNode(),idx(idx) {
	// TODO Auto-generated constructor stub

}

PIndex::~PIndex() {
	// TODO Auto-generated destructor stub
}

double& PIndex::pval(){
	assert(idx < ExpandedModel::n_row);
	assert(ExpandedModel::Y!=NULL);
	return ExpandedModel::Y[idx];
}

string PIndex::toString(int level)
{
	ostringstream oss;
	string s(level,'\t');
	oss<<s<<"[PIndex]("<<this->idx<<")"<<this;
	return oss.str();
}


} /* namespace AutoDiff */
