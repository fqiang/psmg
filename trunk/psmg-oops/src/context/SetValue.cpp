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

#include "SetValue.h"
#include "../model/IDNode.h"
#include "../util/global_util_functions.h"
#include <string>
#include <cassert>
#include <vector>

using namespace std;


SetValue::SetValue(string value)
{
	this->valueList.push_back(value);
}
SetValue::SetValue(vector<string> list)
{
	this->valueList.clear();
	for(vector<string>::iterator i=list.begin();i!=list.end();i++)
	{
		this->valueList.push_back((*i));
	}
}


SetValue::~SetValue()
{
	LOG("SetValue destructor called  -- ["<<this->toString()<<"]");
	this->valueList.clear();
}

SetValue::SetValue(const SetValue& src)
{
	LOG("make SetValue copy -- key["<<src.getKey()<<"]");
	vector<string>::const_iterator i;
	for(i=src.valueList.begin();i!=src.valueList.end();i++)
	{
		this->valueList.push_back((*i));
	}
}

string SetValue::toString()
{
	ostringstream oss(ostringstream::out);
	oss<<"(";
	for(vector<string>::iterator i=valueList.begin();i!=valueList.end();i++)
	{
		oss<<(*i);
		if(i!=valueList.end()-1){
			oss<<",";
		}
	}
	oss<<")";
	return oss.str();
}

string SetValue::getKey() const
{
	ostringstream oss;
	for(vector<string>::const_iterator i=valueList.begin();i!=valueList.end();i++)
	{
		oss<<(*i);
	}
	return oss.str();
}

bool SetValue::isEqual(SetValue* other)
{
	vector<string>::const_iterator this_i = this->valueList.begin();
	vector<string>::const_iterator other_i = other->valueList.begin();
	bool equal = false;
	while(true)
	{
		if(this_i==this->valueList.end()||other_i==other->valueList.end())
		{
			break;
		}
		if((*this_i).compare(*other_i)!=0)
		{
			equal=false;
			break;
		}
		equal = true;
		this_i++;
		other_i++;
	}
	return equal;
}

void SetValue::fillValueList(vector<string>& index)
{
	vector<string>::const_iterator i;
	for(i=this->valueList.begin();i!=this->valueList.end();i++)
	{
		index.push_back((*i));
	}
}

void SetValue::calculateMemoryUsage(unsigned long& size)
{
	size += sizeof(SetValue);
	for(vector<string>::iterator it=valueList.begin();it!=valueList.end();it++)
	{
		size += sizeof(string);
		size += (*it).size()+1;
	}
}
