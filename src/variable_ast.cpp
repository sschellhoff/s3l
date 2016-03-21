#include "include/variable_ast.h"

VariableAST::VariableAST(const std::string &name, Type type):AST(type), name(name) {
}

const std::string &VariableAST::getName()const {
	return name;
}

void VariableAST::accept(Visitor &visitor) {
	visitor.visit(this);
}