#pragma once

#include "ast.h"

class IntConstAST : public AST {
private:
	int value;
public:
	IntConstAST(int value);

	int getValue() const;
	
	virtual void accept(Visitor &visitor);
};