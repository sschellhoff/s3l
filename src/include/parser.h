#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <map>
#include <vector>

#include "operators.h"
#include "token.h"
#include "lexer.h"
#include "function_prototype.h"
#include "ast.h"
#include "function_definition_ast.h"

using ASTPTR = std::unique_ptr<AST>;
using FUNCVECPTR = std::unique_ptr<std::vector<std::unique_ptr<FunctionDefinitionAST>>>;

class Parser {
private:
	std::map<Operator, int> operatorPrecedences;
	Token currentToken;
	Lexer lexer;
	std::ostream &errorStream;
	std::vector<FunctionPrototype> functionDefinitions;
	std::vector<FunctionPrototype> functionDeclarations;
	std::unique_ptr<std::vector<std::unique_ptr<FunctionDefinitionAST>>> functionASTs;
	bool isValidOperator(const Operator op, const Type t1, const Type t2)const;
	Type determineOperatorResult(const Operator op, const Type t1, const Type t2)const;
	void makeError(const std::string &msg);
	void consumeToken();
	bool isIdentifier(const std::string &name);
	bool tryConsumeIdentifier(const std::string &name);
	bool isFunctionDefined(const std::string &name, Type resultType);
	bool isFunctionDeclared(const std::string &name, Type resultType);
	bool removeDeclarationIfNeeded(const std::string &name, Type resultType);
	ASTPTR parsePrimeExpression();
	ASTPTR parseNumberConst();
	ASTPTR parseBoolConst();
	ASTPTR parseIdentifierExpression();
	ASTPTR parseExpression();
	ASTPTR parseBinaryExpressionRHS(int operatorPrecedence, ASTPTR lhs);
	bool parseFunctionDefinition();
public:
	Parser(std::ostream &errorStream);
	FUNCVECPTR parse(const std::string &program);
	FUNCVECPTR parseFile(const std::string &filename);
};