#pragma once

#include "ast.h"

#include <memory>

using ASTPTR = std::unique_ptr<AST>;

class ReturnAST : public AST {
private:
	ASTPTR result;
public:
	ReturnAST();
	ReturnAST(ASTPTR result);
	virtual void accept(Visitor &visitor);
	AST *getResult()const;
};