/*
 * autodiff_marco.h
 *
 *  Created on: 25 May 2014
 *      Author: s0965328
 */

//#define NDEBUG  // uncomment this line for disable assertion
#include <cassert>

#define FORWARD_ENABLED 0

#if DEBUG
#define AD_TRACE(arg) (std::cout<<"AD_TRACE==>"<<arg<<std::endl)
#endif

#if RELEASE
#define AD_TRACE(arg) ((void)0)
#endif
