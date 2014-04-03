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
#define DEBUG_PRINT_ENABLED 0 //enable debug print
#define DEBUG_YAC_DATA_ENABLED 1
#define DEBUG_YAC_MODEL_ENABLE 1
#define SYS_MEMORY_USAGE_ENABLED 0
#define MEMORY_LOG_ENABLED 0
#else
#define DEBUG_PRINT_ENABLED 0  //enable debug print
#define DEBUG_YAC_ENABLED 0
#endif


#ifdef BMARK
#define PERFORMANCE_MEASURE_ENABLED 1 //enable performance measure
#endif

#ifdef MEM
#define SYS_MEMORY_USAGE_ENABLED 0
#define MEMORY_LOG_ENABLED 0
#define PERFORMANCE_MEASURE_ENABLED 1
#else
#define SYS_MEMORY_USAGE_ENABLED 0
#define MEMORY_LOG_ENABLED 0
#endif

#define DEBUG_PAR_ENABLED 0
#define EPSILON 0.0000001

#if DEBUG_PRINT_ENABLED
#define DEBUG_C printf
#else
#define DEBUG_C(format, args...) ((void)0)
#endif


#if DEBUG_PRINT_ENABLED
#define LOG(arg) (std::cout<<"LOG==>["<<GlobalVariables::rank<<"/"<<GlobalVariables::size<<"]-"<<arg<<std::endl)
#else
#define LOG(arg) ((void)0)
#endif

#if DEBUG_PRINT_ENABLED
  #define LOG_SM(arg) (std::cout<<"LOG_SM==>["<<GlobalVariables::rank<<"/"<<GlobalVariables::size<<"]-"<<arg<<std::endl)
#else
  #define LOG_SM(arg) ((void)0)
#endif

#if DEBUG_PAR_ENABLED
#define LOG_PAR(arg) (std::cout<<"PAR_SCRIPT==>"<<arg<<std::endl)
#else
#define LOG_PAR(arg) ((void)0)
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
#else
#define TIMER_START(arg) ((void)0)
#endif

#if PERFORMANCE_MEASURE_ENABLED
#define TIMER_STOP(arg) (TimeStamp::GetTimeStamp(arg)->stop())
#else
#define TIMER_STOP(arg) ((void)0)
#endif

#if PERFORMANCE_MEASURE_ENABLED
#define TIMER_LIST (TimeStamp::List())
#else
#define TIMER_LIST ((void)0)
#endif

#if PERFORMANCE_MEASURE_ENABLED
#define TIMER_RESET (TimeStamp::Reset())
#else
#define TIMER_RESET ((void)0)
#endif

//#if SYS_MEMORY_USAGE_ENABLED
//#define LOG_SYS_MEM(arg) print_mem_usage(arg)
//#else
//#define LOG_SYS_MEM(arg) ((void)0)
//#endif

#if MEMORY_LOG_ENABLED
#define LOG_MEM(arg) (std::cout<<"LOG_MEM==>["<<GlobalVariables::rank<<"/"<<GlobalVariables::size<<"]-"<<arg<<std::endl)
#else
#define LOG_MEM(arg) ((void)0)
#endif

#endif
