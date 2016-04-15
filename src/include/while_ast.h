#pragma once

#include "ast.h"

#include <memory>

using ASTPTR = std::unique_ptr<AST>;

class WhileAST : public AST {
private:
    std::unique_ptr<AST> cond, block;
public:
    WhileAST(ASTPTR cond, ASTPTR block);

    virtual void accept(Visitor &visitor);

    AST* getCondition()const;

    AST* getBlock()const;
};