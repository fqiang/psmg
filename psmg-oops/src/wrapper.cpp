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

#include <iostream>
#include <fstream>
#include <cstring> // for strcmp() and strncmp()
#include <cstdlib> // for exit()
#include "mpi.h"
#include "./sml/Sml.h"
#include "./oops/sml-oops.h"
#include "./util/global_util_functions.h"
#include "./util/GlobalVariables.h"
#include "./metric/Statistics.h"
#include "./context/ExpandedModel2.h"
#include "oops/parutil.h"

using namespace std;

const string progname = "smloops";

void writeHelp(ostream& out, const string& programname) {
   out << "Syntax:" << endl;
   out << "   " << programname
       << " [OPTIONS] modelfile datafile\n\n";
   out << "Option summary:" << endl;
   out << " -d                  Enables debug information when reading model file." << endl;
   out << " --help              Displays this help information." << endl;
   out << " --output=outfile," << endl;
   out << " -o outfile        Write solution to file outfile." << endl;
   out << " -m 					Enable output matlab matrix file"<<endl;
   out << " -M					Enable output MPS matrix file"<<endl;
   out << " -s 					Enable calling solver to solve the problem" <<endl;
   out << " modelfile           File containing SML model." << endl;
   out << " datafile            File containing SML data." << endl;
}

int analyseOptions(int argc, char **argv) {
	int found = 0;
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-d") == 0)
		{
			GlobalVariables::debug = true;
		}
		else if (strcmp(argv[i], "--help") == 0)
		{
			writeHelp(cout, progname);
			exit(0);
		}
		else if (strcmp(argv[i], "-o") == 0)
		{
			if (i + 1 == argc)
			{
				cerr << "-o supplied without filename" << endl;
				return 1;
			}
			GlobalVariables::outfilename = argv[++i];
			if (GlobalVariables::outfilename.at(0) == '-')
			{
				cerr << "-o supplied without filename" << endl;
				return 1;
			}
		}
		else if (strncmp(argv[i], "--output=", 9) == 0)
		{
			GlobalVariables::outfilename = (argv[i] + 9);
		}
		else if(strncmp(argv[i], "--with-ampl=",12)==0)
		{
			GlobalVariables::amplcommand = (argv[i] + 12);
		}
		else if(strcmp(argv[i],"-p")==0)
		{
			GlobalVariables::degreeParallel = atoi(argv[++i]);
		}
		else if(strcmp(argv[i],"-m")==0)
		{
			GlobalVariables::writeMatlab = true;
		}
		else if(strcmp(argv[i],"-M")==0)
		{
			GlobalVariables::writeMPS = true;
		}
		else if(strcmp(argv[i],"-s")==0)
		{
			GlobalVariables::solve = true;
		}
		//======================================================
		else if (*(argv[i]) == '-')
		{
			cerr << "Unrecognised option '" << argv[i] << "'" << endl;
			return 1;
		}
		else
		{
			if (found == 0) {
				// first proper argument is the model file to read
				GlobalVariables::modelfilename = argv[i];
				found++;
			} else if (found == 1) {
				// next one is data file
				GlobalVariables::datafilename = argv[i];
				found++;
			} else {
				cerr << "ERROR: too many filenames." << endl;
				return 1;
			}
		}
	}

	if (GlobalVariables::modelfilename == "" || GlobalVariables::datafilename == "") {
		cerr << "ERROR: both modelfile and datafile must be supplied." << endl << endl;
		return 1;
	}
	return 0;
}

/* ----------------------------------------------------------------------------
main
---------------------------------------------------------------------------- */
int main(int argc, char **argv)
{
	int status = InitLippPar(argc, argv);
	if (status != MPI_SUCCESS)
	{
		cerr<<"Could not Initialize MPI..."<<endl;
		exit(1);
	}

	//MPI::Init();
	MPI::Intercomm comm;
	comm = MPI::COMM_WORLD;
	char hostname[128];
	hostname[127] = '\0';
	gethostname(hostname,127);

	GlobalVariables::hostname = hostname;
	GlobalVariables::rank = comm.Get_rank();
	GlobalVariables::size = comm.Get_size();
	status = analyseOptions(argc, argv);
	GlobalVariables::debugPrintAll();

	if (status)
	{
		return status;
	}

	LOG("Running PSMG with OOPS");
	LOG("Process started on host["<<GlobalVariables::hostname<<"]");
	assert (GlobalVariables::degreeParallel >= 1);
	Sml::print_copy_right(cout);
	TIMER_START("SML_PARSE_MODEL");
	Sml::instance()->process_model_file();
	TIMER_STOP("SML_PARSE_MODEL");
	LOG_SYS_MEM("AfterParseModel");

	if(GlobalVariables::logParseModel) {
		AmplModel::root->dump("logModel.dat");
	}

#ifdef MEM
	unsigned long mem_size_flat = 0;
	AmplModel::root->calculateMemoryUsage(mem_size_flat);
	cout<<"["<<GlobalVariables::rank<<"/"<<GlobalVariables::size<<"]-Flat AmplModel Memory Usage Size ["<<mem_size_flat<<"] bytes"<<endl;
#endif

	TIMER_START("SML_EM2_GENERATION");
	ExpandedModelAbstract *em2 = Sml::instance()->generate_em2();
	TIMER_STOP("SML_EM2_GENERATION");
	LOG_SYS_MEM("AfterEM2Generation");

#ifdef MEM
	unsigned long mem_size_em2 = 0;
	unsigned long mem_size_ctx = 0;
	em2->calculateMemoryUsage(mem_size_em2,mem_size_ctx);
	cout<<"["<<GlobalVariables::rank<<"/"<<GlobalVariables::size<<"]-Structure Memory Usage Size Before Solve ["<<mem_size_em2<<"] bytes"<<endl;
	cout<<"["<<GlobalVariables::rank<<"/"<<GlobalVariables::size<<"]-Data Memory Usage Size Before Solve ["<<mem_size_ctx<<"] bytes"<<endl;
#endif



	if(!em2)
		return 1;
	TIMER_START("SML_OOPS_DRIVER");
	SML_OOPS_driver(em2);
	TIMER_STOP("SML_OOPS_DRIVER");

#ifdef MEM
	mem_size_em2=0;
	mem_size_ctx=0;
	em2->calculateMemoryUsage(mem_size_em2,mem_size_ctx);
	cout<<"["<<GlobalVariables::rank<<"/"<<GlobalVariables::size<<"]-Structure Memory Usage Size After Solve ["<<mem_size_em2<<"] bytes"<<endl;
	cout<<"["<<GlobalVariables::rank<<"/"<<GlobalVariables::size<<"]-Data Memory Usage Size After Solve ["<<mem_size_ctx<<"] bytes"<<endl;
#endif

	LOG_SYS_MEM("AfterSMLOOPSDriver");

//	Feng -- please read comments in sml-oops.cpp
//				for uploadsolution function which is not yet designed
//	em2->outputSolution(cout,0);

	delete em2;
	delete AmplModel::root;

	Sml::deleteInstance();
	TIMER_LIST;
	TIMER_RESET;

	cout<<"["<<GlobalVariables::rank<<"/"<<GlobalVariables::size<<"] - GetNzJac["<<Statistics::numGetNzJacCall<<"]"<<endl;
	cout<<"["<<GlobalVariables::rank<<"/"<<GlobalVariables::size<<"] - GetJac["<<Statistics::numGetJacCall<<"]"<<endl;

	MPI_Barrier(MPI_COMM_WORLD);
	MPI::Finalize();
	return 0;
}
