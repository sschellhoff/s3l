#include "include/parser.h"
#include "include/make_unique.h"
#include "include/variable.h"
#include "include/number_const_ast.h"
#include "include/bool_const_ast.h"
#include "include/function_call_ast.h"
#include "include/binary_expression_ast.h"
#include "include/function_call_ast.h"
#include "include/unary_operator_ast.h"
#include "include/variable_ast.h"

#include <algorithm>
#include <fstream>

Parser::Parser(std::ostream &errorStream):currentToken(TokenType::ERROR), errorStream(errorStream), functionASTs(std::make_unique<std::vector<std::unique_ptr<FunctionDefinitionAST>>>()) {
	operatorPrecedences[Operator::OR] = 30;
	operatorPrecedences[Operator::AND] = 40;
	operatorPrecedences[Operator::EQ] = 50;
	operatorPrecedences[Operator::NE] = 50;
	operatorPrecedences[Operator::LT] = 60;
	operatorPrecedences[Operator::LE] = 60;
	operatorPrecedences[Operator::GT] = 60;
	operatorPrecedences[Operator::GE] = 60;
	operatorPrecedences[Operator::ADD] = 70;
	operatorPrecedences[Operator::SUB] = 80;
	operatorPrecedences[Operator::MULT] = 90;
	operatorPrecedences[Operator::DIV] = 100;
}

bool Parser::isValidOperator(const Operator op, const Type t1, const Type t2)const {
	if (t1 != t2 || t1 == Type::VOID) {
		return false;
	}
	switch(op) {
		case Operator::EQ:
		case Operator::NE:
			return true;
		case Operator::ADD:
			return t1 == Type::NUMBER || t1 == Type::BOOL;
		case Operator::SUB:
		case Operator::MULT:
		case Operator::DIV:
		case Operator::LT:
		case Operator::LE:
		case Operator::GT:
		case Operator::GE:
			return t1 == Type::NUMBER;
		case Operator::AND:
		case Operator::OR:
		case Operator::IMPL:
			return t1 == Type::BOOL;
		default:
			return false;
	}
}

Type Parser::determineOperatorResult(const Operator op, const Type t1, const Type t2)const {
	switch(op) {
		case Operator::EQ:
		case Operator::NE:
		case Operator::LT:
		case Operator::LE:
		case Operator::GT:
		case Operator::GE:
		case Operator::AND:
		case Operator::OR:
		case Operator::IMPL:
			return Type::BOOL;
		case Operator::ADD:
		case Operator::SUB:
		case Operator::MULT:
		case Operator::DIV:
			return t1;
		default:
			return Type::VOID;
	}
}

bool Parser::isValidOperator(const UnaryOperator op, const Type t1)const {
	switch(op) {
		case UnaryOperator::NOT:
			return t1 == Type::BOOL;
		case UnaryOperator::NEG:
			return t1 == Type::NUMBER;
		default:
			return false;
	}
}

void Parser::makeError(const std::string &msg) {
	errorStream << msg << std::endl;
}

void Parser::consumeToken() {
	currentToken = lexer.getNext();
}

bool Parser::isIdentifier(const std::string &name) {
	return currentToken.getTokenType() == TokenType::IDENTIFIER && lexer.getIdentifierString(currentToken.getValueIndex()) == name;
}

bool Parser::tryConsumeIdentifier(const std::string &name) {
	if(isIdentifier(name)) {
		consumeToken();
		return true;
	}
	return false;
}

Type Parser::stringToType(const std::string &typeString)const {
	if(typeString == "bool") {
		return Type::BOOL;
	} else if(typeString == "number") {
		return Type::NUMBER;
	} else {
		return Type::VOID;
	}
}

bool Parser::isFunctionDefined(const std::string &name, Type resultType, std::vector<Type> &types) {
	for(auto &funcDef : functionDefinitions) {
		if(funcDef.equals(name, resultType, types)) {
			return true;
		}
	}
	return false;
}

bool Parser::isFunctionDeclared(const std::string &name, Type resultType, std::vector<Type> &types) {
	for(auto &funcDef : functionDeclarations) {
		if(funcDef.equals(name, resultType, types)) {
			return true;
		}
	}
	return false;
}

bool Parser::removeDeclarationIfNeeded(const std::string &name, Type resultType, std::vector<Type> &types) {
	for(auto it = functionDeclarations.begin(); it != functionDeclarations.end(); it++) {
		if(it->equals(name, resultType, types)) {
			std::swap(*it, functionDeclarations.back());
			functionDeclarations.pop_back();
			return true;
		}
	}
	return false;
}

std::vector<Type> Parser::argumentsToTypes(std::vector<Variable> &arguments)const {
	std::vector<Type> result(arguments.size());
	for(auto &arg : arguments) {
		result.push_back(arg.getType());
	}
	return result;
}

