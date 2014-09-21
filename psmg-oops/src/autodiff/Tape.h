/*
 * Tape.h
 *
 *  Created on: 15 Apr 2013
 *      Author: s0965328
 */

#ifndef TAPE_H_
#define TAPE_H_

#include <vector>
#include <string>
#include <sstream>
#include <iostream>

#include "auto_diff_types.h"

namespace AutoDiff {

using namespace std;
#define TT 	(Tape<double>::valueTape)
#define II 	(Tape<uint>::indexTape)

template<typename T> class Tape {
public:
	Tape<T> () : index(0){};
	T& at(const uint index);
	const T& get(const uint index);
	void set(T& v);
	uint size();
	void clear();
	bool empty();
	string toString();
	virtual ~Tape();

	vector<T> vals;
	uint index;

	static Tape<double>* valueTape;
	static Tape<uint>* indexTape;
};


template<typename T> Tape<T>::~Tape<T>()
{
	index = 0;
	vals.clear();
}

template<typename T> T& Tape<T>::at(const uint i)
{
	assert(this->vals.size()>i);
	return vals[i];
}
template<typename T> const T& Tape<T>::get(const uint i)
{
	assert(this->vals.size()>i);
	return vals[i];
}
template <typename T> void Tape<T>::set(T& v)
{
	vals.push_back(v);
	index++;
}

template<typename T> uint Tape<T>::size()
{
	return this->vals.size();
}

template<typename T> bool Tape<T>::empty()
{
	return vals.empty();
}

template<typename T> void Tape<T>::clear()
{
	this->vals.clear();
	this->index = 0;
	assert(this->vals.size()==0);
	assert(this->vals.empty());
}

template<typename T>  string Tape<T>::toString()
{
	assert(vals.size()>=index);
	ostringstream oss;
	oss<<"Tape size["<<vals.size()<<"]";
	for(uint i=0;i<vals.size();i++){
		if(i%10==0) oss<<endl;
		oss<<vals[i]<<",";
	}
	oss<<endl;
	return oss.str();
}
}
#endif /* TAPE_H_ */
