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
#ifndef COFIG_H
#define COFIG_H

#define GV(X) Config::instance()->X

#include <string>
#include <limits>
#include "../xml/pugixml.hpp"

#define INFINITY_D std::numeric_limits<double>::infinity()
#define NEG_INFINITY_D -std::numeric_limits<double>::infinity()
#define NaN_D std::numeric_limits<double>::quiet_NaN()

/** @class Config
 *  This class provides some  global variables.
 *
 *  This class provides global variables that control the level of debug
 *  printing and logging.
 *  It also stores the names of the model and data input files.
 *  The initial values for these variables is set in ampl.ypp. Any class
 *  that wishes to use this variables must include "GlobalVariables.h".
 */

using namespace std;
class Config{
 public:

	 string hostname;
	 int rank;
	 int size;

	 string configFilename;
	 string datafilename;
	 string modelfilename;
	 bool solve;
	 string solvetype;
	 string ifacetype;

	 string root_stage_parent_name;
	 string node_dummy_prefix;
	 string stage_dummy_prefix;
	 string var_det_prefix;

	 bool assertion;
	 string logdir;
	 bool writeMPS;
	 string mpsFile;
	 bool writeMatlab;
	 string matlabFile;
	 bool logEM;
	 bool logBlock;
	 bool logModel; //!< Controls if the model parser should log
	 string modfile_suffix;
	 string emfile_suffix;

	 static Config* instance();
	 static void printAll();
	 static void initConfig(string& conf_filename);
	 static Config* singleton;


private:
	Config();

};

#endif
