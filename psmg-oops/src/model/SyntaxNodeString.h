/*
 * SyntaxNodeString.h
 *
 *  Created on: 18 Apr 2014
 *      Author: s0965328
 */

#ifndef SYNTAXNODESTRING_H_
#define SYNTAXNODESTRING_H_

#include "SyntaxNode.h"

class SyntaxNodeString: public SyntaxNode {
public:
	SyntaxNodeString(string& val);
	SyntaxNodeString(const SyntaxNodeString& src);
	virtual SyntaxNodeString* clone();
	virtual ~SyntaxNodeString();

	string val;

	std::string value() const;

	virtual std::ostream& put(std::ostream&s) { return s << this->val; }
	virtual void calculateMemoryUsage(unsigned long& size);
};

#endif /* SYNTAXNODESTRING_H_ */
