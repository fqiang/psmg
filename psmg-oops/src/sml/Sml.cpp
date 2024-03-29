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

#include "../util/util.h"
#include "../util/compile_marco.h"

#include "Sml.h"
#include "Config.h"
#include "../metric/Stat.h"
#include "../model/AmplModel.h"
#include "../context/ModelContext.h"
#include "../context/ExpandedModel.h"
#include "../oops/sml-oops.h"
#include "mpi.h"
#include <iostream>
#include <sstream>
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
	TRACE("PSMG processor created...");
}

Sml::~Sml()
{
	TRACE("Sml destructor called...");
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
		TRACE("log directory exist");
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
	TRACE("============== ExpandedModel2 Generation =============================");
	//step 1: create expandedmodel recursively
	AmplModel::root->splitConstraints();  //split constraints according to model levels
	ExpandedModel::root = AmplModel::root->createExpandedModel("",NULL,"",NULL);
	int rbeg = 0, cbeg = 0;
	ExpandedModel::root->setIndex(rbeg, cbeg);
	ExpandedModel::n_col = cbeg;
	ExpandedModel::n_row = rbeg;
	TRACE("Total row ["<<rbeg <<"]  col["<<cbeg<<"]");
	TRACE("============== END ExpandedModel2 Generation =============================");
}

void Sml::resetContextTree()
{
	TRACE("============== resetContextTree =============================");
	ExpandedModel::root->dropCtxRecKeepRoot();
	TRACE("============== END resetContextTree =============================");
}

void Sml::logEM(string filename="")
{
	TRACE("============== printEMStructure =============================");
	string line="   ";
	if(filename.compare("")==0){
		ExpandedModel::root->logEMRecursive(line,cout);
	}
	else{
		ofstream file(filename.c_str(),ofstream::out);
		ExpandedModel::root->logEMRecursive(line,file);
	}
	TRACE("============== END printEMStructure =============================");
}

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

	{
		string default_conf = "./psmg_conf.xml";
		Config::initConfig(default_conf);

		if(GV(solvetype).compare("lp") == 0) {
			ExpandedModel::ptype = LP;
		}
		else if(GV(solvetype).compare("qp") == 0) {
			ExpandedModel::ptype = QP;
		}
		else {
			assert(GV(solvetype).compare("nlp")==0);
			ExpandedModel::ptype = NLP;
		}

		if(GV(ifacetype).compare("local")==0){
			ExpandedModel::itype = LOCAL;
		}
		else {
			assert(GV(ifacetype).compare("distribute"));
			ExpandedModel::itype = DIST;
		}
	}

	for(int i=1;i<argc;i++)
	{ //if provide any model and data files
		GV(modelfilename) = argv[i];
		GV(datafilename) = argv[++i];
	}

	if(GV(rank)==0){
		cout << "Parallel Problem Generator for Structure-conveying Modelling Language, version - trunk" << endl;
		cout << "(c) 2013 Feng Qiang, Andreas Grothey, University of Edinburgh." << endl;
		cout << "Released under LGPL v3" << endl;
		cout << "CPU:: CLOCKS_PER_SEC ["<<CLOCKS_PER_SEC<<"]"<<endl;
		Config::printAll();
	}



	TRACE("Process started on host["<<GV(hostname)<<"]");
	TIMER_START("PARSE_MODEL");
	Sml::instance()->processModelfile();
	TIMER_STOP("PARSE_MODEL");

	//log the model file
	if(GV(logModel) && GV(rank) == 0) {
		string datname = GV(modelfilename).substr(0, GV(modelfilename).find(".", 0));
		ostringstream oss;
		oss<<GV(logdir)<<datname<<GV(modfile_suffix);
		AmplModel::root->logModel(oss.str().c_str());
	}

	unsigned long mem_size_prototype_tree = 0;
	AmplModel::root->calculateMemoryUsage(mem_size_prototype_tree);
	cout<<"["<<GV(rank)<<"/"<<GV(size)<<"]-Prototype tree Memory Usage Size ["<<mem_size_prototype_tree<<"] bytes"<<endl;

	TIMER_START("EM_GENERATION");
	Sml::instance()->generateExpandedModel();
//	assert(ExpandedModel::root->ctx != NULL);
	TIMER_STOP("EM_GENERATION");

	//log the split constrained mode file
	if(GV(logModel) && GV(rank) == 0 ) {
		string datname = GV(modelfilename).substr(0, GV(modelfilename).find(".", 0));
		ostringstream oss;
		oss<<GV(logdir)<<datname<<"_partial"<<GV(modfile_suffix);
		AmplModel::root->logModel(oss.str().c_str());
	}

