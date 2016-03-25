#pragma once

class NumberConstAST;
class BoolConstAST;
class BinaryExpressionAST;
class FunctionCallAST;
class FunctionDefinitionAST;
class UnaryOperatorAST;
class VariableAST;
class ReturnAST;
class BlockAST;
class DeclVarAST;
class AssignVarAST;
class CompositeAst;

class Visitor {
public:
	virtual void visit(NumberConstAST *ast)=0;
	virtual void visit(BoolConstAST *ast)=0;
	virtual void visit(BinaryExpressionAST *ast)=0;
	virtual void visit(FunctionCallAST *ast)=0;
	virtual void visit(FunctionDefinitionAST *ast)=0;
	virtual void visit(UnaryOperatorAST *ast)=0;
	virtual void visit(VariableAST *ast)=0;
	virtual void visit(ReturnAST *ast)=0;
	virtual void visit(BlockAST *ast)=0;
	virtual void visit(DeclVarAST *ast)=0;
	virtual void visit(AssignVarAST *ast)=0;
	virtual void visit(CompositeAst *ast)=0;
};