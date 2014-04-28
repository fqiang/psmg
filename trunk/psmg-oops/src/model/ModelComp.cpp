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
#include "ModelComp.h"
#include "AmplModel.h"
#include "SyntaxNode.h"
#include "SyntaxNodeIDREF.h"
#include "../context/Param.h"
#include "../context/ExpandedModel.h"
#include "../parser/sml.tab.h"
#include "SyntaxNodeValue.h"
#include <cassert>
#include <cstdlib>
#include <iostream>

using namespace std;


const string ModelComp::nameTypes[] = { "variable", "constraint", "parameter", "set", "objective", "submodel" };


/** Construct a model component given its name, id, indexing and attribute
 *  sections.
 *  Also analyses dependencies in indexing and attribute and set the 
 *  dependencies list
 *  @param id
 *         Name of the component
 *  @param type
 *         Type of the component
 *  @param indexing
 *         Root node of the indexing expression
 *                     IDs should have been replaced by IDREFs 
 *  @param attrib
 *         Root node of the attribute expression
 *                     IDs should have been replaced by IDREFs 
 */
ModelComp::ModelComp(const string& id, compType type, SyntaxNode* indexing, SyntaxNode *attribute,AmplModel* owner)
	:name(id), type(type), indexing(indexing),attributes(attribute), model(owner)
{

	LOG("ModelComp constructor  - id=["<<id<<"] indexing["<<this->indexing<<"] attribute["<<this->attributes<<"] type["<<type<<"] -- in Model["<<owner<<"]");

}


ModelComp::~ModelComp() {
	LOG("enter ModelComp destructor called...["<<this->name<<"]");
	if (attributes != NULL) {
		LOG(" --- delete attributes ");
		//delete attributes;
		attributes = NULL;
	}
	if (indexing != NULL) {
		LOG(" --- delete indexing ");
		//delete indexing;
		indexing = NULL;
	}
	LOG("exit ModelComp destructor called...["<<this->name<<"]");
}

void ModelComp::calculateMemoryUsage(unsigned long& size) {
	LOG_MEM("ModelComp::calculateMemoryUsage -- comp["<<this->name<<"]");
	size += sizeof(ModelComp);
	if (this->attributes != NULL) {
		LOG_MEM(" --- attribute ["<<this->attributes->print()<<"]");
		this->attributes->calculateMemoryUsage(size);
	}
	if (this->indexing != NULL) {
		LOG_MEM(" --- indexing ["<<this->indexing->print()<<"]");
		this->indexing->calculateMemoryUsage(size);
	}
}

