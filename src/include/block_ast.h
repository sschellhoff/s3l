#pragma once

#include "ast.h"

#include <vector>
#include <memory>

using ASTPTR = std::unique_ptr<AST>;
using ASTPTRVEC = std::vector<ASTPTR>;

class BlockAST : public AST {
private:
	ASTPTRVEC statements;
public:
	BlockAST(Type type, ASTPTRVEC &&statements);
	const ASTPTRVEC &getStatements()const;
	virtual void accept(Visitor &visitor);
};