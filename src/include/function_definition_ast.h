#pragma once

#include "ast.h"
#include "function_argument.h"

#include <string>
#include <memory>
#include <vector>

class FunctionDefinitionAST : public AST {
private:
std::string name;
std::unique_ptr<AST> body;
std::vector<FunctionArgument> arguments;
public:
	FunctionDefinitionAST(const std::string &name, Type resultType, std::unique_ptr<AST> body, std::vector<FunctionArgument> &&arguments);
	const std::string &getName()const;
	AST *getBody()const;
	virtual void accept(Visitor &visitor);
};