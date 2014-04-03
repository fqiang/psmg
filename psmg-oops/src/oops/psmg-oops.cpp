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
#include "../sml/Sml.h"
#include "../oops/sml-oops.h"
#include "../util/global_util_functions.h"
#include "../sml/GlobalVariables.h"
#include "../metric/Statistics.h"
#include "../context/ExpandedModel.h"
#include "../model/AmplModel.h"
#include "oops/parutil.h"

using namespace std;


/* ----------------------------------------------------------------------------
main
This is the main method for PSMG with OOPS compilation
---------------------------------------------------------------------------- */
//int main(int argc, char **argv)
//{
//	int status = InitLippPar(argc, argv);
//	if (status != MPI_SUCCESS)
//	{
//		cerr<<"Could not Initialize MPI..."<<endl;
//		exit(1);
//	}
//
//	//MPI::Init();
//	MPI::Intercomm comm;
//	comm = MPI::COMM_WORLD;
//	char hostname[128];
//	hostname[127] = '\0';
//	gethostname(hostname,127);
//
//	GlobalVariables::hostname = hostname;
//	GlobalVariables::rank = comm.Get_rank();
//	GlobalVariables::size = comm.Get_size();
//	status = analyseOptions(argc, argv);
//	GlobalVariables::debugPrintAll();
//
//	if (status)
//	{
//		return status;
//	}
//
//	LOG("Running PSMG with OOPS");
//	LOG("Process started on host["<<GlobalVariables::hostname<<"]");
//	TIMER_START("SML_PARSE_MODEL");
//	Sml::instance()->processModelfile();
//	TIMER_STOP("SML_PARSE_MODEL");
//
//	if(GlobalVariables::logParseModel) {
//		AmplModel::root->dump("logModel.dat");
//	}
//
//#ifdef MEM
//	unsigned long mem_size_flat = 0;
//	AmplModel::root->calculateMemoryUsage(mem_size_flat);
//	cout<<"["<<GlobalVariables::rank<<"/"<<GlobalVariables::size<<"]-Flat AmplModel Memory Usage Size ["<<mem_size_flat<<"] bytes"<<endl;
//#endif
//
//	TIMER_START("ANALYSIS_CONSTRAINTS");
//	Sml::instance()->formulateConstraints();
//	TIMER_STOP("ANALYSIS_CONSTRAINTS");
//
//	TIMER_START("SML_EM2_GENERATION");
//	Sml::instance()->generateExpandedModel();
//	Sml::instance()->resetContextTree();
//	assert(ExpandedModel::root->ctx != NULL);
//	TIMER_STOP("SML_EM2_GENERATION");
//
//#ifdef MEM
//	unsigned long mem_size_em2 = 0;
//	unsigned long mem_size_ctx = 0;
//	ExpandedModel::root->calculateMemoryUsage(mem_size_em2,mem_size_ctx);
//	cout<<"["<<GlobalVariables::rank<<"/"<<GlobalVariables::size<<"]-Structure Memory Usage Size Before Solve ["<<mem_size_em2<<"] bytes"<<endl;
//	cout<<"["<<GlobalVariables::rank<<"/"<<GlobalVariables::size<<"]-Data Memory Usage Size Before Solve ["<<mem_size_ctx<<"] bytes"<<endl;
//#endif
//
//	assert(ExpandedModel::root != NULL);
//	TIMER_START("SML_OOPS_DRIVER");
//	SML_OOPS_driver(ExpandedModel::root);
//	TIMER_STOP("SML_OOPS_DRIVER");
//
//#ifdef MEM
//	mem_size_em2=0;
//	mem_size_ctx=0;
//	ExpandedModel::root->calculateMemoryUsage(mem_size_em2,mem_size_ctx);
//	cout<<"["<<GlobalVariables::rank<<"/"<<GlobalVariables::size<<"]-Structure Memory Usage Size After Solve ["<<mem_size_em2<<"] bytes"<<endl;
//	cout<<"["<<GlobalVariables::rank<<"/"<<GlobalVariables::size<<"]-Data Memory Usage Size After Solve ["<<mem_size_ctx<<"] bytes"<<endl;
//#endif
//
//
////	Feng -- please read comments in sml-oops.cpp
////				for uploadsolution function which is not yet designed
////	em2->outputSolution(cout,0);
//
//	delete ExpandedModel::root;
//	delete AmplModel::root;
//
//	Sml::deleteInstance();
//	TIMER_LIST;
//	TIMER_RESET;
//
//	cout<<"["<<GlobalVariables::rank<<"/"<<GlobalVariables::size<<"] - GetNzJac["<<Statistics::numGetNzJacCall<<"]"<<endl;
//	cout<<"["<<GlobalVariables::rank<<"/"<<GlobalVariables::size<<"] - GetJac["<<Statistics::numGetJacCall<<"]"<<endl;
//
//	MPI_Barrier(MPI_COMM_WORLD);
//	MPI::Finalize();
//	return 0;
//}
