#pragma once

#include <iostream>

#include "visitor.h"
#include "number_const_ast.h"
#include "bool_const_ast.h"
#include "binary_expression_ast.h"
#include "function_call_ast.h"
#include "function_definition_ast.h"

const char* beautifyOperator(Operator op) {
	switch(op) {
		case Operator::ADD:
			return "+";
		case Operator::SUB:
			return "-";
		case Operator::MULT:
			return "*";
		case Operator::DIV:
			return "/";
		default:
			return "unknown operator";
	}
}

class PrintVisitor : public Visitor {
	virtual void visit(NumberConstAST *ast) {
		std::cout << "value: " << ast->getValue() << std::endl;
	}

	virtual void visit(BoolConstAST *ast) {
		std::cout << "value: " << (ast->getValue()==1) << std::endl;
	}

	virtual void visit(BinaryExpressionAST *ast) {
		std::cout << beautifyOperator(ast->getOperator()) << std::endl;
		ast->getLHS()->accept(*this);
		ast->getRHS()->accept(*this);
	}

	virtual void visit(FunctionCallAST *ast) {
		std::cout << ast->getName() << "()" << std::endl;
	}

	virtual void visit(FunctionDefinitionAST *ast) {
		std::cout << ast->getName() << "() {" << std::endl;
		ast->getBody()->accept(*this);
		std::cout << "}" << std::endl << std::endl;
	}
};