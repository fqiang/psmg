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

//#include "../context/ExpandedModel2.h"

class ExpandedModel;
/** @class OOPSBlock
 *  OOPSBlock is an object that directly corresponds to a node in the
 *  OOPS Algebra Tree. 
 *  
 * It is characterized by 
 *   - An ExpandedModelInterface/NlFile that gives information on the rows 
 *     in this block
 *   - A list of variable definitions (given as the start of variable names)
 *     that should be used from the NlFile.
 *  This class will do the necessary interfacing (i.e. extract a list
 *  of variable indices from the NlFile that should be used and indicate
 *  In which positions these should appear in the block
 */
class PDProblem;

class OOPSBlock {
 public:
  ExpandedModel *emrow;    //!< Expanded Model giving row information
  ExpandedModel *emcol;    //!< Expanded Model giving col information
  PDProblem* prob;
  int i;
  // --------------------------- methods -----------------------------------
  /** constrct an OOPS block from the cross section of two ExpandedModelInterface's
   *  @param rowmod       The model giving the row information
   *  @param colmod       The model giving the column information
   */
  OOPSBlock(ExpandedModel *rowmod, ExpandedModel *colmod);
};
