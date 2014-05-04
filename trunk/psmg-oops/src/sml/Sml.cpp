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
#include "../sml/Sml.h"
#include "../metric/Stat.h"
#include "../model/AmplModel.h"
#include "../context/ModelContext.h"
#include "../context/Block.h"
#include "../oops/sml-oops.h"
#include "mpi.h"
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <vector>
#include <string>
#include "oops/parutil.h"

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
	FILE *inputFile = fopen(GV(modelfilename).c_str(),"r");
	if (!inputFile)
	{
		cerr << "ERROR: Cannot open file '" << GV(modelfilename) << "'.\n";
	}
	fclose(inputFile);

	// check that we can access the datafile, otherwise we get an ugly message
	// from amplsolver in case the file cannot be accessed
	inputFile = fopen(GV(datafilename).c_str(), "r");
	if (!inputFile)
	{
		cerr << "ERROR: Cannot open file: '" << GV(datafilename) << "'.\n";
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
	AmplModel::root->splitConstraints();  //split constraints according to model levels
	ExpandedModel::root = AmplModel::root->createExpandedModel("",NULL,"",NULL);
	LOG("============== END ExpandedModel2 Generation =============================");
}

void Sml::resetContextTree()
{
	LOG("============== resetContextTree =============================");
	ExpandedModel::root->clearAllContextTreeKeepRoot();
	LOG("============== END resetContextTree =============================");
}

void Sml::logEM(string filename="")
{
	LOG("============== printEMStructure =============================");
	string line="   ";
	if(filename.compare("")==0){
		ExpandedModel::root->logEMRecursive(line,cout);
	}
	else{
		ofstream file(filename.c_str(),ofstream::out);
		ExpandedModel::root->logEMRecursive(line,file);
	}
	LOG("============== END printEMStructure =============================");
}

//
//void Sml::testInterfaceLocal2(ExpandedModel* emrow)
//{
//	Block* block = emrow->getBlockLocal();
//
//	BOOST_FOREACH(ExpandedModel* em, block->ems)
//	{
//		uint numPriVar = em->getNLocalVars();
//		double elts[numPriVar];
//		for(uint i=0;i<numPriVar;i++)
//		{
//			elts[i] = 0.1*i+0.1;
//		}
//		em->update_primal_var_soln(elts);
//	}
//}
//
//void Sml::testInterfaceLocal1(ExpandedModel* emrow,ExpandedModel* emcol )
//{
//	//update local/primal variable values before calling Local Interface methods
//	this->testInterfaceLocal2(emrow);
//
//	uint nr, nc, nz;
//	//for emrow x emcol block  -- normal matrix block
//	nr = emrow->getNLocalCons();
//	nc = emcol->getNLocalVars();
//
//	//Objective gradient declared by (objective from emrow) X (variables from emcol) ~ ie. [either 1 X emcol.numLocalVar] or [0 X emcol.numLocalVar]
//	vector<double> ograd;
//	emrow->obj_grad_local(emcol,ograd);
//	assert(emrow->model->obj_comp==NULL || ograd.size() == nc);
//	LOG("Objective gradient vector  -- ograd size["<<ograd.size()<<"]   ------ ["<<emrow->name<<"] X ["<<emcol->name<<"]");
//
//	//Objective Hessian declared by (variables from emrow) X (variables from emcol)
//	nz = emrow->nz_obj_hess_local(emcol);
//	boost::numeric::ublas::compressed_matrix<double> objhess;
//	emrow->obj_hess_local(emcol,objhess);
//	assert(objhess.size1()==emrow->getNLocalVars() && objhess.size2() == nc);
//	LOG("Objective Hessian block  -- ["<<objhess.size1()<<" X "<<objhess.size2()<<"]   -- nz["<<nz<<"]  -------  ["<<emrow->name<<"] X ["<<emcol->name<<"]");
//	objhess.clear();
//
//	//Constraints Jacobian submatrix declared by (constraints from emrow) X (variables from emcol)
//	nz = emrow->nz_cons_jacobs_local(emcol);
//	boost::numeric::ublas::compressed_matrix<double> consjac;
//	emrow->cons_jacobs_local(emcol,consjac);
//	assert(consjac.size1()==nr && consjac.size2() == nc);
//	LOG("Jacobian block  -- ["<<consjac.size1()<<" X "<<consjac.size2()<<"]   -- nz["<<nz<<"]  ------ ["<<emrow->name<<"] X ["<<emcol->name<<"]");
//	consjac.clear();
//
//	//Constraints Hessian submatrix declared by (variables from emrow) X (variables from emcol)
//	nz = emrow->nz_cons_hess_local(emcol);
//	boost::numeric::ublas::compressed_matrix<double> conshess;
//	emrow->cons_hess_local(emcol,conshess);
//	assert(conshess.size1()==emrow->numLocalVars && conshess.size2() == nc);  //hessian is square matrix
//	LOG("Hessian block  -- ["<<conshess.size1()<<" X "<<conshess.size2()<<"]   -- nz["<<nz<<"]  -------  ["<<emrow->name<<"] X ["<<emcol->name<<"]");
//	conshess.clear();
//}
//
//void Sml::testInterfaceLocal0(ExpandedModel* root,ExpandedModel* curr)
//{
//	this->testInterfaceLocal1(root,curr);
//	this->testInterfaceLocal1(curr,root);
//	vector<ExpandedModel*>::iterator it = curr->children.begin();
//	for(it = curr->children.begin();it != curr->children.end();it++) {
//		this->testInterfaceLocal0(root, *it);
//	}
//}
//
//void Sml::testInterfaceLocal(ExpandedModel* root)
//{
//	vector<ExpandedModel*>::iterator it = root->children.begin();
//	for(;it!=root->children.end();it++)
//	{
//		this->testInterfaceLocal0(root,(*it));
//		this->testInterfaceLocal(*it);
//	}
//
//	//update local/primal variable values before calling Local Interface methods
//	this->testInterfaceLocal2(root);
//
//	//Object and Constraint Evaluation Local Inteface calls
//	uint nr, nc;
//	nr = root->getNLocalCons();
//	nc = root->getNLocalVars();
//
//	//for diagonal blocks
//	double oval;
//	root->obj_feval_local(oval);
//	LOG("Objective Eval -- ["<<"< oval["<<oval<<"]  ------ ["<<root->name<<"] X ["<<root->name<<"]");
//	assert(root->model->obj_comp!=NULL || (isnan(oval) && root->model->obj_comp == NULL));
//
//	vector<double> fval;
//	root->cons_feval_local(fval);
//	assert(fval.size()==nr);
//	LOG("Constraint Eval -- ["<<"<fval size ["<<fval.size()<<"]  ------ ["<<root->name<<"] X ["<<root->name<<"]");
//	fval.clear();
//
//	//calling Local inteface calls for Gradient and Hessian of the diagonal block
//	this->testInterfaceLocal1(root,root);
//}