FUNCVECPTR Parser::parse(const std::string &program) {
	lexer.setInput(program);
	consumeToken();
	while(currentToken.getTokenType() == TokenType::IDENTIFIER) {
		if(!parseFunctionDefinition()) {
			return nullptr;
		}
	}
	if(currentToken.getTokenType() != TokenType::EOI) {
		makeError("unused tokens detected");
		return nullptr;
	}
	if(!functionDeclarations.empty()) {
		makeError("functions declared but not defined");
		return nullptr;
	}
	return std::move(functionASTs);
}

FUNCVECPTR Parser::parseFile(const std::string &filename) {
	std::ifstream inputStream(filename);
	std::string program(
		(std::istreambuf_iterator<char>(inputStream) ),
		(std::istreambuf_iterator<char>() ) );
	return std::move(parse(program));
}

ASTPTR Parser::parsePrimeExpression() {
	switch(currentToken.getTokenType()) {
		case TokenType::NUMBER:
			return parseNumberConst();
		case TokenType::BOOL:
			return parseBoolConst();
		case TokenType::LPAR:
		{
			consumeToken();
			auto result = parseExpression();
			if(currentToken.getTokenType() != TokenType::RPAR) {
				makeError("missing )");
				return nullptr;
			}
			consumeToken();
			return result;
		}
		case TokenType::IDENTIFIER:
			return parseIdentifierExpression();
		case TokenType::UNARYOPERATOR:
		{
			auto unOp = currentToken;
			consumeToken();
			auto inner = parsePrimeExpression();
			if(!inner) {
				return nullptr;
			}
			auto op = lexer.getUnaryOperator(unOp.getValueIndex());
			if(!isValidOperator(op, inner->getType())) {
				makeError("wrong inner type for unary expression");
				return nullptr;
			}
			return std::make_unique<UnaryOperatorAST>(op, inner->getType(), std::move(inner));
		}
		case TokenType::OPERATOR:
		{
			if(lexer.getOperator(currentToken.getValueIndex()) != Operator::SUB) {
				makeError("could not use binary operator as unary prefix operator");
				return nullptr;
			}
			consumeToken();
			auto inner = parsePrimeExpression();
			if(!inner) {
				return nullptr;
			}
			auto op = UnaryOperator::NEG;
			if(!isValidOperator(op, inner->getType())) {
				makeError("wrong inner type for unary expression");
				return nullptr;
			}
			return std::make_unique<UnaryOperatorAST>(op, inner->getType(), std::move(inner));
		}
		default:
			makeError("primary expression expected");
			return nullptr;
	}
}

ASTPTR Parser::parseNumberConst() {
	if(currentToken.getTokenType() != TokenType::NUMBER) {		
		makeError("number constant expected");
		return nullptr;
	}
	auto result = std::make_unique<NumberConstAST>(lexer.getDoubleConstant(currentToken.getValueIndex()));
	consumeToken();
	return std::move(result);
}

ASTPTR Parser::parseBoolConst() {
	if(currentToken.getTokenType() != TokenType::BOOL) {
		makeError("bool constant expected");
		return nullptr;
	}
	auto result = std::make_unique<BoolConstAST>(currentToken.getValueIndex() == 1);
	consumeToken();
	return std::move(result);
}

ASTPTR Parser::parseIdentifierExpression() {
	if(currentToken.getTokenType() != TokenType::IDENTIFIER) {
		makeError("identifier expected");
		return nullptr;
	}
	std::string identifierName = lexer.getIdentifierString(currentToken.getValueIndex());
	consumeToken();
	if(currentToken.getTokenType() != TokenType::LPAR) {
		if(!environments.top().isDefined(identifierName)) {
			makeError("used of undeclared variable");
			return nullptr;
		}
		auto var = environments.top().getValue(identifierName);
		auto type = var.getType();
		return std::make_unique<VariableAST>(identifierName, type);
	}
	consumeToken();

	std::vector<ASTPTR> arguments;
	while(currentToken.getTokenType() != TokenType::RPAR) {
		auto arg = parseExpression();
		if(!arg) {
			makeError("error parsing arguments in function call");
			return nullptr;
		}
		arguments.push_back(std::move(arg));
		if(currentToken.getTokenType() != TokenType::RPAR) {
			if(currentToken.getTokenType() == TokenType::SEPARATOR) {
				consumeToken();
			} else {
				makeError("error parsing arguments in function call");
				return nullptr;
			}
		}
	}

	if(currentToken.getTokenType() != TokenType::RPAR) {
		makeError("missing )");
		return nullptr;
	}
	consumeToken();
	return std::make_unique<FunctionCallAST>(identifierName, std::move(arguments));
}

ASTPTR Parser::parseExpression() {
	auto lhs = parsePrimeExpression();
	if(!lhs) {
		return nullptr;
	}
	return parseBinaryExpressionRHS(0, std::move(lhs));
}

