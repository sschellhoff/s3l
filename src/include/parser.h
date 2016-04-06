#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <map>
#include <vector>
#include <stack>

#include "environment.h"
#include "operators.h"
#include "token.h"
#include "lexer.h"
#include "function_prototype.h"
#include "ast.h"
#include "function_definition_ast.h"

using ENV = Environment<Variable>;
using ENVSTACK = std::stack<ENV>;
using ASTPTR = std::unique_ptr<AST>;
using FUNCVECPTR = std::unique_ptr<std::vector<std::unique_ptr<FunctionDefinitionAST>>>;

class Parser {
private:
	std::map<Operator, int> operatorPrecedences;
	Token currentToken;
	ENVSTACK environments;
	Type currentFunctionsResultType;
	Lexer lexer;
	std::ostream &errorStream;
	std::vector<FunctionPrototype> functionDefinitions;
	std::vector<FunctionPrototype> functionDeclarations;
	std::unique_ptr<std::vector<std::unique_ptr<FunctionDefinitionAST>>> functionASTs;
	Type stringToType(const std::string &typeString)const;
	bool isValidOperator(const Operator op, const Type t1, const Type t2)const;
	Type determineOperatorResult(const Operator op, const Type t1, const Type t2)const;
	Type getFunctionResultType(const std::string &name, std::vector<Type> &types);
	bool isValidOperator(const UnaryOperator op, const Type t1)const;
	void makeError(const std::string &msg);
	void consumeToken();
	bool isIdentifier(const std::string &name);
	bool tryConsumeIdentifier(const std::string &name);
	bool isFunctionDefined(const std::string &name, Type resultType, std::vector<Type> &types);
	bool isFunctionDefined(const std::string &name, std::vector<Type> &types);
	bool isFunctionDeclared(const std::string &name, Type resultType, std::vector<Type> &types);
	bool isFunctionDeclared(const std::string &name, std::vector<Type> &types);
	bool removeDeclarationIfNeeded(const std::string &name, Type resultType, std::vector<Type> &types);
	std::vector<Type> argumentsToTypes(std::vector<Variable> &arguments)const;
	ASTPTR parsePrimeExpression();
	ASTPTR parseNumberConst();
	ASTPTR parseBoolConst();
	ASTPTR parseIdentifierExpression();
	ASTPTR parseExpression();
	ASTPTR parseBinaryExpressionRHS(int operatorPrecedence, ASTPTR lhs);
	ASTPTR parseReturn();
	ASTPTR parseBlock();
	ASTPTR parseIf();
	ASTPTR parseLoop();
	ASTPTR parseIdentifierStatement();
	bool parseFunctionDefinition();
public:
	Parser(std::ostream &errorStream);
	FUNCVECPTR parse(const std::string &program);
	FUNCVECPTR parseFile(const std::string &filename);
};