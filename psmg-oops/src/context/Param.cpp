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
#include "ParamValue.h"
#include "../parser/data.tab.h"
#include "../util/GlobalVariables.h"
#include "../model/ModelComp.h"
#include "../model/SyntaxNode.h"
#include "../model/SyntaxNodeIx.h"
#include "../util/global_util_functions.h"
#include <string>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>

using namespace std;

using namespace __gnu_cxx;

Param::Param(int numInd_,string name_):numIndicies(numInd_),name(name_),card(0)
{
	LOG("Create param - name["<<name<<"] numIndicies["<<numIndicies<<"]");
}

Param::~Param()
{
	hash_map<string,ParamValue*>::iterator it=paramValues.begin();
	for(;it!=paramValues.end();it++)
	{
		delete it->second;
	}
	paramValues.clear();
}

void Param::addParamValue(ParamValue* value)
{
	assert(value->indices.size()==this->numIndicies);
	LOG("Add ParamValue index["<<card<<"] ["<<value->toString()<<"]");
	paramValues.insert(pair<string,ParamValue*>(value->getParamIndicesKey(),value));
	card++;
}

double Param::findParamValue(string indiciesKey)
{
	LOG("-- findParamValue in    - "<<endl<<this->toString());
	double rval = (this->paramValues.find(indiciesKey)->second)->value;
	LOG("-- findParamValue in    - ["<<rval<<"]");
	return rval;
}

string Param::toString() const
{
	ostringstream oss;
	hash_map<string,ParamValue*>::const_iterator i;
	for(i=this->paramValues.begin();i!=this->paramValues.end();i++)
	{
		oss<<(*i).second->toString()<<endl;
	}
	return oss.str();
}

string Param::getName()
{
	return this->name;
}

void Param::calculateMemoryUsage(unsigned long& size)
{
	size += sizeof(Param);
	size += name.size() + 1;
	hash_map<string,ParamValue*>::iterator it = paramValues.begin();
	for(;it!=paramValues.end();it++)
	{
		size += sizeof(pair<string,ParamValue*>);
		size += (*it).first.size() + 1;
		(*it).second->calculateMemoryUsage(size);
	}
}
