#include "include/while_ast.h"

WhileAST::WhileAST(ASTPTR cond, ASTPTR block):AST(Type::VOID), cond(std::move(cond)), block(std::move(block)) {
}

void WhileAST::accept(Visitor &visitor) {
    visitor.visit(this);
}

AST* WhileAST::getCondition()const {
    return cond.get();
}

AST* WhileAST::getBlock()const {
    return block.get();
}