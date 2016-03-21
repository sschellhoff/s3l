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
			}
			auto element = std::stod(buffer.str(), nullptr);
			auto it = std::find(numericalConstants.begin(), numericalConstants.end(), element);
			int index = numericalConstants.size();
			if(it != numericalConstants.end()) {
				index = it - numericalConstants.begin();
			} else {
				numericalConstants.push_back(element);
			}
			return Token(TokenType::NUMBER, index);
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
			}
			int index = identifier.size();
			identifier.push_back(buffer.str());
			return Token(TokenType::IDENTIFIER, index);
		} else {
			switch(input[currentPos]) {
				case '+':
					currentPos++;
					return Token(TokenType::OPERATOR, 0);
				break;
				case '-':
					currentPos++;
					return Token(TokenType::OPERATOR, 1);
				break;
				case '*':
					currentPos++;
					return Token(TokenType::OPERATOR, 2);
				break;
				case '/':
					currentPos++;
					return Token(TokenType::OPERATOR, 3);
				break;
				case '<':
					currentPos++;
					if(inputLeft() && input[currentPos] == '=') {
						currentPos++;
						return Token(TokenType::OPERATOR, 5);
					}
					return Token(TokenType::OPERATOR, 4);
				break;
				case '>':
					currentPos++;
					if(inputLeft() && input[currentPos] == '=') {
						currentPos++;
						return Token(TokenType::OPERATOR, 7);
					}
					return Token(TokenType::OPERATOR, 6);
				break;
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
					return Token(TokenType::ERROR);
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
					return Token(TokenType::SEMICOLON, 0);
				case ',':
					currentPos++;
					return Token(TokenType::SEPARATOR, 0);
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
	return numericalConstants[index];
}

Operator Lexer::getOperator(int index) const {
	return operators[index];
}

UnaryOperator Lexer::getUnaryOperator(int index) const {
	return unaryOperators[index];
}