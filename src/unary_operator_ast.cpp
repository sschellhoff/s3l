#include "include/unary_operator_ast.h"

UnaryOperatorAST::UnaryOperatorAST(UnaryOperator op, Type type, ASTPTR inner):AST(type), op(op), inner(std::move(inner)) {
}

void UnaryOperatorAST::accept(Visitor &visitor) {
	visitor.visit(this);
}

UnaryOperator UnaryOperatorAST::getOperator()const {
	return op;
}

AST* UnaryOperatorAST::getInner()const {
	return inner.get();
}