#include "include/function_call_ast.h"

FunctionCallAST::FunctionCallAST(const std::string &name):AST(Type::NUMBER), name(name) {
}

const std::string &FunctionCallAST::getName()const {
	return name;
}

void FunctionCallAST::accept(Visitor &visitor) {
	visitor.visit(this);
}