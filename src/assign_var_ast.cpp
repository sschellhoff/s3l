#include "include/assign_var_ast.h"

AssignVarAST::AssignVarAST(Variable var, ASTPTR &&expression):AST(Type::VOID), var(var), expression(std::move(expression)) {

}

Variable AssignVarAST::getVariable()const {
    return var;
}

AST *AssignVarAST::getExpression()const {
    return expression.get();
}

void AssignVarAST::accept(Visitor &visitor) {
    visitor.visit(this);
}