#ifndef SET_H
#define SET_H

#include "CompDescr.h"
#include "../util/global_util_functions.h"
#include <ext/hash_map>
#include <string>

using namespace std;
using namespace __gnu_cxx;

class Set: public CompDescr{
	private:

	public:
		int dim; //same as in ModelComp
		string name;
		int card;
		vector<string> setValues_data_order;
		hash_map<string,int> setOrders;
		hash_map<int,string> setKeys;

		Set(int dim_,string name_);
		virtual ~Set();
		Set(const Set& dest);

		int setOrder(string key);
		void addSetValue(string& key);
		void addSetValue(ostringstream& oss);
		void fillSetValues(char*& data);
		Set* setDiff(Set* sub);
		Set* setCross(Set* other);
		int getCard();
		string toString() const;
		bool contains(string&);
		void calculateMemoryUsage(unsigned long& size);
	private:
};

#endif
