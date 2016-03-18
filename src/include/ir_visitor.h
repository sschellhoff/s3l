#pragma once

#include <memory>
#include <iostream>
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"

#include "visitor.h"
#include "number_const_ast.h"
#include "bool_const_ast.h"
#include "binary_expression_ast.h"
#include "function_call_ast.h"
#include "function_definition_ast.h"
#include "make_unique.h"

class IRVisitor : public Visitor {
private:
	std::unique_ptr<llvm::Module> theModule;
	llvm::IRBuilder<> builder;
	llvm::Value *currentValue;
public:
	IRVisitor():theModule(std::make_unique<llvm::Module>("my llvm module", llvm::getGlobalContext())), builder(llvm::getGlobalContext()){}
	virtual void visit(NumberConstAST *ast);
	virtual void visit(BoolConstAST *ast);
	virtual void visit(BinaryExpressionAST *ast);
	virtual void visit(FunctionCallAST *ast);
	virtual void visit(FunctionDefinitionAST *ast);
	void print();
	void printModule();
	void writeModule(std::ostream &outStream);
};