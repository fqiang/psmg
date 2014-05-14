/*
 * Config.cpp
 *
 *  Created on: 18 Feb 2013
 *      Author: s0965328
 */

#include <string>
#include "Config.h"
#include "../util/util.h"
#include "mpi.h"
#include "pugixml.hpp"
#include <cstdlib>
#include <cassert>

using namespace std;

Config* Config::singleton = NULL;

Config::Config(): hostname("UNSET_HOSTNAME"),rank(-1),size(-1),solve(false), solvetype("lp"),configFilename("./psmg_conf.xml"),
		modelfilename(""),datafilename(""),root_stage_parent_name("null"),node_dummy_prefix("n"),stage_dummy_prefix("st"),var_det_prefix(""),
		logdir("./log/"),debug(false),writeMatlab("false"),matlabFile("mat.m"),writeMPS(false),mpsFile("mps.mps"),
		logEM(true),logBlock(true),logModel(true),modfile_suffix(".mdat"),emfile_suffix(".emdat")
{

}

Config* Config::instance()
{
	if(singleton == NULL)
	{
		cerr<<"configuration not yet initilized!"<<endl;
		assert(false);
		exit(-1);
	}
	return singleton;
}

void Config::initConfig(string& conf_filename)
{
	assert(singleton == NULL);
	Config::singleton = new Config();
	//loading the psmg configuration file
	pugi::xml_document conf;
	pugi::xml_parse_result result = conf.load_file(conf_filename.c_str());
	if(result.status != pugi::status_ok)
	{
		cerr<<"can't find configuration file: "<<conf_filename<<", or file is corrupted!"<<endl;
		exit(0);
	}

	MPI::Intercomm comm;
	comm = MPI::COMM_WORLD;
	char hname[128];
	hname[127] = '\0';
	gethostname(hname,127);

	singleton->hostname = hname;
	singleton->rank = comm.Get_rank();
	singleton->size = comm.Get_size();

	singleton->configFilename = conf_filename;

	const pugi::xml_node& psmgn = conf.child("PSMG");
	singleton->modelfilename = psmgn.attribute("modelfile").as_string();
	singleton->datafilename = psmgn.attribute("datafile").as_string();
	singleton->solve = psmgn.attribute("solve").as_bool();
	singleton->solvetype = psmgn.attribute("solvetype").as_string();
	const pugi::xml_node& stoch = conf.child("STOCH");
	singleton->root_stage_parent_name = stoch.attribute("root_stage_parent_name").as_string();
	singleton->node_dummy_prefix = stoch.attribute("node_dummy_prefix").as_string();
	singleton->stage_dummy_prefix = stoch.attribute("stage_dummy_prefix").as_string();
	singleton->var_det_prefix = stoch.attribute("var_det_prefix").as_string();

	const pugi::xml_node& debugn = conf.child("DEBUG");
	singleton->writeMatlab = debugn.attribute("writeMatlab").as_bool();
	singleton->matlabFile = debugn.attribute("matlabFile").as_string();
	singleton->writeMPS = debugn.attribute("writeMPS").as_bool();
	singleton->mpsFile = debugn.attribute("mpsFile").as_string();
	singleton->debug = debugn.attribute("debug").as_bool();
	singleton->logdir = debugn.attribute("logdir").as_string();

	singleton->logEM = debugn.attribute("logEM").as_bool();
	singleton->logBlock = debugn.attribute("logBlock").as_bool();
	singleton->emfile_suffix = debugn.attribute("emfile_suffix").as_string();
	singleton->logModel = debugn.attribute("logModel").as_bool();
	singleton->modfile_suffix = debugn.attribute("modfile_suffix").as_string();
}

void Config::printAll()
{
	Config* conf = Config::instance();
	cout<<"Opt: "<<"config filename ["<<conf->configFilename<<"]"<<endl;
	cout<<"Opt: "<<"hostname ["<<conf->hostname<<"]"<<endl;
	cout<<"Opt: "<<"Rank ["<<GV(rank)<<"]"<<endl;
	cout<<"Opt: "<<"Size ["<<GV(size)<<"]"<<endl;
	cout<<"Opt: "<<"model file [" << conf->modelfilename << "]"<<endl;
	cout<<"Opt: "<<"data file [" << conf->datafilename << "]"<<endl;
	cout<<"Opt: "<<"solve ["<<conf->solve<<"]"<<endl;
	cout<<"Opt: "<<"solvetype ["<<conf->solvetype<<"]"<<endl;
	cout<<"Opt: "<<"root_stage_parent_name ["<<conf->root_stage_parent_name<<"]"<<endl;
	cout<<"Opt: "<<"node_dummy_prefix ["<<conf->node_dummy_prefix<<"]"<<endl;
	cout<<"Opt: "<<"stage_dummy_prefix ["<<conf->stage_dummy_prefix<<"]"<<endl;
	cout<<"Opt: "<<"var_det_prefix ["<<conf->var_det_prefix<<"]"<<endl;

	cout<<"Opt: "<<"Debug["<<conf->debug<<"]"<<endl;
	cout<<"Opt: "<<"write matlab ["<<conf->writeMatlab<<"]"<<endl;
	cout<<"Opt: "<<"write MPS ["<<conf->writeMPS<<"]"<<endl;
	cout<<"Opt: "<<"logEM["<<GV(logEM)<<"]"<<endl;
	cout<<"Opt: "<<"logBlock["<<GV(logBlock)<<"]"<<endl;
	cout<<"Opt: "<<"logModel ["<<conf->logModel<<"]"<<endl;
	cout<<"Opt: "<<"logdir ["<<conf->logdir<<"]"<<endl;
}

