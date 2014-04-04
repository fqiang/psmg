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

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "OOPSBlock.h"
#include "../context/ExpandedModel.h"
#include "../sml/GlobalVariables.h"
#include "../util/global_util_functions.h"

using namespace std;

//PrintLevelValues GlobalVariables::prtLvl;

/* ----------------------------------------------------------------------------
OOPSBlock::OOPSBlock(ExpandedModelInterface*, list<string>*)
---------------------------------------------------------------------------- */
OOPSBlock::OOPSBlock(ExpandedModel *rowmod, ExpandedModel *colmod):emrow(rowmod),emcol(colmod)
{
  LOG("OOPSBlock: col: " << colmod->name << "/ row: " << rowmod->name <<" -- (" << rowmod->getNLocalCons() << "x" << colmod->getNLocalVars() << ")");
}