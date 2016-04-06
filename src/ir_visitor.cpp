#include "include/ir_visitor.h"

#include "llvm/IR/Verifier.h"
#include "llvm/Support/raw_os_ostream.h"

llvm::Type *IRVisitor::typeConversion(Type type)const {
	switch(type) {
		case Type::INT:
			return llvm::IntegerType::get(llvm::getGlobalContext(), 32);
		case Type::BOOL:
			return llvm::IntegerType::get(llvm::getGlobalContext(), 1);
		case Type::REAL:
			return llvm::Type::getDoubleTy(llvm::getGlobalContext());
		default:
			fprintf(stderr, "wrong operator\n");
			return nullptr;
	}
}

void IRVisitor::visit(IntConstAST *ast) {
	currentValue = llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(32, ast->getValue(), true));
}

void IRVisitor::visit(RealConstAST *ast) {
	currentValue = llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APFloat(ast->getValue()));
}

void IRVisitor::visit(BoolConstAST *ast) {
	currentValue = llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(1, ast->getValue()?1:0, true));
}

void IRVisitor::visit(BinaryExpressionAST *ast) {
	ast->getLHS()->accept(*this);
	llvm::Value *lhs = currentValue;
	ast->getRHS()->accept(*this);
	llvm::Value *rhs = currentValue;
	auto type = ast->getLHS()->getType();
	if (type == Type::REAL) {
		switch (ast->getOperator()) {
			case Operator::ADD:
				currentValue = builder.CreateFAdd(lhs, rhs, "addFloat");
				break;
			case Operator::SUB:
				currentValue = builder.CreateFSub(lhs, rhs, "subFloat");
				break;
			case Operator::MULT:
				currentValue = builder.CreateFMul(lhs, rhs, "multFloat");
				break;
			case Operator::DIV:
				currentValue = builder.CreateFDiv(lhs, rhs, "divFloat");
				break;
			case Operator::EQ:
				currentValue = builder.CreateFCmpOEQ(lhs, rhs, "eqFloat");
				break;
			case Operator::NE:
				currentValue = builder.CreateFCmpUNE(lhs, rhs, "neFloat");
				break;
			case Operator::LT:
				currentValue = builder.CreateFCmpULT(lhs, rhs, "ltFloat");
				break;
			case Operator::LE:
				currentValue = builder.CreateFCmpULE(lhs, rhs, "leFloat");
				break;
			case Operator::GT:
				currentValue = builder.CreateFCmpUGT(lhs, rhs, "gtFloat");
				break;
			case Operator::GE:
				currentValue = builder.CreateFCmpUGE(lhs, rhs, "geFloat");
				break;
			default:
				fprintf(stderr, "wrong operator\n");
				// error
				// currentValue = nullptr;
				break;
		}
	} else if(type == Type::INT) {
		fprintf(stderr, "int not implemented in ir\n");

	} else if(type == Type::BOOL) {
		switch(ast->getOperator()) {
			case Operator::EQ:
			currentValue = builder.CreateICmpEQ(lhs, rhs, "eqBool");
			break;
			case Operator::NE:
			currentValue = builder.CreateICmpNE(lhs, rhs, "neBool");
			break;
			case Operator::AND:
			currentValue = builder.CreateAnd(lhs, rhs, "andBool");
			break;
			case Operator::OR:
			currentValue = builder.CreateOr(lhs, rhs, "orBool");
			break;
			case Operator::ADD:
			currentValue = builder.CreateXor(lhs, rhs, "xorBool");
			break;
			case Operator::IMPL:
			{
				// lhs xor true == !lhs
				auto true_const = llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(1, 1));
				lhs = builder.CreateXor(lhs, true_const, "xorBool");
				// !lhs or rhs == lhs => rhs
				currentValue = builder.CreateOr(lhs, rhs, "orBool");
			}
			break;
			default:
			fprintf(stderr, "wrong operator\n");
			// error
			// currentValue = nullptr;
			break;
		}
	}
}

void IRVisitor::visit(FunctionCallAST *ast) {
	 auto function = theModule->getFunction(ast->getName());
	 if(!function) {
		fprintf(stderr, "call to unknown function\n");
	 	return;
	 }
	 auto &arguments = ast->getArguments();
	 std::vector<llvm::Value*> argValues;
	 for(auto &arg : arguments) {
	 	arg->accept(*this);
	 	argValues.push_back(currentValue);
	 }
	 if(function->getReturnType()->isVoidTy()) {
		 currentValue = builder.CreateCall(function, argValues, "");
	 } else {
		 currentValue = builder.CreateCall(function, argValues, ast->getName());
	 }
}

