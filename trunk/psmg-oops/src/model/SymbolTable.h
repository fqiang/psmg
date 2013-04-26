/* (c) 2008,2009 Jonathan Hogg and Andreas Grothey, University of Edinburgh
 *
 * This file is part of SML.
 *
 * SML is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, using version 3 of the License.
 *
 * SML is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see http://www.gnu.org/licenses/.
 */

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <list>
#include <string>

class ModelComp;

class SymbolTable
{

public:
	enum SymType {ST_NONE=0, ST_PARAM, ST_VAR, ST_CONS, ST_OBJ, ST_SET};

	class Entry
	{
		private:
			const std::string name;
			const SymType type;

		public: // should be private
			ModelComp *mc;

			Entry(const std::string new_id, const SymType new_type,
			ModelComp *new_mc) :
			name(new_id), type(new_type), mc(new_mc) {}

			/** Retrieve the identifier for this entry */
			const std::string& id() const { return name; }

			/** Whether this entry is of type @a t */
			bool isType(SymType t) const { return type == t; }
	};

	SymbolTable();
	void copy(const SymbolTable& src)
	{
		for (int i = 0; i < n_hash; ++i)
		{
			for (std::list<Entry>::const_iterator j = src.table_[i].begin();j != src.table_[i].end(); ++j)
			{
				table_[i].push_back(*j);
			}
		}
	}
	virtual ~SymbolTable(){
	}
	bool defineSymbol(SymType, char *id, ModelComp *mc);
	const Entry* findSymbol(const std::string& id) const;
	std::list<Entry> getListByType(const SymType type) const;
	void calculateMemoryUsage(unsigned long& actual);
private:
	static const int n_hash = 100;
	/** Number of available hash codes */
	std::list<Entry> table_[n_hash];
	unsigned long hash_function(const char *str) const;

public:

private:
};

#endif
