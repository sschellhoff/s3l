#pragma once

#include "ast.h"
#include "variable.h"

class DeclVarAST : public AST {
private:
    Variable variable;
public:
    DeclVarAST(Variable var);
    virtual void accept(Visitor &visitor);
    Variable getVariable()const;
};