#include "include/composite_ast.h"

CompositeAst::CompositeAst(ASTPTR &&first, ASTPTR &&second)
        :AST(first->getType() != Type::VOID ? first->getType() : second->getType()),
         first(std::move(first)),
         second(std::move(second)) {
}

AST *CompositeAst::getFirst()const {
    return first.get();
}

AST *CompositeAst::getSecond()const {
    return second.get();
}

void CompositeAst::accept(Visitor &visitor) {
    visitor.visit(this);
}