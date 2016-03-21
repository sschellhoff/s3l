#pragma once

#include "ast.h"
#include "types.h"

#include <string>

class VariableAST : public AST {
private:
	std::string name;
public:
	VariableAST(const std::string &name, Type type);
	const std::string &getName()const;
	virtual void accept(Visitor &visitor);
};