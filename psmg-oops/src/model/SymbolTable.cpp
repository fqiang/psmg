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

#include "SymbolTable.h"
#include "../util/global_util_functions.h"
#include <iostream>

using namespace std;

SymbolTable::SymbolTable()
{
	LOG("init symbolTable -- ");
}

/* Note: original symbol table also checked that this variable was not defined
 * in an enclosing scope, rather than just hiding the previously defined
 * variables as we do now. */
bool SymbolTable::defineSymbol(Entry::SymType type, char *id, ModelComp *mc)
{
	/* Calculate hashcode */
	int hash = hash_function(id) % n_hash;

	/* First check id not already defined, return false if it is */
	for(list<Entry>::iterator i=table_[hash].begin(); i!=table_[hash].end(); i++)
	{
		if ((*i).id() == id)
		{
			return false;
		}
	}
	/* Otherwise insert at the start of the list */
	table_[hash].push_back(Entry(id,type,mc));
	LOG("SYMTAB: defined " << id << " of type " << type);

	/* Sucessfully defined NEW symbol */
	return true;
}

const Entry* SymbolTable::findSymbol(const string& id) const
{
   /* Calculate hashcode */
   int hash = hash_function(id.c_str()) % n_hash;

   list<Entry>::const_iterator i;
   for(i=table_[hash].begin(); i!=table_[hash].end(); ++i) {
		if ((*i).id() == id){
			break;
		}
   }

   if(i==table_[hash].end()) return NULL;

   return &(*i);
}

void SymbolTable::calculateMemoryUsage(unsigned long& size)
{
	size += sizeof(SymbolTable);
	for(int i=0;i<SymbolTable::n_hash;i++)
	{
		size += sizeof(list<Entry>);
		for(list<Entry>::iterator it=table_[i].begin();it!=table_[i].end();it++)
		{
			size += sizeof(Entry);
			size += (*it).id().size() + 1;
		}
	}

}

/* ----------------------------------------------------------------------------
hash function:
---------------------------------------------------------------------------- */
/* this is djb2 (k=33) of dan bernstein taken from 
   http://www.cse.yorku.ca/~oz/hash.html
*/
unsigned long SymbolTable::hash_function(const char *str) const
{

	unsigned long hash = 5381;
	int c;

	while ( (c=*str++) )
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}
