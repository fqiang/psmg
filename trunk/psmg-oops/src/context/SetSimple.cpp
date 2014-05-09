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

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "SetSimple.h"
#include "../model/SyntaxNode.h"
#include "../context/SetOrdered.h"
#include "../model/SetComp.h"
#include "../parser/data.tab.h"
#include "../util/util.h"

using namespace std;
using namespace __gnu_cxx;

SetSimple::SetSimple(string& name, int dim):Set(name,dim)
{
	LOG("Create SimpleSet - name["<<name<<"] dim["<<dim<<"]");
}

SetSimple::SetSimple(SetComp* comp): Set(comp)
{
	LOG("Create SimpleSet - name["<<name<<"] dim["<<dim<<"]");
}

//SimpleSet::SimpleSet(SimpleSet& src):dim(src.dim),name(src.name),card(0)
//{
//	LOG("make set copy -- from name["<<name<<"] -- "<<src.toString());
//	vector<string>::const_iterator i;
//	for(i=src.setValues_data_order.begin();i!=src.setValues_data_order.end();i++)
//	{
//		string value = (*i);
//		this->addSetValue(value);
//	}
//}

SetSimple::~SetSimple()
{
	LOG("SimpleSet::~SimpleSet -- Delete  --"<<toString());
	this->setValues_data_order.clear();
}

void SetSimple::addSetValue(const string& value)
{
	if(!this->contains(value))
	{
		card++;
		LOG("Add SetValue card["<<card<<"] ["<<value<<"]");
		this->setValues_data_order.push_back(value);
	}
	else
	{
		LOG("SetSimple -- already has["<<value<<"]");
	}
	//LOG("addSetValue --  set contains "<<this->toString());
}

string SetSimple::toString()
{
	LOG("SimpleSet["<<name<<"] card["<<card<<"]");
	ostringstream oss;
	vector<string>::const_iterator i;
	for(i= this->setValues_data_order.begin();i!=this->setValues_data_order.end();i++)
	{
		oss<<(*i)<<"|";
	}
	return oss.str();
}

void SetSimple::copyFromSet(Set* src)
{
	assert(this->setValues_data_order.empty());
	this->dim = src->dim;
	this->card = src->card;
	this->setValues_data_order.insert(this->setValues_data_order.begin(),src->setValues_data_order.begin(),src->setValues_data_order.end());
}

void SetSimple::calculateMemoryUsage(unsigned long& size)
{
	Set::calculateMemoryUsage(size);
	size += sizeof(SetSimple);
}
