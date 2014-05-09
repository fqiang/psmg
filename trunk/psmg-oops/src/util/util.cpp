/*
 * util.cpp
 *
 *  Created on: 25 Apr 2012
 *      Author: s0965328
 */

#include <unistd.h>
#include <ios>
#include <iostream>
#include <fstream>
#include <string>
#include "util.h"

using namespace std;
using namespace __gnu_cxx;


void print_mem_usage(string name)
{
   using std::ios_base;
   using std::ifstream;
   using std::string;

   double vm_usage     = 0.0;
   double resident_set = 0.0;

   // 'file' stat seems to give the most reliable results
   //
   ifstream stat_stream("/proc/self/stat",ios_base::in);

   // dummy vars for leading entries in stat that we don't care about
   //
   string pid, comm, state, ppid, pgrp, session, tty_nr;
   string tpgid, flags, minflt, cminflt, majflt, cmajflt;
   string utime, stime, cutime, cstime, priority, nice;
   string O, itrealvalue, starttime;

   // the two fields we want
   //
   unsigned long vsize;
   long rss;

   stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
               >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
               >> utime >> stime >> cutime >> cstime >> priority >> nice
               >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest

   stat_stream.close();

   long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
   vm_usage     = vsize / 1024.0;
   resident_set = rss * page_size_kb;
   cout<<"["<<GV(rank)<<"/"<<GV(size)<<"] LOG_SYS_MEM["<<name<<"]"<<" Pid["<<pid<<"]"<<" VM[ "<<vm_usage<<" ] RSS[ "<<resident_set<<" ]"<<endl;

}
