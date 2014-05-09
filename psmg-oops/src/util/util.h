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
#include <sstream>
#include <inttypes.h>
#include <unistd.h>
#include <fstream>
#include "../metric/Stat.h"
#include "../metric/TimeStamp.h"
#include "../sml/Config.h"
#include "compile_marco.h"

void print_mem_usage(std::string name);


#endif /* UTIL_H_ */
