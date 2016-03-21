#pragma once

class NumberConstAST;
class BoolConstAST;
class BinaryExpressionAST;
class FunctionCallAST;
class FunctionDefinitionAST;
class UnaryOperatorAST;
class VariableAST;

class Visitor {
public:
	virtual void visit(NumberConstAST *ast)=0;
	virtual void visit(BoolConstAST *ast)=0;
	virtual void visit(BinaryExpressionAST *ast)=0;
	virtual void visit(FunctionCallAST *ast)=0;
	virtual void visit(FunctionDefinitionAST *ast)=0;
	virtual void visit(UnaryOperatorAST *ast)=0;
	virtual void visit(VariableAST *ast)=0;
};