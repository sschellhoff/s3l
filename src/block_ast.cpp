#include "include/block_ast.h"

BlockAST::BlockAST(Type type, ASTPTRVEC &&statements):AST(type), statements(std::move(statements)) {
}

const ASTPTRVEC &BlockAST::getStatements()const {
	return statements;
}

void BlockAST::accept(Visitor &visitor) {
	visitor.visit(this);
}