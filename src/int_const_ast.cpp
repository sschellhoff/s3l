#include "include/int_const_ast.h"

IntConstAST::IntConstAST(int value):AST(Type::INT), value(value) {
}

int IntConstAST::getValue() const {
	return value;
}

void IntConstAST::accept(Visitor &visitor) {
	visitor.visit(this);
}