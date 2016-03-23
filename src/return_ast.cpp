#include "include/return_ast.h"

ReturnAST::ReturnAST():AST(Type::VOID) {}

ReturnAST::ReturnAST(ASTPTR result):AST(result->getType()), result(std::move(result)) {}

void ReturnAST::accept(Visitor &visitor) {
	visitor.visit(this);
}

AST *ReturnAST::getResult()const {
	return result ? result.get() : nullptr;
}