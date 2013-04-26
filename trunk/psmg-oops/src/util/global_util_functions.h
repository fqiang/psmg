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
