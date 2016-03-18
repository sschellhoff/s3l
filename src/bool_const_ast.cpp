#include "include/bool_const_ast.h"

BoolConstAST::BoolConstAST(bool value):AST(Type::BOOL), value(value) {
}

bool BoolConstAST::getValue() const {
	return value;
}

void BoolConstAST::accept(Visitor &visitor) {
	visitor.visit(this);
}