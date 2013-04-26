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
#ifndef SETELH
#define SETELH
#include <vector>
#include <string>

using namespace std;


/** @class SetValue
 *  This class describes an element of a set. It is basically an array of
 *  strings (char*) together with a size.
 */

class SetValue
{

	public:
		vector<string> valueList;


		SetValue(string);
		SetValue(vector<string> list);
		SetValue(const SetValue& src);
		~SetValue();

		string toString();
		string getKey() const ;
		bool isEqual(SetValue*);
		void fillValueList(vector<string>& index);
		void calculateMemoryUsage(unsigned long& size);
};


#endif
