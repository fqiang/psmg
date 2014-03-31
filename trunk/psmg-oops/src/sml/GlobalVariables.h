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
#ifndef GLOBALVARIABLES
#define GLOBALVARIABLES

#define GV(X) GlobalVariables::X

#include <string>
#include <limits>

#define D_POS_INFI std::numeric_limits<double>::infinity()
#define D_NEG_INFI -std::numeric_limits<double>::infinity()
#define D_NaN std::numeric_limits<double>::quiet_NaN()

/** @class GlobalVariables 
 *  This class provides some static global variables.
 *
 *  This class provides global variables that control the level of debug
 *  printing and logging.
 *  It also stores the names of the model and data input files.
 *  The initial values for these variables is set in ampl.ypp. Any class
 *  that wishes to use this variables must include "GlobalVariables.h".
 */
using namespace std;
class GlobalVariables{
 public:

	static int rank;
	static int size;
	//! Name of the data file
	static string datafilename;

	static string modelfilename;

	static string outfilename;

	//! Command used for invoking ampl
	static string amplcommand;

	static string hostname;

	static string executable;

	static bool logParseModel; //!< Controls if the model parser should log

	static bool debug;

	static bool writeMPS;

	static bool writeMatlab;

	static bool solve;

	static string logdir;

	static void printAll();
};

#endif
