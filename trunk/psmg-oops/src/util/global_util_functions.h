/*
 * util.h
 *
 *  Created on: 20 Apr 2011
 *      Author: s0965328
 */

#ifndef GLOBAL_UTIL_FUNCTION_H_
#define GLOBAL_UTIL_FUNCTION_H_

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
#include "../sml/GlobalVariables.h"
#include "../metric/TimeStamp.h"

class ModelComp;
class AmplModel;
class ExpandedModel;
class Var;
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

	template<> struct hash< ::ExpandedModel* >
	{
		size_t operator()( const ExpandedModel* x) const
		{
			size_t h = (intptr_t)(x);
			LOG("hashkey - "<<h);
			return h;
		}
	};

	template<> struct hash< ::Var* >
	{
		size_t operator()( const Var* x) const
		{
			size_t h = (intptr_t)(x);
			LOG("hashkey - "<<h);
			return h;
		}
	};
}

#endif /* UTIL_H_ */