ASTPTR Parser::parseBinaryExpressionRHS(int operatorPrecedence, ASTPTR lhs) {
	while(true) {
		if(currentToken.getTokenType() != TokenType::OPERATOR) {
			return lhs;
		}
		int currentOperatorPrecedence = operatorPrecedences[lexer.getOperator(currentToken.getValueIndex())];
		if(currentOperatorPrecedence < operatorPrecedence) {
			return lhs;
		}
		Token binaryOperator = currentToken;
		consumeToken();
		auto rhs = parsePrimeExpression();
		if(!rhs) {
			return nullptr;
		}
		if(currentToken.getTokenType() == TokenType::OPERATOR) {
			int nextOperatorPrecedence = operatorPrecedences[lexer.getOperator(currentToken.getValueIndex())];
			if(currentOperatorPrecedence < nextOperatorPrecedence) {
				rhs = parseBinaryExpressionRHS(currentOperatorPrecedence + 1, std::move(rhs));
				if(!rhs) {
					return nullptr;
				}
			}
		}
		auto op = lexer.getOperator(binaryOperator.getValueIndex());
		auto typeLHS = lhs->getType();
		auto typeRHS = rhs->getType();
		if(!isValidOperator(op, typeLHS, typeRHS)) {
			makeError("wrong operands");
			return nullptr;
		}
		Type resultType = determineOperatorResult(op, lhs->getType(), rhs->getType());
		if (resultType == Type::VOID) {
			makeError("unknown operator");
			return nullptr;
		}
		lhs = std::make_unique<BinaryExpressionAST>(op, resultType, std::move(lhs), std::move(rhs));

	}
}

bool Parser::parseFunctionDefinition() {
	environments.push(ENV());
	if(currentToken.getTokenType() != TokenType::IDENTIFIER) {
		makeError("function name expected");
		return false;
	}
	std::string functionName = lexer.getIdentifierString(currentToken.getValueIndex());
	std::vector<Variable> arguments;
	std::vector<Type> argumentTypes = argumentsToTypes(arguments);
	consumeToken();
	if(currentToken.getTokenType() != TokenType::LPAR) {
		makeError("missing (");
		return false;
	}
	consumeToken();

	while(currentToken.getTokenType() == TokenType::IDENTIFIER) {
		std::string argName = lexer.getIdentifierString(currentToken.getValueIndex());
		consumeToken();
		if(currentToken.getTokenType() != TokenType::IDENTIFIER) {
			makeError("missing arguments type");
			return false;
		}
		Type argType = stringToType(lexer.getIdentifierString(currentToken.getValueIndex()));
		consumeToken();
		if(argType == Type::VOID) {
			makeError("unknown argument type");
			return false;
		}
		if(environments.top().isAlreadyDefined(argName)) {
			makeError("argument name already in use");
			return false;
		}
		environments.top().add(argName, Variable(argType, argName));
		arguments.push_back(Variable(argType, argName));
		if(currentToken.getTokenType() != TokenType::SEPARATOR) {
			if(currentToken.getTokenType() != TokenType::RPAR) {
				makeError("error in function definition - parameter list");
				return nullptr;
			}
			break;
		}
		consumeToken();
	}

	if(currentToken.getTokenType() != TokenType::RPAR) {
		makeError("missing )");
		return false;
	}
	consumeToken();
	Type resultType = Type::VOID;

	if(currentToken.getTokenType() == TokenType::IDENTIFIER) {
		auto typeString = lexer.getIdentifierString(currentToken.getValueIndex());
		consumeToken();
		resultType = stringToType(typeString);
		if(resultType == Type::VOID) {
			makeError("unknown function result type");
			return false;
		}
	}
	if(currentToken.getTokenType() == TokenType::LBLOCK) {
		consumeToken();
		auto body = parseExpression();
		if(!body) {
			makeError("function body expected");
			return false;
		}
		if(currentToken.getTokenType() != TokenType::RBLOCK) {
			makeError("missing }");
			return false;
		}
		consumeToken();
		if(isFunctionDefined(functionName, resultType, argumentTypes)) {
			makeError("doubled function definition");
			return false;
		}
		removeDeclarationIfNeeded(functionName, resultType, argumentTypes);
		functionDefinitions.push_back(FunctionPrototype(functionName, resultType, arguments));
		if(body->getType() != resultType) {
			makeError("result of function body and function prototype do not match");
			return false;
		}
		functionASTs->push_back(std::make_unique<FunctionDefinitionAST>(functionName, resultType, std::move(body), arguments));
		environments.pop();
		return true;
	} else if(currentToken.getTokenType() == TokenType::SEMICOLON) {
		if(!isFunctionDefined(functionName, resultType, argumentTypes) && !isFunctionDeclared(functionName, resultType, argumentTypes)) {
			functionDeclarations.push_back(FunctionPrototype(functionName, resultType, std::move(arguments)));
		}
		environments.pop();
		return true;
	} else {
		makeError("missing ; in function declaration");
		return false;
	}
}