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

#include "../sml/Sml.h"
//#include "../sml/backend.h"
#include "../model/AmplModel.h"
#include "../context/ModelContext.h"
#include "../util/global_util_functions.h"
#include "../sml/GlobalVariables.h"
#include <iostream>
#include <sys/stat.h>
#include <time.h>
#include <vector>
#include <string>

#ifdef HAVE_DIRECT_H
#include <direct.h> // for mkdir() under MinGW
#endif

using namespace std;

extern int parse_model();

Sml* Sml::_sml = NULL;

Sml* Sml::instance()
{
	if(!_sml)
	{
		_sml = new Sml();
	}
	return _sml;
}

void Sml::deleteInstance()
{
	if(_sml)
	{
		delete _sml;
		_sml = NULL;
	}
}

Sml::Sml()
{
	LOG("Sml processor created...scriptList["<<scriptNameList.size()<<"]");
}

Sml::~Sml()
{
	LOG("Sml destructor called...");
}

void Sml::print_copy_right(ostream &out)
{
	out << "Parallel Problem Generator for Structure-conveying Modelling Language, version - trunk" << endl;
	out << "(c) 2013 Feng Qiang, Andreas Grothey, University of Edinburgh." << endl;
	out << "Released under LGPL v3" << endl;
	out << "CPU:: CLOCKS_PER_SEC ["<<CLOCKS_PER_SEC<<"]"<<endl;
}

int Sml::sml_gen_pre_checks()
{
	FILE *inputFile = fopen(GlobalVariables::modelfilename.c_str(),"r");
	if (!inputFile)
	{
		cerr << "ERROR: Cannot open file '" << GlobalVariables::modelfilename << "'.\n";
     	return 1;
   	}
	fclose(inputFile);

   	// check that we can access the datafile, otherwise we get an ugly message
   	// from amplsolver in case the file cannot be accessed
   	inputFile = fopen(GlobalVariables::datafilename.c_str(), "r");
   	if (!inputFile)
   	{
   		cerr << "Cannot open file: '" << GlobalVariables::datafilename << "'.\n";
     	return 2;
   	}
   	fclose(inputFile);

   	return 0;
}

void Sml::process_model_file()
{
	int errcode = Sml::sml_gen_pre_checks();
	assert(errcode ==0);
	errcode = parse_model();
	assert(errcode ==0);
}

void Sml::generate_em2()
{
	LOG("============== ExpandedModel2 Generation =============================");
	//step 1: create expandedmodel recursively
	AmplModel::root->createExpandedModel2(NULL,NULL);
	LOG("============== END ExpandedModel2 Generation =============================");
}

void Sml::analyse_constraints()
{
	LOG("============== analyse_constraints =============================");
	AmplModel::root->analyseConstraints();
	LOG("============== END analyse_constraints =============================");
}