//oops properties
FILE *globlog = NULL;
FILE *printout = stdout;

int main(int argc, char **argv)
{
	int parstatus = InitLippPar(argc, argv);
	if (parstatus != MPI_SUCCESS)
	{
		cerr<<"Could not Initialize MPI..."<<endl;
		exit(1);
	}

	string default_conf = "./psmg_conf.xml";
	Config::initConfig(default_conf);
	for(int i=1;i<argc;i++)
	{ //if provide any model and data files
		GV(modelfilename) = argv[i];
		GV(datafilename) = argv[++i];
	}
	Config::printAll();


	LOG("Process started on host["<<GV(hostname)<<"]");
	TIMER_START("SML_PARSE_MODEL");
	Sml::instance()->processModelfile();
	TIMER_STOP("SML_PARSE_MODEL");

	if(GV(logModel)) {
		string mfile = GV(logdir)+GV(logModelFile);
		AmplModel::root->logModel(mfile.c_str());
	}

	unsigned long mem_size_flat = 0;
	AmplModel::root->calculateMemoryUsage(mem_size_flat);
	cout<<"["<<GV(rank)<<"/"<<GV(size)<<"]-Flat AmplModel Memory Usage Size ["<<mem_size_flat<<"] bytes"<<endl;

	TIMER_START("SML_EM2_GENERATION");
	Sml::instance()->generateExpandedModel();
	if(GV(logModel)) {
			string mfile = GV(logdir)+GV(logModelFile) + "_partial";
			AmplModel::root->logModel(mfile.c_str());
	}
	Sml::instance()->resetContextTree();
	assert(ExpandedModel::root->ctx != NULL);
	TIMER_STOP("SML_EM2_GENERATION");

	unsigned long mem_size_em2 = 0;
	unsigned long mem_size_ctx = 0;
	ExpandedModel::root->calculateMemoryUsage(mem_size_em2,mem_size_ctx);
	cout<<"["<<GV(rank)<<"/"<<GV(size)<<"]-Structure Memory Usage Size Before Solve ["<<mem_size_em2<<"] bytes"<<endl;
	cout<<"["<<GV(rank)<<"/"<<GV(size)<<"]-Data Memory Usage Size Before Solve ["<<mem_size_ctx<<"] bytes"<<endl;

	assert(ExpandedModel::root != NULL);

//	//Testing Section start
//	Sml::instance()->testInterfaceLocal(ExpandedModel::root);
//	//Testing Section end

	SML_OOPS_driver_LP(ExpandedModel::root);

	if(GV(logEM)){
		Sml::instance()->logEM(GV(logdir)+GV(logEMFile));
	}

	mem_size_em2=0;
	mem_size_ctx=0;
	ExpandedModel::root->calculateMemoryUsage(mem_size_em2,mem_size_ctx);
	cout<<"["<<GV(rank)<<"/"<<GV(size)<<"]-Structure Memory Usage Size After Solve ["<<mem_size_em2<<"] bytes"<<endl;
	cout<<"["<<GV(rank)<<"/"<<GV(size)<<"]-Data Memory Usage Size After Solve ["<<mem_size_ctx<<"] bytes"<<endl;



	delete ExpandedModel::root;
	delete AmplModel::root;
	Sml::deleteInstance();

	TIMER_LIST;
	TIMER_RESET;

	Stat::logStatistics(cout);

	MPI_Barrier(MPI_COMM_WORLD);
	MPI::Finalize();
	return 0;
}
