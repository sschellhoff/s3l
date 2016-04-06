#include "include/real_const_ast.h"

RealConstAST::RealConstAST(double value):AST(Type::REAL), value(value) {
}

double RealConstAST::getValue() const {
	return value;
}

void RealConstAST::accept(Visitor &visitor) {
	visitor.visit(this);
}