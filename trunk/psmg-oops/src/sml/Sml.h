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
#ifndef SML_H
#define SML_H

#include <string>
#include <iostream>
#include <vector>
#include "../context/ExpandedModel.h"

using namespace std;

class Sml
{
public:
	static Sml* instance();
	static void deleteInstance();

	void writeHelp(ostream& out);
	int analyseOptions(int argc, char **argv);

	void processModelfile();
	void generateExpandedModel();
	void resetContextTree();
	void formulateConstraints();
	void printEMStructure(string filename);
	void testRecursiveCreateEMB0(ExpandedModel* root,ExpandedModel* other);
	void testRecursiveCreateEMB1(ExpandedModel* root);

private:
	Sml();
	~Sml();
	static Sml* _sml;

};
#endif
