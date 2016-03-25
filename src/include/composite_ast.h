#pragma once

#include "ast.h"

#include <memory>

using ASTPTR = std::unique_ptr<AST>;

class CompositeAst : public AST {
private:
    ASTPTR first;
    ASTPTR second;
public:
    CompositeAst(ASTPTR &&first, ASTPTR &&second);
    AST *getFirst()const;
    AST *getSecond()const;
    virtual void accept(Visitor &visitor);
};