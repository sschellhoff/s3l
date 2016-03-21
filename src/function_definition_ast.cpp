#include "include/function_definition_ast.h"

FunctionDefinitionAST::FunctionDefinitionAST(const std::string &name, Type resultType, std::unique_ptr<AST> body, std::vector<FunctionArgument> &&arguments):AST(resultType), name(name), body(std::move(body)), arguments(std::move(arguments)) {
}

const std::string &FunctionDefinitionAST::getName()const {
	return name;
}

AST *FunctionDefinitionAST::getBody()const {
	return body.get();
}

void FunctionDefinitionAST::accept(Visitor &visitor) {
	visitor.visit(this);
}