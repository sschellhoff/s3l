#pragma once

#include "ast.h"

#include <memory>

using ASTPTR = std::unique_ptr<AST>;

class IfAST : public AST {
private:
    std::unique_ptr<AST> cond, thenBlock, elseBlock;
public:
    IfAST(ASTPTR cond, ASTPTR thenBlock);

    IfAST(ASTPTR cond, ASTPTR thenBlock, ASTPTR elseBlock);

    virtual void accept(Visitor &visitor);

    AST* getCondition()const;

    AST* getThen()const;

    AST* getElse()const;
};