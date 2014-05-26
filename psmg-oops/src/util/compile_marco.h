#ifndef COMPILERMARCO_H_
#define COMPILERMARCO_H_

#include <iostream>

/*
 * compile_marco.h
 *
 *  Created on: 18 Feb 2013
 *      Author: s0965328
 */

typedef unsigned int uint;
typedef unsigned long ulong;

#if DEBUG
#define TRACE_PRINT_ENABLED 1
#define TRACE_PARSER_ENABLED 1
#define MEM_ENABLED 1
#define TIMER_ENABLED 1
#endif

#if RELEASE
#define TRACE_PRINT_ENABLED 0
#define TRACE_PARSER_ENABLED 0
#define MEM_ENABLED 0
#define TIMER_ENABLED 1
#endif

#if TRACE_PRINT_ENABLED
#define TRACE(arg) (std::cout<<"TRACE==>["<<GV(rank)<<"/"<<GV(size)<<"]-"<<arg<<std::endl)
#else
#define TRACE(arg) ((void)0)
#endif

#define ERROR(arg) (std::cout<<"ERROR **** ["<<GV(rank)<<"/"<<GV(size)<<"]-"<<arg<<std::endl)
#define WARN(arg) (std::cout<<"ERROR **** ["<<GV(rank)<<"/"<<GV(size)<<"]-"<<arg<<std::endl)

#if 0
#define YAC(arg) (std::cout<<"YAC==>"<<arg<<std::endl)
#else
#define YAC(arg) ((void)0)
#endif

#if TIMER_ENABLED
#define TIMER_START(arg) (TimeStamp::GetTimeStamp(arg)->start())
#define TIMER_STOP(arg) (TimeStamp::GetTimeStamp(arg)->stop())
#define TIMER_LIST (TimeStamp::List())
#define TIMER_RESET (TimeStamp::Reset())
#define LOG_TIME(arg) (std::cout<<"TIME==>["<<GV(rank)<<"/"<<GV(size)<<"]-"<<arg<<std::endl)
#else
#define TIMER_START(arg) ((void)0)
#define TIMER_STOP(arg) ((void)0)
#define TIMER_LIST ((void)0)
#define TIMER_RESET ((void)0)
#define LOG_TIME(arg) ((void)0)
#endif


#if MEM_ENABLED
#define MEM(arg) (std::cout<<"MEM==>["<<GV(rank)<<"/"<<GV(size)<<"]-"<<arg<<std::endl)
#else
#define MEM(arg) ((void)0)
#endif

#endif
