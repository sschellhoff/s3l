#pragma once

#include <string>
#include <vector>

#include "operators.h"
#include "token.h"

class Lexer {
private:
	std::vector<double> realConstants;
	std::vector<int> intConstants;
	std::vector<std::string> identifier;
	std::vector<Operator> operators;
	std::vector<UnaryOperator> unaryOperators;
	std::string input;
	unsigned int currentPos;
	unsigned short currentLine;
	unsigned short currentColumn;
	void skipWhitespaces();
	bool inputLeft()const;
	void initOperators();
	int addRealConstant(double val);
	int addIntConstant(int val);
public:
	Lexer();
	Lexer(const std::string &input);
	void setInput(const std::string &input);
	Token getNext();
	std::string getIdentifierString(int index)const;
	double getDoubleConstant(int index)const;
	double getIntConstant(int index)const;
	Operator getOperator(int index)const;
	UnaryOperator getUnaryOperator(int index)const;
};