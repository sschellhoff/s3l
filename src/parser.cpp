#include "include/parser.h"
#include "include/make_unique.h"
#include "include/number_const_ast.h"
#include "include/bool_const_ast.h"
#include "include/function_call_ast.h"
#include "include/binary_expression_ast.h"
#include "include/function_call_ast.h"

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

bool Parser::isFunctionDefined(const std::string &name) {
	auto result = std::find(functionDefinitions.begin(), functionDefinitions.end(), name);
	return result != functionDefinitions.end();
}

bool Parser::isFunctionDeclared(const std::string &name) {
	auto result = std::find(functionDeclarations.begin(), functionDeclarations.end(), name);
	return result != functionDeclarations.end();
}

bool Parser::removeDeclarationIfNeeded(const std::string &name) {
	auto result = std::find(functionDeclarations.begin(), functionDeclarations.end(), name);
	if(result == functionDeclarations.end()) {
		return false;
	}
	std::swap(*result, functionDeclarations.back());
	functionDeclarations.pop_back();
	return true;
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
		return nullptr; // variable
	}
	consumeToken();
	if(currentToken.getTokenType() != TokenType::RPAR) {
		makeError("missing )");
		return nullptr;
	}
	consumeToken();
	return std::make_unique<FunctionCallAST>(identifierName);
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
	if(currentToken.getTokenType() != TokenType::IDENTIFIER) {
		makeError("function name expected");
		return false;
	}
	std::string functionName = lexer.getIdentifierString(currentToken.getValueIndex());
	consumeToken();
	if(currentToken.getTokenType() != TokenType::LPAR) {
		makeError("missing (");
		return false;
	}
	consumeToken();
	if(currentToken.getTokenType() != TokenType::RPAR) {
		makeError("missing )");
		return false;
	}
	consumeToken();
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
		if(isFunctionDefined(functionName)) {
			makeError("doubled function definition");
			return false;
		}
		removeDeclarationIfNeeded(functionName);
		functionDefinitions.push_back(functionName);
		functionASTs->push_back(std::make_unique<FunctionDefinitionAST>(functionName, body->getType(), std::move(body)));
		return true;
	}
	if(!isFunctionDefined(functionName) && !isFunctionDeclared(functionName)) {
		functionDeclarations.push_back(functionName);
	}
	return true;
}