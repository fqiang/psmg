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

#include "Param.h"
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include "../parser/data.tab.h"
#include "../util/util.h"
#include "../model/ParamComp.h"
#include "../context/ParamMult.h"
#include "../context/ParamSingle.h"

using namespace std;


Param::Param(ParamComp* comp):numIndicies(comp->numIndicies),name(comp->name),card(0)
{
	TRACE("Create param - name["<<name<<"] numIndicies["<<numIndicies<<"]");
}

Param::~Param()
{
	TRACE("delete param - name["<<name<<"]");
}

Param* Param::createParam(ParamComp* comp)
{
	if(comp->numIndicies>0) return new ParamMult(comp);
	else{
		assert(comp->numIndicies == 0);
		return new ParamSingle(comp);
	}

}

