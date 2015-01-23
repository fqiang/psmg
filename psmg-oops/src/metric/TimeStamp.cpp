/*
 * TimeStamp.cpp
 *
 *  Created on: 10 Apr 2011
 *      Author: s0965328
 */

#include "TimeStamp.h"
#include "../util/util.h"
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
	TRACE("Disable copying!!");
}

TimeStamp::TimeStamp(string name) {
	TRACE("create timestamp ["<<name<<"]");
	this->startTime = 0;
	this->stopTime = 0;
	this->elapse = 0;
	this->name = name;
}

void TimeStamp::start() {
	this->startTime = clock();
	cout<<"["<<GV(rank)<<"/"<<GV(size)<<"] TimeStamp:["<<name<<"] START@:["<<startTime<<"]"<<endl;
}

void TimeStamp::stop() {
	this->stopTime = clock();
	clock_t curr_elapse = stopTime - startTime;
	this->elapse += curr_elapse;
	cout<<'['<<GV(rank)<<"/"<<GV(size)<<"] TimeStamp:["<<name<<"] STOP@:["<<stopTime<<"] Elapse[ "<<this->elapse/CLOCKS_PER_SEC<<" ] CElapse[ "<<curr_elapse/CLOCKS_PER_SEC<<" ]"<<endl;
}

string TimeStamp::toString() const{
	ostringstream buf;
	buf<<"[name:"<<this->name<<"] [startTime:"<<this->startTime<<"] [stopTime:"<<this->stopTime<<"] [elapse:"<<this->elapse/CLOCKS_PER_SEC<<"]";
	return buf.str();
}


void TimeStamp::List()
{
	cout<<"["<<GV(rank)<<"/"<<GV(size)<<"] == TimeStamp List["<<timeStamps.size()<<"] =="<<endl;
	map<string,TimeStamp*>::iterator it;
	it = timeStamps.begin();
	while(timeStamps.size()!=0&&it!=timeStamps.end())
	{
		cout<<"["<<GV(rank)<<"/"<<GV(size)<<"] TimeStamp:"<<(*it).second->toString()<<endl;
		it++;
	}
	cout<<"["<<GV(rank)<<"/"<<GV(size)<<"] == TimeStamp List =="<<endl;
}

void TimeStamp::Reset() {
	cout<<"["<<GV(rank)<<"/"<<GV(size)<<"] TimeStamp RESET@:["<<time(NULL)<<"]"<<endl;
	map<string,TimeStamp*>::iterator it;
	it = timeStamps.begin();
	while(it!=timeStamps.end())
	{
		cout<<"["<<GV(rank)<<"/"<<GV(size)<<"] Clear TimeStamp:["<<(*it).first<<"]"<<endl;
		delete (*it).second;
		it++;
	}
	timeStamps.clear();
}


TimeStamp::~TimeStamp() {
	TRACE("Destructor called on TimeStamp");
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
