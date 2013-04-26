/*
 * TimeStamp.cpp
 *
 *  Created on: 10 Apr 2011
 *      Author: s0965328
 */

#include "TimeStamp.h"
#include "../util/global_util_functions.h"
#include <assert.h>
#include <sstream>

map<string,TimeStamp*> TimeStamp::timeStamps;
bool TimeStamp::first = true;

TimeStamp* TimeStamp::GetTimeStamp(string name)
{
	map<string,TimeStamp*>::iterator it;
	it = timeStamps.find(name);
	TimeStamp* ts = it==timeStamps.end()?NULL:it->second;
	if(ts==NULL)
	{	
		ts = new TimeStamp(name);
		TimeStamp::timeStamps.insert(pair<string,TimeStamp*>(name,ts));
	}
	return ts;
}
TimeStamp::TimeStamp(const TimeStamp& ts)
{
	assert(false); //should never happen
	DEBUG_C("Disable copying!! \n");
}

TimeStamp::TimeStamp(string name) {
	DEBUG_C("create timestamp [%s] \n",name.c_str());
	this->startTime = 0;
	this->stopTime = 0;
	this->name = name;
}

void TimeStamp::start() {
//	time(&this->startTime );
	this->startTime = clock();
	cout<<"["<<GlobalVariables::rank<<"/"<<GlobalVariables::size<<"] TimeStamp:["<<name<<"] START@:["<<startTime<<"]"<<endl;
}

void TimeStamp::stop() {
//	time(&this->stopTime);
	this->stopTime = clock();
//	time_t elapse = stopTime - startTime;
	clock_t elapse = stopTime - startTime;
	cout<<"["<<GlobalVariables::rank<<"/"<<GlobalVariables::size<<"] TimeStamp:["<<name<<"] STOP@:["<<stopTime<<"] Elapse[ "<<elapse<<" ]"<<endl;
}

string TimeStamp::toString() const{
	ostringstream buf;
	time_t elapse = this->stopTime - this->startTime;
	buf<<"[name:"<<this->name<<"] [startTime:"<<this->startTime<<"] [stopTime:"<<this->stopTime<<"] [elapse:"<<elapse<<"]";
	return buf.str();
}


void TimeStamp::List()
{
	cout<<"["<<GlobalVariables::rank<<"/"<<GlobalVariables::size<<"] == TimeStamp List["<<timeStamps.size()<<"] =="<<endl;
	map<string,TimeStamp*>::iterator it;
	it = timeStamps.begin();
	while(timeStamps.size()!=0&&it!=timeStamps.end())
	{
		cout<<"["<<GlobalVariables::rank<<"/"<<GlobalVariables::size<<"] TimeStamp:["<<(*it).first<<"]"<<(*it).second->toString()<<endl;
		it++;
	}
	cout<<"["<<GlobalVariables::rank<<"/"<<GlobalVariables::size<<"] == TimeStamp List =="<<endl;
}

void TimeStamp::Reset() {
	cout<<"["<<GlobalVariables::rank<<"/"<<GlobalVariables::size<<"] TimeStamp RESET@:["<<time(NULL)<<"]"<<endl;
	map<string,TimeStamp*>::iterator it;
	it = timeStamps.begin();
	while(it!=timeStamps.end())
	{
		cout<<"["<<GlobalVariables::rank<<"/"<<GlobalVariables::size<<"] Clear TimeStamp:["<<(*it).first<<"]"<<endl;
		delete (*it).second;
		it++;
	}
	timeStamps.clear();
}


TimeStamp::~TimeStamp() {
	DEBUG_C("Destructor called on TimeStamp");
}

void TimeStamp::SelfTest()
{
	TimeStamp* ts = TimeStamp::GetTimeStamp("All");
	ts->start();
	sleep(2);
	TimeStamp::List();
	ts = TimeStamp::GetTimeStamp("Test2");
	ts->start();

	ts = TimeStamp::GetTimeStamp("All");
	ts->stop();
	TimeStamp::List();
	ts = TimeStamp::GetTimeStamp("Test2");
	sleep(3);
	ts->stop();
	TimeStamp::List();
	TimeStamp::Reset();
	ts = TimeStamp::GetTimeStamp("All");
	TimeStamp::List();
}
