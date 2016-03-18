#include "include/number_const_ast.h"

NumberConstAST::NumberConstAST(double value):AST(Type::NUMBER), value(value) {
}

double NumberConstAST::getValue() const {
	return value;
}

void NumberConstAST::accept(Visitor &visitor) {
	visitor.visit(this);
}