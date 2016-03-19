#include "include/binary_expression_ast.h"

BinaryExpressionAST::BinaryExpressionAST(Operator op, Type type, ASTPTR lhs, ASTPTR rhs):AST(type), op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {
}

void BinaryExpressionAST::accept(Visitor &visitor) {
	visitor.visit(this);
}

Operator BinaryExpressionAST::getOperator()const {
	return op;
}

AST* BinaryExpressionAST::getLHS()const {
	return lhs.get();
}

AST* BinaryExpressionAST::getRHS()const {
	return rhs.get();
}