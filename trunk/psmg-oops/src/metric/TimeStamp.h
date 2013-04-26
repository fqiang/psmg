/*
 * TimeStamp.h
 *
 *  Created on: 15 Apr 2011
 *      Author: s0965328
 */

#ifndef TIMESTAMP_H_
#define TIMESTAMP_H_

#include <time.h>
#include <string>
#include <iostream>
#include <map>

using namespace std;

class TimeStamp {
public:
	static TimeStamp* GetTimeStamp(string name);
	static void Reset();
	static void List();
	static void SelfTest();
	string toString() const;
	~TimeStamp();
	void start();
	void stop();

private:
	TimeStamp(const TimeStamp&);
	TimeStamp(string name);
	clock_t startTime;
	clock_t stopTime;
	string name;

	static bool first;
	static map<string,TimeStamp*> timeStamps;
};


#endif /* TIMESTAMP_H_ */

