#pragma once

#include <memory>
#include <iostream>
#include <stack>
#include <map>

#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"

#include "visitor.h"
#include "make_unique.h"
#include "environment.h"
#include "ir_variable.h"

#include "int_const_ast.h"
#include "real_const_ast.h"
#include "bool_const_ast.h"
#include "binary_expression_ast.h"
#include "function_call_ast.h"
#include "function_definition_ast.h"
#include "unary_operator_ast.h"
#include "variable_ast.h"
#include "return_ast.h"
#include "block_ast.h"
#include "decl_var_ast.h"
#include "assign_var_ast.h"
#include "composite_ast.h"
#include "if_ast.h"

using IRVAR = IRVariable<llvm::Value>;
using IRENV = Environment<IRVAR>;
using IRENVSTACK = std::stack<IRENV>;
using FUNCMAP = std::map<std::string, llvm::Function*>;

class IRVisitor : public Visitor {
private:
	std::unique_ptr<llvm::Module> theModule;
	llvm::IRBuilder<> builder;
	llvm::Value *currentValue;
	IRENVSTACK environments;
	FUNCMAP functionMap;
	llvm::Type *typeConversion(Type type)const;
public:
	IRVisitor():theModule(std::make_unique<llvm::Module>("my llvm module", llvm::getGlobalContext())), builder(llvm::getGlobalContext()){}
	virtual void visit(IntConstAST *ast);
	virtual void visit(RealConstAST *ast);
	virtual void visit(BoolConstAST *ast);
	virtual void visit(BinaryExpressionAST *ast);
	virtual void visit(FunctionCallAST *ast);
	virtual void visit(FunctionDefinitionAST *ast);
	virtual void visit(UnaryOperatorAST *ast);
	virtual void visit(VariableAST *ast);
	virtual void visit(ReturnAST *ast);
	virtual void visit(BlockAST *ast);
	virtual void visit(DeclVarAST *ast);
	virtual void visit(AssignVarAST *ast);
	virtual void visit(CompositeAst *ast);
	virtual void visit(IfAST *ast);

	void print();
	void printModule();
	void writeModule(std::ostream &outStream);
};