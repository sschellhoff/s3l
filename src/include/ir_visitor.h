#pragma once

#include <memory>
#include <iostream>
#include <stack>

#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"

#include "visitor.h"
#include "number_const_ast.h"
#include "bool_const_ast.h"
#include "binary_expression_ast.h"
#include "function_call_ast.h"
#include "function_definition_ast.h"
#include "unary_operator_ast.h"
#include "variable_ast.h"
#include "make_unique.h"
#include "environment.h"
#include "ir_variable.h"

using IRVAR = IRVariable<llvm::Value>;
using IRENV = Environment<IRVAR>;
using IRENVSTACK = std::stack<IRENV>;

class IRVisitor : public Visitor {
private:
	std::unique_ptr<llvm::Module> theModule;
	llvm::IRBuilder<> builder;
	llvm::Value *currentValue;
	IRENVSTACK environments;
	llvm::Type *typeConversion(Type type)const;
public:
	IRVisitor():theModule(std::make_unique<llvm::Module>("my llvm module", llvm::getGlobalContext())), builder(llvm::getGlobalContext()){}
	virtual void visit(NumberConstAST *ast);
	virtual void visit(BoolConstAST *ast);
	virtual void visit(BinaryExpressionAST *ast);
	virtual void visit(FunctionCallAST *ast);
	virtual void visit(FunctionDefinitionAST *ast);
	virtual void visit(UnaryOperatorAST *ast);
	virtual void visit(VariableAST *ast);

	void print();
	void printModule();
	void writeModule(std::ostream &outStream);
};