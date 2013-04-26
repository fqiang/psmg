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
#ifndef SETNODE_H
#define SETNODE_H

#include "SyntaxNode.h"
#include "../context/CompDescr.h"
#include "../parser/sml.tab.h"
#include <cassert>
#include <string>
#include <vector>

/** @class SetNode
 *  This class represents a set in the the syntax tree.
 */
class SetNode: public SyntaxNode /*, public CompDescr*/ {
protected:
   //SetNode *within;

public:
   SetNode(int opCode_, SyntaxNode *node1=NULL, SyntaxNode *node2=NULL);
   virtual void calculateMemoryUsage(unsigned long& size);
};

#endif /* ifndef SETNODE_H */
