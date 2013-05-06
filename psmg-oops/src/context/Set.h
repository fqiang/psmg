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
#include "../util/global_util_functions.h"
#include <ext/hash_map>
#include <string>

using namespace std;
using namespace __gnu_cxx;

class Set: public CompDescr{
	private:

	public:
		int dim; //same as in ModelComp
		string name;
		int card;
		vector<string> setValues_data_order;
		hash_map<string,int> setOrders;
		hash_map<int,string> setKeys;

		Set(int dim_,string name_);
		virtual ~Set();
		Set(const Set& dest);

		int setOrder(string key);
		void addSetValue(string& key);
		void addSetValue(ostringstream& oss);
		void fillSetValues(char*& data);
		Set* setDiff(Set* sub);
		Set* setCross(Set* other);
		int getCard();
		string toString() const;
		bool contains(string&);
		void calculateMemoryUsage(unsigned long& size);
	private:
};

#endif
