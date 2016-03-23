#pragma once

#include "ast.h"
#include "operators.h"

#include <memory>

using ASTPTR = std::unique_ptr<AST>;

class UnaryOperatorAST : public AST {
private:
	UnaryOperator op;
	std::unique_ptr<AST> inner;
public:
	UnaryOperatorAST(UnaryOperator op, Type type, ASTPTR inner);

	virtual void accept(Visitor &visitor);

	UnaryOperator getOperator()const;

	AST* getInner()const;
};