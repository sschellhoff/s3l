#pragma once

#include "ast.h"

class RealConstAST : public AST {
private:
	double value;
public:
	RealConstAST(double value);

	double getValue() const;
	
	virtual void accept(Visitor &visitor);
};