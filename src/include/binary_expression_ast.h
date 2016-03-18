#pragma once

#include "ast.h"
#include "operators.h"

#include <memory>

using ASTPTR = std::unique_ptr<AST>;

class BinaryExpressionAST : public AST {
private:
	Operator op;
	std::unique_ptr<AST> lhs, rhs;
public:
	BinaryExpressionAST(Operator op, ASTPTR lhs, ASTPTR rhs);

	virtual void accept(Visitor &visitor);

	Operator getOperator()const;

	AST* getLHS()const;
	
	AST* getRHS()const;
};