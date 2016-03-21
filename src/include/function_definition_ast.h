#pragma once

#include "ast.h"
#include "variable.h"

#include <string>
#include <memory>
#include <vector>

class FunctionDefinitionAST : public AST {
private:
std::string name;
std::unique_ptr<AST> body;
std::vector<Variable> arguments;
public:
	FunctionDefinitionAST(const std::string &name, Type resultType, std::unique_ptr<AST> body, std::vector<Variable> arguments);
	const std::string &getName()const;
	AST *getBody()const;
	const std::vector<Variable> &getArguments()const;
	virtual void accept(Visitor &visitor);
};