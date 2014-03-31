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

#include "../util/global_util_functions.h"
#include "../sml/GlobalVariables.h"
#include "../sml/Sml.h"
#include "../metric/Statistics.h"
#include "../model/AmplModel.h"
#include "../context/ModelContext.h"
#include "../context/Block.h"
#include "mpi.h"
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <vector>
#include <string>

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
	LOG("PSMG processor created...");
	cout << "Parallel Problem Generator for Structure-conveying Modelling Language, version - trunk" << endl;
	cout << "(c) 2013 Feng Qiang, Andreas Grothey, University of Edinburgh." << endl;
	cout << "Released under LGPL v3" << endl;
	cout << "CPU:: CLOCKS_PER_SEC ["<<CLOCKS_PER_SEC<<"]"<<endl;
}

Sml::~Sml()
{
	LOG("Sml destructor called...");
}

void Sml::processModelfile()
{
	FILE *inputFile = fopen(GlobalVariables::modelfilename.c_str(),"r");
	if (!inputFile)
	{
		cerr << "ERROR: Cannot open file '" << GlobalVariables::modelfilename << "'.\n";
	}
	fclose(inputFile);

	// check that we can access the datafile, otherwise we get an ugly message
	// from amplsolver in case the file cannot be accessed
	inputFile = fopen(GlobalVariables::datafilename.c_str(), "r");
	if (!inputFile)
	{
		cerr << "ERROR: Cannot open file: '" << GlobalVariables::datafilename << "'.\n";
	}
	fclose(inputFile);

	struct stat s;
	stat(GV(logdir).c_str(),&s);
	if(S_ISDIR(s.st_mode))
	{
		LOG("log directory exist");
	}
	else{
		int errcode = mkdir(GV(logdir).c_str(),S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		assert(errcode == 0);
	}

	int errcode = parse_model();
	assert(errcode ==0);
}

void Sml::generateExpandedModel()
{
	LOG("============== ExpandedModel2 Generation =============================");
	//step 1: create expandedmodel recursively
	ExpandedModel::root = AmplModel::root->createExpandedModel("",NULL,"",NULL);
	LOG("============== END ExpandedModel2 Generation =============================");
}

void Sml::resetContextTree()
{
	LOG("============== resetContextTree =============================");
	ExpandedModel::root->clearAllContextTreeKeepRoot();
	LOG("============== END resetContextTree =============================");
}

void Sml::formulateConstraints()
{
	LOG("============== formulateConstraints =============================");
	AmplModel::root->settingUpLevels(0);
	LOG("Max declared subproblem level -- "<<AmplModel::MAX_LEVEL);
	AmplModel::root->formulateConstraints();
	LOG("============== END formulateConstraints =============================");
}

void Sml::printEMStructure(string filename="")
{
	LOG("============== printEMStructure =============================");
	string line="   ";
	if(filename.compare("")==0){
		ExpandedModel::root->printEMRecursive(line,cout);
	}
	else{
		ofstream file(filename.c_str(),ofstream::out);
		ExpandedModel::root->printEMRecursive(line,file);
	}
	LOG("============== END printEMStructure =============================");
}

void Sml::writeHelp(ostream& out)
{
   out << "Syntax:" << endl;
   out << "   " << GlobalVariables::executable<< " [OPTIONS] modelfile datafile\n\n";
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

int Sml::analyseOptions(int argc, char **argv)
{
	int found = 0;
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-d") == 0)
		{
			GlobalVariables::debug = true;
		}
		else if (strcmp(argv[i], "--help") == 0)
		{
			Sml::writeHelp(cout);
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

void Sml::testInterfaceLocal2(ExpandedModel* emrow)
{
	Block* block = emrow->getBlockLocal();

	BOOST_FOREACH(ExpandedModel* em, block->ems)
	{
		uint numPriVar = em->getNLocalVars();
		double elts[numPriVar];
		for(uint i=0;i<numPriVar;i++)
		{
			elts[i] = 0.1*i+0.1;
		}
		em->update_primal_var_soln(elts);
	}
}

void Sml::testInterfaceLocal1(ExpandedModel* emrow,ExpandedModel* emcol )
{
	//update local/primal variable values before calling Local Interface methods
	this->testInterfaceLocal2(emrow);

	uint nr, nc, nz;
	//for emrow x emcol block  -- normal matrix block
	nr = emrow->getNLocalCons();
	nc = emcol->getNLocalVars();

	//Objective gradient declared by (objective from emrow) X (variables from emcol) ~ ie. [either 1 X emcol.numLocalVar] or [0 X emcol.numLocalVar]
	vector<double> ograd;
	emrow->obj_grad_local(emcol,ograd);
	assert(emrow->model->obj_comp==NULL || ograd.size() == nc);
	LOG("Objective gradient vector  -- ograd size["<<ograd.size()<<"]   ------ ["<<emrow->name<<"] X ["<<emcol->name<<"]");

	//Objective Hessian declared by (variables from emrow) X (variables from emcol)
	nz = emrow->nz_obj_hess_local(emcol);
	boost::numeric::ublas::compressed_matrix<double> objhess;
	emrow->obj_hess_local(emcol,objhess);
	assert(objhess.size1()==emrow->getNLocalVars() && objhess.size2() == nc);
	LOG("Objective Hessian block  -- ["<<objhess.size1()<<" X "<<objhess.size2()<<"]   -- nz["<<nz<<"]  -------  ["<<emrow->name<<"] X ["<<emcol->name<<"]");
	objhess.clear();

	//Constraints Jacobian submatrix declared by (constraints from emrow) X (variables from emcol)
	nz = emrow->nz_cons_jacobs_local(emcol);
	boost::numeric::ublas::compressed_matrix<double> consjac;
	emrow->cons_jacobs_local(emcol,consjac);
	assert(consjac.size1()==nr && consjac.size2() == nc);
	LOG("Jacobian block  -- ["<<consjac.size1()<<" X "<<consjac.size2()<<"]   -- nz["<<nz<<"]  ------ ["<<emrow->name<<"] X ["<<emcol->name<<"]");
	consjac.clear();

	//Constraints Hessian submatrix declared by (variables from emrow) X (variables from emcol)
	nz = emrow->nz_cons_hess_local(emcol);
	boost::numeric::ublas::compressed_matrix<double> conshess;
	emrow->cons_hess_local(emcol,conshess);
	assert(conshess.size1()==emrow->numLocalVars && conshess.size2() == nc);  //hessian is square matrix
	LOG("Hessian block  -- ["<<conshess.size1()<<" X "<<conshess.size2()<<"]   -- nz["<<nz<<"]  -------  ["<<emrow->name<<"] X ["<<emcol->name<<"]");
	conshess.clear();
}

void Sml::testInterfaceLocal0(ExpandedModel* root,ExpandedModel* curr)
{
	this->testInterfaceLocal1(root,curr);
	this->testInterfaceLocal1(curr,root);
	vector<ExpandedModel*>::iterator it = curr->children.begin();
	for(it = curr->children.begin();it != curr->children.end();it++) {
		this->testInterfaceLocal0(root, *it);
	}
}

void Sml::testInterfaceLocal(ExpandedModel* root)
{
	vector<ExpandedModel*>::iterator it = root->children.begin();
	for(;it!=root->children.end();it++)
	{
		this->testInterfaceLocal0(root,(*it));
		this->testInterfaceLocal(*it);
	}

	//update local/primal variable values before calling Local Interface methods
	this->testInterfaceLocal2(root);

	//Object and Constraint Evaluation Local Inteface calls
	uint nr, nc;
	nr = root->getNLocalCons();
	nc = root->getNLocalVars();

	//for diagonal blocks
	double oval;
	root->obj_feval_local(oval);
	LOG("Objective Eval -- ["<<"< oval["<<oval<<"]  ------ ["<<root->name<<"] X ["<<root->name<<"]");
	assert(root->model->obj_comp!=NULL || (isnan(oval) && root->model->obj_comp == NULL));

	vector<double> fval;
	root->cons_feval_local(fval);
	assert(fval.size()==nr);
	LOG("Constraint Eval -- ["<<"<fval size ["<<fval.size()<<"]  ------ ["<<root->name<<"] X ["<<root->name<<"]");
	fval.clear();

	//calling Local inteface calls for Gradient and Hessian of the diagonal block
	this->testInterfaceLocal1(root,root);
}

int main(int argc, char **argv)
{
	MPI::Init();
	MPI::Intercomm comm;
	comm = MPI::COMM_WORLD;
	char hostname[128];
	hostname[127] = '\0';
	gethostname(hostname,127);

	GV(hostname) = hostname;
	GV(rank) = comm.Get_rank();
	GV(size) = comm.Get_size();
	GV(executable) = "psmg-test";
	int status = Sml::instance()->analyseOptions(argc, argv);
	GlobalVariables::printAll();

	if (status)
	{
		return status;
	}

	LOG("Process started on host["<<GlobalVariables::hostname<<"]");
	TIMER_START("SML_PARSE_MODEL");
	Sml::instance()->processModelfile();
	TIMER_STOP("SML_PARSE_MODEL");

	if(GlobalVariables::logParseModel) {
		string mfile = GV(logdir)+"logModel.dat";
		AmplModel::root->logModel(mfile.c_str());
	}

	unsigned long mem_size_flat = 0;
	AmplModel::root->calculateMemoryUsage(mem_size_flat);
	cout<<"["<<GlobalVariables::rank<<"/"<<GlobalVariables::size<<"]-Flat AmplModel Memory Usage Size ["<<mem_size_flat<<"] bytes"<<endl;

	TIMER_START("ANALYSIS_CONSTRAINTS");
	Sml::instance()->formulateConstraints();
	TIMER_STOP("ANALYSIS_CONSTRAINTS");

	TIMER_START("SML_EM2_GENERATION");
	Sml::instance()->generateExpandedModel();
	Sml::instance()->resetContextTree();
	assert(ExpandedModel::root->ctx != NULL);
	TIMER_STOP("SML_EM2_GENERATION");

	unsigned long mem_size_em2 = 0;
	unsigned long mem_size_ctx = 0;
	ExpandedModel::root->calculateMemoryUsage(mem_size_em2,mem_size_ctx);
	cout<<"["<<GlobalVariables::rank<<"/"<<GlobalVariables::size<<"]-Structure Memory Usage Size Before Solve ["<<mem_size_em2<<"] bytes"<<endl;
	cout<<"["<<GlobalVariables::rank<<"/"<<GlobalVariables::size<<"]-Data Memory Usage Size Before Solve ["<<mem_size_ctx<<"] bytes"<<endl;

	assert(ExpandedModel::root != NULL);

	//Testing Section start
	Sml::instance()->testInterfaceLocal(ExpandedModel::root);
	//Testing Section end

	Sml::instance()->printEMStructure(GV(logdir)+"logEM.dat");

	mem_size_em2=0;
	mem_size_ctx=0;
	ExpandedModel::root->calculateMemoryUsage(mem_size_em2,mem_size_ctx);
	cout<<"["<<GlobalVariables::rank<<"/"<<GlobalVariables::size<<"]-Structure Memory Usage Size After Solve ["<<mem_size_em2<<"] bytes"<<endl;
	cout<<"["<<GlobalVariables::rank<<"/"<<GlobalVariables::size<<"]-Data Memory Usage Size After Solve ["<<mem_size_ctx<<"] bytes"<<endl;



	delete ExpandedModel::root;
	delete AmplModel::root;
	Sml::deleteInstance();

	TIMER_LIST;
	TIMER_RESET;

	Statistics::logStatistics(cout);

	MPI_Barrier(MPI_COMM_WORLD);
	MPI::Finalize();
	return 0;
}
