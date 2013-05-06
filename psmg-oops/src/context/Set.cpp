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
#include "Set.h"
#include "../model/SyntaxNode.h"
#include "../model/ListNode.h"
#include "../parser/data.tab.h"
#include "../util/global_util_functions.h"

using namespace std;
using namespace __gnu_cxx;

Set::Set(int dim_,string name_): dim(dim_),name(name_),card(0)
{
	LOG("Create set - name["<<name<<"] dim["<<dim<<"]");
}

Set::Set(const Set& src):dim(src.dim),name(src.name),card(0)
{
	LOG("make set copy -- from name["<<name<<"] -- "<<src.toString());
	vector<string>::const_iterator i;
	for(i=src.setValues_data_order.begin();i!=src.setValues_data_order.end();i++)
	{
		string value = (*i);
		this->addSetValue(value);
	}
}

Set::~Set()
{
	LOG("Set destructor called ");
	this->setValues_data_order.clear();
	this->setOrders.clear();
	this->setKeys.clear();
}

void Set::addSetValue(string& value)
{
	card++;
	LOG("Add SetValue index["<<card<<"] ["<<value<<"]");
	this->setValues_data_order.push_back(value);
	this->setOrders.insert(pair<string,int>(value,card));
	this->setKeys.insert(pair<int,string>(card,value));
	LOG("addSetValue -- "<<this->toString());
}


void Set::addSetValue(ostringstream& oss)
{
	card++;
	LOG("Add SetValue oss index["<<card<<"] ["<<oss.str()<<"]");
	this->setValues_data_order.push_back(oss.str());
	this->setOrders.insert(pair<string,int>(oss.str(),card));
	this->setKeys.insert(pair<int,string>(card,oss.str()));
	LOG("addSetValue -- "<<this->toString());
}

void Set::fillSetValues(char*& data)
{
//	char* data = datalist.c_str();
	char* token;
	token = strtok(data,",() ");

	ostringstream oss(ostringstream::out);
	unsigned int tokenNum = 0;
	do
	{
		oss<<token;
		tokenNum++;
		if(tokenNum%this->dim == 0)
		{
			this->addSetValue(oss);
			oss.str("");
			oss.clear();
		}
		token = strtok(NULL,",() ");
	}while(token!=NULL);
	LOG("Set::fillSetValues -- tokenNum-"<<tokenNum<<"] -- ");
	LOG("set --------");
	LOG(this->toString());
}

Set* Set::setDiff(Set* sub)
{
	assert(this->dim == sub->dim);
	LOG("call set diff  ["<<this->name<<"  DIFF  "<<sub->name<<"]");
	Set* rval = new Set(this->dim,this->name+"_DIFF_"+sub->name);

	vector<string>::iterator i;
	for(i= this->setValues_data_order.begin();i!=this->setValues_data_order.end();i++)
	{
		string currKey = (*i);
		if(sub->setOrders.find(currKey)==sub->setOrders.end())
		{
			rval->addSetValue(currKey);
		}
	}
	return rval;
}

Set* Set::setCross(Set* other)
{
	LOG("call set cross  ["<<this->name<<"  CROSS  "<<other->name<<"]");
	Set* rval = new Set(this->dim*other->dim,this->name+"CROSS"+other->name);
	vector<string>::iterator i;
	vector<string>::iterator j;
	ostringstream oss(ostringstream::out);
	for(i= this->setValues_data_order.begin();i!=this->setValues_data_order.end();i++)
	{
		for(j= other->setValues_data_order.begin();i!=other->setValues_data_order.end();i++)
		{
			oss<<*i;
			oss<<*j;
			rval->addSetValue(oss);
			oss.str("");
			oss.clear();
		}
	}
	return rval;
}


int Set::setOrder(string key)
{
	__gnu_cxx::hash_map<string,int>::iterator i = this->setOrders.find(key);
	assert(i!=this->setOrders.end());
	return (*i).second;
}

int Set::getCard()
{
	return this->setOrders.size();
}

string Set::toString() const
{
	ostringstream oss;
	vector<string>::const_iterator i;
	for(i= this->setValues_data_order.begin();i!=this->setValues_data_order.end();i++)
	{
		oss<<(*i)<<"|";
	}
	return oss.str();
}

bool Set::contains(string& value)
{
	bool isIn = true;
	__gnu_cxx::hash_map<string,int>::iterator i = this->setOrders.find(value);
	if(i==this->setOrders.end())
	{
		isIn = false;
	}
	return isIn;
}

void Set::calculateMemoryUsage(unsigned long& size)
{
	size += sizeof(Set);
	size += name.size() + 1;
	for(vector<string>::iterator it=setValues_data_order.begin();it!=setValues_data_order.end();it++)
	{
		size += (*it).size() + 1;
	}
	for(hash_map<string,int>::iterator it=setOrders.begin();it!=setOrders.end();it++)
	{
		size += sizeof(pair<string,int>);
		size += (*it).first.size() + 1;
	}
	for(hash_map<int,string>::iterator it=setKeys.begin();it!=setKeys.end();it++)
	{
		size += sizeof(pair<int,string>);
		size += (*it).second.size() + 1;
	}
}
