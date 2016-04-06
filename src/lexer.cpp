#include "include/lexer.h"
#include "include/token.h"

#include <sstream>
#include <algorithm>

Lexer::Lexer() {
	initOperators();
}

Lexer::Lexer(const std::string &input):input(input), currentPos(0) {
	initOperators();
}


void Lexer::initOperators() {
	operators.push_back(Operator::ADD);
	operators.push_back(Operator::SUB);
	operators.push_back(Operator::MULT);
	operators.push_back(Operator::DIV);
	operators.push_back(Operator::LT);
	operators.push_back(Operator::LE);
	operators.push_back(Operator::GT);
	operators.push_back(Operator::GE);
	operators.push_back(Operator::EQ);
	operators.push_back(Operator::NE);
	operators.push_back(Operator::AND);
	operators.push_back(Operator::OR);
	operators.push_back(Operator::IMPL);

	unaryOperators.push_back(UnaryOperator::NOT);
}

int Lexer::addRealConstant(double val) {
	auto it = std::find(realConstants.begin(), realConstants.end(), val);
	int index = realConstants.size();
	if(it != realConstants.end()) {
		index = it - realConstants.begin();
	} else {
		realConstants.push_back(val);
	}
	return index;
}

int Lexer::addIntConstant(int val) {
	auto it = std::find(intConstants.begin(), intConstants.end(), val);
	int index = intConstants.size();
	if(it != intConstants.end()) {
		index = it - intConstants.begin();
	} else {
		intConstants.push_back(val);
	}
	return index;
}

void Lexer::setInput(const std::string &input) {
	this->input = input;
	currentPos = 0;
}

Token Lexer::getNext() {
	skipWhitespaces();
	if(inputLeft()) {
		if(isdigit(input[currentPos])) {
			std::stringstream buffer;
			buffer << input[currentPos];
			currentPos++;
			while(inputLeft() && isdigit(input[currentPos])) {
				buffer << input[currentPos];
				currentPos++;
			}
			if(inputLeft() && input[currentPos] == '.') {
				currentPos++;
				buffer << '.';
				while(inputLeft() && isdigit(input[currentPos])) {
					buffer << input[currentPos];
					currentPos++;
				}
				auto element = std::stod(buffer.str(), nullptr);
				auto index = addRealConstant(element);
				return Token(TokenType::REAL, index);
			} else {
				auto element = std::stoi(buffer.str(), nullptr);
				auto index = addIntConstant(element);
				return Token(TokenType::INT, index); // INT
			}
		} else if(isalpha(input[currentPos]) || input[currentPos] == '_') {
			std::stringstream buffer;
			buffer << input[currentPos];
			currentPos++;
			while(inputLeft() && (isalnum(input[currentPos]) || input[currentPos] == '_')) {
				buffer << input[currentPos];
				currentPos++;
			}
			auto identifierValue = buffer.str();
			if(identifierValue == "true") {
				return Token(TokenType::BOOL, 1);
			} else if(identifierValue == "false") {
				return Token(TokenType::BOOL, 0);
			} else if(identifierValue == "return") {
				return Token(TokenType::RETURN, 0);
			} else if(identifierValue == "if") {
				return Token(TokenType::IF);
			} else if(identifierValue == "else") {
				return Token(TokenType::ELSE);
			} else if(identifierValue == "loop") {
				return Token(TokenType::LOOP);
			}
			int index = identifier.size();
			identifier.push_back(buffer.str());
			return Token(TokenType::IDENTIFIER, index);
		} else {
			switch(input[currentPos]) {
				case '+':
					currentPos++;
					return Token(TokenType::OPERATOR, 0);
				case '-':
					currentPos++;
					return Token(TokenType::OPERATOR, 1);
				case '*':
					currentPos++;
					return Token(TokenType::OPERATOR, 2);
				case '/':
					currentPos++;
					return Token(TokenType::OPERATOR, 3);
				case '<':
					currentPos++;
					if(inputLeft() && input[currentPos] == '=') {
						currentPos++;
						return Token(TokenType::OPERATOR, 5);
					}
					return Token(TokenType::OPERATOR, 4);
				case '>':
					currentPos++;
					if(inputLeft() && input[currentPos] == '=') {
						currentPos++;
						return Token(TokenType::OPERATOR, 7);
					}
					return Token(TokenType::OPERATOR, 6);
				case '(':
					currentPos++;
					return Token(TokenType::LPAR, 0);
				case ')':
					currentPos++;
					return Token(TokenType::RPAR, 0);
				case '{':
					currentPos++;
					return Token(TokenType::LBLOCK, 0);
				case '}':
					currentPos++;
					return Token(TokenType::RBLOCK, 1);
				case '=':
					currentPos++;
					if(inputLeft() && input[currentPos] == '=') {
						currentPos++;
						return Token(TokenType::OPERATOR, 8);
					} else if( inputLeft() && input[currentPos] == '>') {
						currentPos++;
						return Token(TokenType::OPERATOR, 12);
					}
					return Token(TokenType::ASSIGN);
				case '!':
					currentPos++;
					if(inputLeft() && input[currentPos] == '=') {
						currentPos++;
						return Token(TokenType::OPERATOR, 9);
					}
					return Token(TokenType::UNARYOPERATOR, 0);
				case '&':
					currentPos++;
					if(inputLeft() && input[currentPos] == '&') {
						currentPos++;
						return Token(TokenType::OPERATOR, 10);
					}
					return Token(TokenType::ERROR);
				case '|':
					currentPos++;
					if(inputLeft() && input[currentPos] == '|') {
						currentPos++;
						return Token(TokenType::OPERATOR, 11);
					}
					return Token(TokenType::ERROR);
				case ';':
					currentPos++;
					return Token(TokenType::SEMICOLON);
				case ',':
					currentPos++;
					return Token(TokenType::SEPARATOR);
				case ':':
					currentPos++;
					if(inputLeft() && input[currentPos] == '=') {
						currentPos++;
						return Token(TokenType::DEFINE);
					}
					return Token(TokenType::ERROR);
				default:
				break;
			}
		}
	} else {
		return Token(TokenType::EOI);
	}
	currentPos++;
	return Token(TokenType::ERROR);
}

void Lexer::skipWhitespaces() {
	for(unsigned int length = input.length(); currentPos < length && isspace(input[currentPos]); currentPos++) {
	}
}

bool Lexer::inputLeft() const {
	return currentPos < input.length();
}

std::string Lexer::getIdentifierString(int index) const {
	return identifier[index];
}

double Lexer::getDoubleConstant(int index) const {
	return realConstants[index];
}

double Lexer::getIntConstant(int index) const {
	return intConstants[index];
}

Operator Lexer::getOperator(int index) const {
	return operators[index];
}

UnaryOperator Lexer::getUnaryOperator(int index) const {
	return unaryOperators[index];
}