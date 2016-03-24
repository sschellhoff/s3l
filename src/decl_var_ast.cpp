#include "include/decl_var_ast.h"

DeclVarAST::DeclVarAST(Variable var):AST(Type::VOID), variable(var) { }

Variable DeclVarAST::getVariable()const {
    return variable;
}

void DeclVarAST::accept(Visitor &visitor) {
    visitor.visit(this);
}