#include "include/function_call_ast.h"

FunctionCallAST::FunctionCallAST(const std::string &name, std::vector<std::unique_ptr<AST> > &&arguments):AST(Type::NUMBER), name(name), argumentValues(std::move(arguments)) {
}

const std::string &FunctionCallAST::getName()const {
	return name;
}

const std::vector<std::unique_ptr<AST> > &FunctionCallAST::getArguments()const {
	return argumentValues;
}

void FunctionCallAST::accept(Visitor &visitor) {
	visitor.visit(this);
}