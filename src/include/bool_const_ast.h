#pragma once

#include "ast.h"

class BoolConstAST : public AST {
private:
	bool value;
public:
	BoolConstAST(bool value);

	bool getValue() const;
	
	virtual void accept(Visitor &visitor);
};