/*
 * util.h
 *
 *  Created on: 20 Apr 2011
 *      Author: s0965328
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <stdio.h>
#include <cassert>
#include <string.h>
#include <sys/stat.h>
#include <ext/hash_map>
#include <sstream>
#include <inttypes.h>
#include <unistd.h>
#include <fstream>
#include "CompilerMarco.h"
#include "../metric/TimeStamp.h"
#include "GlobalVariables.h"

class ModelComp;
class AmplModel;
void print_mem_usage(string name);

int inline findAndReplace(string& source, const char* find, const char* replace)
{
	LOG_PAR("findAndReplace - find["<<find<<"] replace["<<replace<<"]");
    int num=0;
    size_t fLen = strlen(find);
    size_t rLen = strlen(replace);

    for (size_t pos=0; (pos=source.find(find, pos))!=string::npos; pos+=rLen)
    {
        num++;
        source.replace(pos, fLen, replace);
    }
    return num;
}

int inline createTmpDirIfNotPresent()
{
	int err = 0;
	bool fl_exists;
	struct stat sbuf;
	const char *tmpdirname = "tmp";

	fl_exists = !(stat(tmpdirname, &sbuf) == -1);

	if(!fl_exists)
	{// tmp doesn't exist
#ifdef HAVE_DIRECT_H
		err = mkdir(tmpdirname);
#else
		err = mkdir(tmpdirname, S_IRWXU);
#endif
		if(err)
			cerr << "ERROR: Failed to create temporary directory '"<< tmpdirname << "'.\n";
	}
	else
	{	// tmp is present
		// check that it's a directory with RWX permissions
		bool isusable = S_ISDIR(sbuf.st_mode) && ((sbuf.st_mode & S_IRWXU)== S_IRWXU);
		if (!isusable)
		{
			err = 1;
			cerr << "ERROR: Cannot use '" << tmpdirname<< "' as temporary directory.\n";
		}
	}
	return err;
}

namespace __gnu_cxx
{
	template<> struct hash< std::string >
	{
		size_t operator()( const std::string& x) const
		{
			return hash< const char* >()(x.c_str());
		}
	};

	template<> struct hash< ::ModelComp* >
	{
		size_t operator()( const ModelComp* x) const
		{
			size_t h = (intptr_t)(x);
			LOG("hashkey - "<<h);
			return h;
		}
	};
}

#endif /* UTIL_H_ */
