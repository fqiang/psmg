/* (c) 2014 Feng Qiang and Andreas Grothey, University of Edinburgh
 *
 * This file is part of PSMG.
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

#ifndef STOCHMODEL_H
#define STOCHMODEL_H

#include <string>
#include <vector>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include "../model/AmplModel.h"

class SyntaxNode;
class ExpandedModel;
class ModelContext;

/* ------------------------------------------------------------------------ */
/** @class StochModel
 *  This class describes a stochastic model (block).
 *
 *  It will gather the information present in the SML model file for this
 *  block much in the same way that AmplModel does for ordinary blocks.
 *  The difference is that sets and parameters that define the
 *  Scenario tree are associated with it. These are
 *  - stageset:    (ordered) set of stages
 *  - nodeset:     set of nodes
 *  - anc:         parameter array giving ancestor node for every node
 *  - prob:        parameter array giving conditional probability for each node
 *
 *  In principle the stochastic model block can also be repeated "horizontally"
 *  in the same manner as all other blocks by specifying an indexing
 *  expression.
 *  The stochastic model block will be expanded at processing time into a
 *  nested set of AmplModels.
 */
class StochModel: public AmplModel {

public:

	//! The set of NODES
	SyntaxNode *nodeset;
	//! The parameter array of ancestors
	SyntaxNode *ancestor;
	//! The parameter array of probabilities
	SyntaxNode *probs;
	//! The set of STAGES
	SyntaxNode *stageset;

	//! The stage node corresponding to the stage set for this stochastic model.
	boost::unordered_map<SyntaxNode*,boost::unordered_set<string>* > stageSetMap;

	//! Constructor
	StochModel(const string& name, SyntaxNode* stochsets,AmplModel *parent);
	~StochModel();
	ExpandedModel* createExpandedModel(string dummyVar,SetComp* comp,string value,ModelContext* parent);
	AmplModel* convertToAmplModel(ModelContext& parCtx);

	bool isInCurrentStage(SyntaxNode* stage, string& stagename, ModelContext& ctx);
};

#endif
