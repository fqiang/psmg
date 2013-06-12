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

using namespace std;

class Sml
{
public:
	static Sml* instance();
	static void deleteInstance();
	static std::string smlVersion();
	static void print_copy_right(ostream &out);

	void process_model_file();
	void generate_em2();
	void analyse_constraints();


	vector<string> scriptNameList;

private:
	Sml();
	~Sml();
	static Sml* _sml;
	static int sml_gen_pre_checks();

};
#endif
