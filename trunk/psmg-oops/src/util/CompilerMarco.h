#ifndef COMPILERMARCO_H_
#define COMPILERMARCO_H_

#include <iostream>

/*
 * CompilerMarco.h
 *
 *  Created on: 18 Feb 2013
 *      Author: s0965328
 */

typedef unsigned int uint;
typedef unsigned long ulong;

#ifdef DEBUG
#define DEBUG_PRINT_ENABLED 1 //enable debug print
#define DEBUG_YAC_DATA_ENABLED 1
#define DEBUG_YAC_MODEL_ENABLE 1
#define SYS_MEMORY_USAGE_ENABLED 0
#define MEMORY_LOG_ENABLED 0
#define PERFORMANCE_MEASURE_ENABLED 0
#else
#define DEBUG_PRINT_ENABLED 0
#define DEBUG_YAC_ENABLED 0
#define DEBUG_YAC_MODEL_ENABLE 0
#define SYS_MEMORY_USAGE_ENABLED 0
#define MEMORY_LOG_ENABLED 0
#define PERFORMANCE_MEASURE_ENABLED 1
#endif

#ifdef MEM
#define SYS_MEMORY_USAGE_ENABLED 1
#define MEMORY_LOG_ENABLED 1
#endif

#if DEBUG_PRINT_ENABLED
#define LOG(arg) (std::cout<<"LOG==>["<<GV(rank)<<"/"<<GV(size)<<"]-"<<arg<<std::endl)
#else
#define LOG(arg) ((void)0)
#endif

#if DEBUG_PRINT_ENABLED
  #define LOG_SM(arg) (std::cout<<"LOG_SM==>["<<GV(rank)<<"/"<<GV(size)<<"]-"<<arg<<std::endl)
#else
  #define LOG_SM(arg) ((void)0)
#endif

#if DEBUG_YAC_DATA_ENABLED
#define YAC_DATA_LOG(arg) (std::cout<<"YAC_DATA_LOG==>"<<arg<<std::endl)
#else
#define YAC_DATA_LOG(arg) ((void)0)
#endif

#if DEBUG_YAC_MODEL_ENABLE
#define YAC_MODEL_LOG(arg) (std::cout<<"YAC_MODEL_LOG==>"<<arg<<std::endl)
#else
#define YAC_MODEL_LOG(arg) ((void)0)
#endif


#if PERFORMANCE_MEASURE_ENABLED
#define TIMER_START(arg) (TimeStamp::GetTimeStamp(arg)->start())
#define TIMER_STOP(arg) (TimeStamp::GetTimeStamp(arg)->stop())
#define TIMER_LIST (TimeStamp::List())
#define TIMER_RESET (TimeStamp::Reset())
#define LOG_TIME(arg) (std::cout<<"LOG==>["<<GV(rank)<<"/"<<GV(size)<<"]-"<<arg<<std::endl)
#else
#define TIMER_START(arg) ((void)0)
#define TIMER_STOP(arg) ((void)0)
#define TIMER_LIST ((void)0)
#define TIMER_RESET ((void)0)
#define LOG_TIME(arg) ((void)0)
#endif

//#if SYS_MEMORY_USAGE_ENABLED
//#define LOG_SYS_MEM(arg) print_mem_usage(arg)
//#else
//#define LOG_SYS_MEM(arg) ((void)0)
//#endif

#if MEMORY_LOG_ENABLED
#define LOG_MEM(arg) (std::cout<<"LOG_MEM==>["<<GV(rank)<<"/"<<GV(size)<<"]-"<<arg<<std::endl)
#else
#define LOG_MEM(arg) ((void)0)
#endif

#endif
