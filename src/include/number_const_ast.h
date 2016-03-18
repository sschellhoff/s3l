#pragma once

#include "ast.h"

class NumberConstAST : public AST {
private:
	double value;
public:
	NumberConstAST(double value);

	double getValue() const;
	
	virtual void accept(Visitor &visitor);
};