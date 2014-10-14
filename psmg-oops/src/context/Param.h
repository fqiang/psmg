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
#ifndef PARAM_H_
#define PARAM_H_

#include <string>
#include "CompDescr.h"

class ParamComp;


using namespace std;


/** @class CompDescrParam
 *  This class describes a parameter: it consists of
 *   - a list of indexing sets (these might be multidimensional themselves) 
 *   - a list of parameter values (in dense format?) - 
 *         i.e. a multidimensional array
 */  
class	 Param: public CompDescr{

	public:
		string name;		//param name
		int card;  			//number of values
		int numIndicies; 	//number of set indices

		Param(ParamComp* comp);
		~Param();

		static Param* createParam(ParamComp* comp);
		virtual string toString() =0;
		virtual void calculateMemoryUsage(unsigned long& size) =0;

};

#endif
