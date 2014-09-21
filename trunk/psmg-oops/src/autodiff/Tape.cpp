/*
 * Tape.cpp
 *
 *  Created on: 6 Nov 2013
 *      Author: s0965328
 */



#include "Tape.h"
#include "autodiff_assert.h"

namespace AutoDiff
{
	template<> Tape<uint>* Tape<uint>::indexTape = NULL;
	template<> Tape<double>* Tape<double>::valueTape = NULL;
}
