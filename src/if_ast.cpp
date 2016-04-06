#include "include/if_ast.h"

IfAST::IfAST(ASTPTR cond, ASTPTR thenBlock):AST(Type::VOID), cond(std::move(cond)), thenBlock(std::move(thenBlock)), elseBlock(nullptr) {
}

IfAST::IfAST(ASTPTR cond, ASTPTR thenBlock, ASTPTR elseBlock):AST(Type::VOID), cond(std::move(cond)), thenBlock(std::move(thenBlock)), elseBlock(std::move(elseBlock)) {
}

void IfAST::accept(Visitor &visitor) {
    visitor.visit(this);
}

AST* IfAST::getCondition()const {
    return cond.get();
}

AST* IfAST::getThen()const {
    return thenBlock.get();
}

AST* IfAST::getElse()const {
    return elseBlock.get();
}