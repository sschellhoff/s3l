#pragma once

#include <string>
#include <vector>

#include "operators.h"
#include "token.h"

class Lexer {
private:
	std::vector<double> numericalConstants;
	std::vector<std::string> identifier;
	std::vector<Operator> operators;
	std::vector<UnaryOperator> unaryOperators;
	std::string input;
	unsigned int currentPos;
	void skipWhitespaces();
	bool inputLeft()const;
	void initOperators();
public:
	Lexer();
	Lexer(const std::string &input);
	void setInput(const std::string &input);
	Token getNext();
	std::string getIdentifierString(int index)const;
	double getDoubleConstant(int index)const;
	Operator getOperator(int index)const;
	UnaryOperator getUnaryOperator(int index)const;
};