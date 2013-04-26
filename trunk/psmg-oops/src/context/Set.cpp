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
	__gnu_cxx::hash_map<string,SetValue*>::const_iterator i;
	for(i=src.setValues.begin();i!=src.setValues.end();i++)
	{
		SetValue* value = (*i).second;
		SetValue* tmp = new SetValue(*value);
		this->addSetValue(tmp);
	}
}

Set::~Set()
{
	LOG("Set destructor called ");
	vector<SetValue*>::iterator it = this->setValues_data_order.begin();
	for(;it!=this->setValues_data_order.end();it++)
	{
		delete (*it);
	}
	this->setValues_data_order.clear();
	this->setValues.clear();
	this->setOrders.clear();
	this->setKeys.clear();
}

void Set::addSetValue(SetValue* value)
{
	card++;
	assert(value->valueList.size()==this->dim);
	LOG("Add SetValue index["<<card<<"] ["<<value->toString()<<"]");
	this->setValues.insert(pair<string,SetValue*>(value->getKey(),value));
	this->setOrders.insert(pair<string,int>(value->getKey(),card));
	this->setKeys.insert(pair<int,string>(card,value->getKey()));
	this->setValues_data_order.push_back(value);
	LOG("addSetValue -- "<<this->toString());
}

int Set::setOrder(string key)
{
	__gnu_cxx::hash_map<string,int>::iterator i = this->setOrders.find(key);
	assert(i!=this->setOrders.end());
	return (*i).second;
}

void Set::fillSetValues(string datalist)
{
	char* data = new char[datalist.length() + 1];
	strcpy(data,datalist.c_str());
	char* token;
	token = strtok(data,",() ");

	vector<string> toks;
	int tokenNum = 0;
	while(token!=NULL)
	{
		string tok(token);
		if(tokenNum%this->dim == 0 && toks.size()!=0)
		{
			SetValue* value = new SetValue(toks);
			this->addSetValue(value);
			toks.clear();
		}
		toks.push_back(tok);
		token = strtok(NULL,",() ");
		tokenNum++;
	}
	//add the last set value
	this->addSetValue(new SetValue(toks));
	toks.clear();

	delete [] data;
}

Set* Set::setDiff(Set* sub)
{
	assert(this->dim == sub->dim);
	LOG("call set diff  ["<<this->name<<"  DIFF  "<<sub->name<<"]");
	Set* rval = new Set(this->dim,this->name+"_DIFF_"+sub->name);
	__gnu_cxx::hash_map<string,SetValue*>::iterator i;
	for(i= this->setValues.begin();i!=this->setValues.end();i++)
	{
		string currKey = (*i).first;
		if(sub->setValues.find(currKey)==sub->setValues.end())
		{
			SetValue* v = new SetValue((*i).second->valueList);
			rval->addSetValue(v);
		}
	}
	return rval;
}

Set* Set::setCross(Set* other)
{
	LOG("call set cross  ["<<this->name<<"  CROSS  "<<other->name<<"]");
	Set* rval = new Set(this->dim*other->dim,this->name+"CROSS"+other->name);
	__gnu_cxx::hash_map<string,SetValue*>::iterator i;
	__gnu_cxx::hash_map<string,SetValue*>::iterator j;
	for(i= this->setValues.begin();i!=this->setValues.end();i++)
	{
		for(j= other->setValues.begin();i!=other->setValues.end();i++)
		{
			vector<string> val;
			copy((*i).second->valueList.begin(),(*i).second->valueList.end(),val.begin());
			copy((*j).second->valueList.begin(),(*j).second->valueList.end(),val.end());
			SetValue* v = new SetValue(val);
			rval->addSetValue(v);
		}
	}
	return rval;
}

int Set::getSetOrder(string indiciesKey)
{
	return this->setOrders.find(indiciesKey)->second;
}

int Set::getCard()
{
	return this->setOrders.size();
}

string Set::toString() const
{
	ostringstream oss;
	__gnu_cxx::hash_map<string,SetValue*>::const_iterator i;
	for(i= this->setValues.begin();i!=this->setValues.end();i++)
	{
		oss<<(*i).second->toString();
	}
	return oss.str();
}

bool Set::contains(SetValue& value)
{
	bool isIn = true;
	string key = value.getKey();
	__gnu_cxx::hash_map<string,SetValue*>::iterator i = this->setValues.find(key);
	if(i==this->setValues.end())
	{
		isIn = false;
	}
	return isIn;
}

void Set::calculateMemoryUsage(unsigned long& size)
{
	size += sizeof(Set);
	size += name.size() + 1;
	for(vector<SetValue*>::iterator it=setValues_data_order.begin();it!=setValues_data_order.end();it++)
	{
		size += sizeof(SetValue*);
		(*it)->calculateMemoryUsage(size);
	}
	for(hash_map<string,SetValue*>::iterator it=setValues.begin();it!=setValues.end();it++)
	{
		size += sizeof(pair<string,SetValue*>);
		size += (*it).first.size() + 1;
		//(*it).second->calculateMemoryUsage(size);  //ready count in setValues_data_order
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
