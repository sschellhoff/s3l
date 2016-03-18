#pragma once

#include "ast.h"

#include <string>

class FunctionCallAST : public AST {
private:
std::string name;
public:
	FunctionCallAST(const std::string &name);
	const std::string &getName()const;
	virtual void accept(Visitor &visitor);
};