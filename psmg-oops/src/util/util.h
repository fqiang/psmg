/*
 * util.h
 *
 *  Created on: 20 Apr 2011
 *      Author: s0965328
 */

#ifndef UTIL_H_
#define UTIL_H_

#define NDEBUG     //enable or disable assert
#include <cassert>


#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sstream>
#include <inttypes.h>
#include <unistd.h>
#include <fstream>
#include "compile_marco.h"
#include "../metric/TimeStamp.h"
#include "../metric/Stat.h"
#include "../sml/Config.h"

class SyntaxNode;

void print_mem_usage(std::string name);

void print(SyntaxNode* node);

#endif /* UTIL_H_ */