void IRVisitor::visit(FunctionDefinitionAST *ast) {
	environments.push(IRENV());
	auto arguments = ast->getArguments();
	std::vector<llvm::Type*> args;
	for(auto &arg : arguments) {
		args.push_back(typeConversion(arg.getType()));
	}
	llvm::FunctionType *funcType;
	if(ast->getType() == Type::INT) {
		funcType = llvm::FunctionType::get(llvm::Type::getInt32Ty(llvm::getGlobalContext()), args, false);
	} else if(ast->getType() == Type::BOOL) {
		funcType = llvm::FunctionType::get(llvm::Type::getInt1Ty(llvm::getGlobalContext()), args, false);
	} else if(ast->getType() == Type::REAL) {
		funcType = llvm::FunctionType::get(llvm::Type::getDoubleTy(llvm::getGlobalContext()), args, false);
	} else if(ast->getType() == Type::VOID) {
		funcType = llvm::FunctionType::get(llvm::Type::getVoidTy(llvm::getGlobalContext()), args, false);
	} else {
		fprintf(stderr, "could not create function\n");
		// error
		return;
	}

	llvm::Function *func = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, ast->getName(), theModule.get());

	if(!func) {
		fprintf(stderr, "could not create function\n");
		// error
		return;
	}

	llvm::BasicBlock *block = llvm::BasicBlock::Create(llvm::getGlobalContext(), "entry", func);
	builder.SetInsertPoint(block);

	auto arg_ir = func->arg_begin();
	auto arg_ir_end = func->arg_end();
	auto arg = arguments.begin();
	auto arg_end = arguments.end();
	for(; arg_ir != arg_ir_end && arg != arg_end; arg_ir++, arg++) {
		arg_ir->setName(arg->getName());
		auto arg_alloc = builder.CreateAlloca(typeConversion(arg->getType()), 0, arg->getName());
		builder.CreateStore(arg_ir, arg_alloc, false);
		environments.top().add(arg->getName(), IRVAR(arg->getType(), arg->getName(), arg_alloc));
	}

	ast->getBody()->accept(*this);
	if(currentValue) {
		//builder.CreateRet(currentValue);
		llvm::verifyFunction(*func);
		//func->dump();
		environments.pop();
		return;
	}
	fprintf(stderr, "could not create function body\n");
	// error
	func->eraseFromParent();
	environments.pop();
}

void IRVisitor::visit(UnaryOperatorAST *ast) {
	switch(ast->getOperator()) {
		case UnaryOperator::NOT:
		{
			// a xor true == !a
			auto true_const = llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(1, 1));
			ast->getInner()->accept(*this);
			currentValue = builder.CreateXor(currentValue, true_const, "xorBool");
			break;
		}
		case UnaryOperator::NEG:
		{
			// 0 - a == -a
			auto zero_const = llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APFloat(0.0));
			ast->getInner()->accept(*this);
			currentValue = builder.CreateFSub(zero_const, currentValue, "subFloat");
			break;
		}
		default:
		{
			fprintf(stderr, "unknown unary operator");
			return;
		}
	}
}

void IRVisitor::visit(VariableAST *ast) {
	auto name = ast->getName();
	if(!environments.top().isDefined(name)) {
		fprintf(stderr, "Variable(%s) not found\n", name.c_str());
		return;
	}
	auto var = environments.top().getValue(name).getIRVariable();
	currentValue = builder.CreateLoad(var, false, name);
}

void IRVisitor::visit(ReturnAST *ast) {
	if(ast->getType() != Type::VOID) {
		ast->getResult()->accept(*this);
		if(currentValue) {
			builder.CreateRet(currentValue);
		}
	} else {
		builder.CreateRet(nullptr);		
	}
}

void IRVisitor::visit(BlockAST *ast) {

	environments.push(IRENV(&environments.top()));
	auto &statements = ast->getStatements();
	for(auto &statement : statements) {
		statement->accept(*this);
	}
	environments.pop();
}

void IRVisitor::visit(DeclVarAST *ast) {
	auto var = ast->getVariable();
	auto type = var.getType();
	auto name = var.getName();
	auto arg_alloc = builder.CreateAlloca(typeConversion(type), 0, name);
	environments.top().add(name, IRVAR(type, name, arg_alloc));
}

void IRVisitor::visit(AssignVarAST *ast) {
	auto var = environments.top().getValue(ast->getVariable().getName());
	ast->getExpression()->accept(*this);
	builder.CreateStore(currentValue, var.getIRVariable(), false);
}

void IRVisitor::visit(CompositeAst *ast) {
	ast->getFirst()->accept(*this);
	ast->getSecond()->accept(*this);
}

void IRVisitor::visit(IfAST *ast) {
	// must be done!!
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