//	cout<<"pausing.... "<<endl;
//	getchar();
//	cout<<"continuing.... "<<endl;
	Sml::instance()->resetContextTree();
//	cout<<"pausing.... "<<endl;
//	getchar();
//	cout<<"continuing.... "<<endl;

	unsigned long mem_size = 0;
	ExpandedModel::root->calculateMemoryUsage(mem_size);
	cout<<"["<<GV(rank)<<"/"<<GV(size)<<"]- Memory Usage Before Solve [ "<<mem_size<<" ] bytes"<<endl;

	assert(ExpandedModel::root != NULL);

//	//Testing Section start
//	Sml::instance()->testInterfaceLocal(ExpandedModel::root);
//	//Testing Section end
	AutoDiff::autodiff_setup(); //setting up tape and stack

	if(ExpandedModel::ptype == LP || ExpandedModel::ptype == QP){
		SML_OOPS_driver_LP_QP(ExpandedModel::root);
	}
	else{
		assert(ExpandedModel::ptype == NLP);
		SML_OOPS_driver_NLP(ExpandedModel::root);
	}

	if(GV(logEM)){
		string datname = GV(datafilename).substr(0, GV(datafilename).find(".", 0));
		ostringstream oss;
		oss<<GV(logdir)<<datname<<"_"<<GV(rank)<<"_"<<GV(size)<<GV(emfile_suffix);
		Sml::instance()->logEM(oss.str());
	}

	mem_size=0;
	ExpandedModel::root->calculateMemoryUsage(mem_size);
	cout<<"["<<GV(rank)<<"/"<<GV(size)<<"]- Memory Usage After Solve [ "<<mem_size<<" ] bytes"<<endl;


	AutoDiff::autodiff_cleanup();  //delete the tape and stack
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





//
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
//	TRACE("Objective gradient vector  -- ograd size["<<ograd.size()<<"]   ------ ["<<emrow->name<<"] X ["<<emcol->name<<"]");
//
//	//Objective Hessian declared by (variables from emrow) X (variables from emcol)
//	nz = emrow->nz_obj_hess_local(emcol);
//	boost::numeric::ublas::compressed_matrix<double> objhess;
//	emrow->obj_hess_local(emcol,objhess);
//	assert(objhess.size1()==emrow->getNLocalVars() && objhess.size2() == nc);
//	TRACE("Objective Hessian block  -- ["<<objhess.size1()<<" X "<<objhess.size2()<<"]   -- nz["<<nz<<"]  -------  ["<<emrow->name<<"] X ["<<emcol->name<<"]");
//	objhess.clear();
//
//	//Constraints Jacobian submatrix declared by (constraints from emrow) X (variables from emcol)
//	nz = emrow->nz_cons_jacobs_local(emcol);
//	boost::numeric::ublas::compressed_matrix<double> consjac;
//	emrow->cons_jacobs_local(emcol,consjac);
//	assert(consjac.size1()==nr && consjac.size2() == nc);
//	TRACE("Jacobian block  -- ["<<consjac.size1()<<" X "<<consjac.size2()<<"]   -- nz["<<nz<<"]  ------ ["<<emrow->name<<"] X ["<<emcol->name<<"]");
//	consjac.clear();
//
//	//Constraints Hessian submatrix declared by (variables from emrow) X (variables from emcol)
//	nz = emrow->nz_cons_hess_local(emcol);
//	boost::numeric::ublas::compressed_matrix<double> conshess;
//	emrow->cons_hess_local(emcol,conshess);
//	assert(conshess.size1()==emrow->numLocalVars && conshess.size2() == nc);  //hessian is square matrix
//	TRACE("Hessian block  -- ["<<conshess.size1()<<" X "<<conshess.size2()<<"]   -- nz["<<nz<<"]  -------  ["<<emrow->name<<"] X ["<<emcol->name<<"]");
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
//	TRACE("Objective Eval -- ["<<"< oval["<<oval<<"]  ------ ["<<root->name<<"] X ["<<root->name<<"]");
//	assert(root->model->obj_comp!=NULL || (isnan(oval) && root->model->obj_comp == NULL));
//
//	vector<double> fval;
//	root->cons_feval_local(fval);
//	assert(fval.size()==nr);
//	TRACE("Constraint Eval -- ["<<"<fval size ["<<fval.size()<<"]  ------ ["<<root->name<<"] X ["<<root->name<<"]");
//	fval.clear();
//
//	//calling Local inteface calls for Gradient and Hessian of the diagonal block
//	this->testInterfaceLocal1(root,root);
//}
