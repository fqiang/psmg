/*
 * SyntaxNodeIDREFM.h
 *
 *  Created on: 6 Jan 2014
 *      Author: s0965328
 */

#ifndef SYNTAXNODEIDREFM_H_
#define SYNTAXNODEIDREFM_H_

#include "SyntaxNode.h"

class SyntaxNodeIDREFM : public SyntaxNode {
public:
	AmplModel* ref;

	SyntaxNodeIDREFM(int opCode, SyntaxNode* id, AmplModel* model);
	virtual ~SyntaxNodeIDREFM();

	std::ostream& put(std::ostream& s);
	void calculateMemoryUsage(unsigned long& size);
};

#endif /* SYNTAXNODEIDREFM_H_ */
