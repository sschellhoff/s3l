#pragma once

#include "ast.h"
#include "variable.h"

#include <memory>

using ASTPTR = std::unique_ptr<AST>;

class AssignVarAST : public AST {
private:
    Variable var;
    ASTPTR expression;
public:
    AssignVarAST(Variable var, ASTPTR &&expression);
    Variable getVariable()const;
    AST *getExpression()const;
    virtual void accept(Visitor &visitor);
};
