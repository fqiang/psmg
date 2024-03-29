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
class ExpandedModel;

using namespace std;

class Sml
{
public:
	static Sml* instance();
	static void deleteInstance();

	void processModelfile();
	void generateExpandedModel();
	void resetContextTree();
	void logEM(string filename);


	void testInterfaceLocal0(ExpandedModel* root,ExpandedModel* other);
	void testInterfaceLocal1(ExpandedModel* emrow,ExpandedModel* emcol);
	void testInterfaceLocal2(ExpandedModel* emrow);
	void testInterfaceLocal(ExpandedModel* root);

private:
	Sml();
	~Sml();
	static Sml* _sml;

};
#endif
