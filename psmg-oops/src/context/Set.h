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
#ifndef SET_H
#define SET_H

#include "CompDescr.h"
#include "SetValue.h"
#include "../util/global_util_functions.h"
#include <ext/hash_map>
#include <string>

using namespace std;
using namespace __gnu_cxx;

class Set: public CompDescr{
	private:

	public:
		int dim; //same as in ModelComp
		int card;
		string name;
		vector<SetValue*> setValues_data_order;
		__gnu_cxx::hash_map<string,SetValue*> setValues;
		__gnu_cxx::hash_map<string,int> setOrders;
		__gnu_cxx::hash_map<int,string> setKeys;

		Set(int dim_,string name_);
		virtual ~Set();
		Set(const Set& dest);

		int getSetOrder(string key);
		void addSetValue(SetValue* element);
		void fillSetValues(string datalist);
		Set* setDiff(Set* sub);
		Set* setCross(Set* other);
		int  setOrder(string key);
		int getCard();
		string toString() const;
		bool contains(SetValue&);
		void calculateMemoryUsage(unsigned long& size);
	private:
};

#endif
