#include "include/ir_visitor.h"

#include "llvm/ADT/APFloat.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/raw_os_ostream.h"

void IRVisitor::visit(NumberConstAST *ast) {
	currentValue = llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APFloat(ast->getValue()));
}

void IRVisitor::visit(BoolConstAST *ast) {
	// zu bool Wert ändern
	currentValue = llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APFloat(ast->getValue()?1.0:0.0));
}

void IRVisitor::visit(BinaryExpressionAST *ast) {
	ast->getLHS()->accept(*this);
	llvm::Value *lhs = currentValue;
	ast->getRHS()->accept(*this);
	llvm::Value *rhs = currentValue;
	switch(ast->getOperator()) {
		case Operator::ADD:
		currentValue = builder.CreateFAdd(lhs, rhs, "addtmp");
		break;
		case Operator::SUB:
		currentValue = builder.CreateFSub(lhs, rhs, "dubtmp");
		break;
		case Operator::MULT:
		currentValue = builder.CreateFMul(lhs, rhs, "multmp");
		break;
		case Operator::DIV:
		currentValue = builder.CreateFDiv(lhs, rhs, "divtmp");
		break;
		default:
		fprintf(stderr, "wrong operator\n");
		// error
		// currentValue = nullptr;
		break;
	}
}

void IRVisitor::visit(FunctionCallAST *ast) {
	 auto function = theModule->getFunction(ast->getName());
	 if(!function) {
		fprintf(stderr, "call to unknown function\n");
	 	// error
	 	// unknown function
	 	return;
	 }
	 if(function->arg_size() != 0) {
	 	return;
	 }
	 std::vector<llvm::Value*> argValues;
	 currentValue = builder.CreateCall(function, argValues, ast->getName());
}

void IRVisitor::visit(FunctionDefinitionAST *ast) {
	std::vector<llvm::Type*> args;
	llvm::FunctionType *funcType = llvm::FunctionType::get(llvm::Type::getDoubleTy(llvm::getGlobalContext()), args, false);
	llvm::Function *func = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, ast->getName(), theModule.get());

	if(!func) {
		fprintf(stderr, "could not create function\n");
		// error
		return;
	}
	llvm::BasicBlock *block = llvm::BasicBlock::Create(llvm::getGlobalContext(), "entry", func);
	builder.SetInsertPoint(block);
	ast->getBody()->accept(*this);
	if(currentValue) {
		builder.CreateRet(currentValue);
		llvm::verifyFunction(*func);
		//func->dump();
		return;
	}
	fprintf(stderr, "could not create function body\n");
	// error
	func->eraseFromParent();
}

void IRVisitor::print() {
	if(currentValue) {
		currentValue->dump();
	}
}

void IRVisitor::printModule() {
	if(theModule) {
		theModule->dump();
	}
}

void IRVisitor::writeModule(std::ostream &outStream) {
	llvm::raw_os_ostream out(outStream);
	theModule->print(out, 0, false);
}