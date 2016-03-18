#pragma once

#include "ast.h"

#include <string>
#include <memory>

class FunctionDefinitionAST : public AST {
private:
std::string name;
std::unique_ptr<AST> body;
public:
	FunctionDefinitionAST(const std::string &name, Type resultType, std::unique_ptr<AST> body);
	const std::string &getName()const;
	AST *getBody()const;
	virtual void accept(Visitor &visitor);
};