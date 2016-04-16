#include "include/lexer.h"
#include "include/AutoCounter.h"

#include <sstream>
#include <algorithm>

Lexer::Lexer() {
	initOperators();
}

Lexer::Lexer(const std::string &input):input(input), currentPos(0), currentLine(1), currentColumn(1) {
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
	operators.push_back(Operator::MOD);

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
	currentColumn = 1;
	currentLine = 1;
	this->input = input;
	currentPos = 0;
}

Token Lexer::getNext() {
	try {

	} catch(int i) {

	}
	skipWhitespacesAndComments();

	AutoCounter ac(currentPos, currentPos, currentColumn);

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
				return Token(TokenType::REAL, index, currentColumn, currentLine);
			} else {
				auto element = std::stoi(buffer.str(), nullptr);
				auto index = addIntConstant(element);
				return Token(TokenType::INT, index, currentColumn, currentLine); // INT
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
				return Token(TokenType::BOOL, 1, currentColumn, currentLine);
			} else if(identifierValue == "false") {
				return Token(TokenType::BOOL, 0, currentColumn, currentLine);
			} else if(identifierValue == "return") {
				return Token(TokenType::RETURN, currentColumn, currentLine);
			} else if(identifierValue == "if") {
				return Token(TokenType::IF, currentColumn, currentLine);
			} else if(identifierValue == "else") {
				return Token(TokenType::ELSE, currentColumn, currentLine);
			} else if(identifierValue == "loop") {
				return Token(TokenType::LOOP, currentColumn, currentLine);
			}
			int index = identifier.size();
			identifier.push_back(buffer.str());
			return Token(TokenType::IDENTIFIER, index, currentColumn, currentLine);
		} else {
			switch(input[currentPos]) {
				case '+':
					currentPos++;
					return Token(TokenType::OPERATOR, 0, currentColumn, currentLine);
				case '-':
					currentPos++;
					return Token(TokenType::OPERATOR, 1, currentColumn, currentLine);
				case '*':
					currentPos++;
					return Token(TokenType::OPERATOR, 2, currentColumn, currentLine);
				case '/':
					currentPos++;
					return Token(TokenType::OPERATOR, 3, currentColumn, currentLine);
				case '<':
					currentPos++;
					if(inputLeft() && input[currentPos] == '=') {
						currentPos++;
						return Token(TokenType::OPERATOR, 5, currentColumn, currentLine);
					}
					return Token(TokenType::OPERATOR, 4, currentColumn, currentLine);
				case '>':
					currentPos++;
					if(inputLeft() && input[currentPos] == '=') {
						currentPos++;
						return Token(TokenType::OPERATOR, 7, currentColumn, currentLine);
					}
					return Token(TokenType::OPERATOR, 6, currentColumn, currentLine);
				case '(':
					currentPos++;
					return Token(TokenType::LPAR, 0, currentColumn, currentLine);
				case ')':
					currentPos++;
					return Token(TokenType::RPAR, 0, currentColumn, currentLine);
				case '{':
					currentPos++;
					return Token(TokenType::LBLOCK, 0, currentColumn, currentLine);
				case '}':
					currentPos++;
					return Token(TokenType::RBLOCK, 1, currentColumn, currentLine);
				case '=':
					currentPos++;
					if(inputLeft() && input[currentPos] == '=') {
						currentPos++;
						return Token(TokenType::OPERATOR, 8, currentColumn, currentLine);
					} else if( inputLeft() && input[currentPos] == '>') {
						currentPos++;
						return Token(TokenType::OPERATOR, 12, currentColumn, currentLine);
					}
					return Token(TokenType::ASSIGN, currentColumn, currentLine);
				case '!':
					currentPos++;
					if(inputLeft() && input[currentPos] == '=') {
						currentPos++;
						return Token(TokenType::OPERATOR, 9, currentColumn, currentLine);
					}
					return Token(TokenType::UNARYOPERATOR, 0, currentColumn, currentLine);
				case '&':
					currentPos++;
					if(inputLeft() && input[currentPos] == '&') {
						currentPos++;
						return Token(TokenType::OPERATOR, 10, currentColumn, currentLine);
					}
					return Token(TokenType::ERROR, currentColumn, currentLine);
				case '|':
					currentPos++;
					if(inputLeft() && input[currentPos] == '|') {
						currentPos++;
						return Token(TokenType::OPERATOR, 11, currentColumn, currentLine);
					}
					return Token(TokenType::ERROR, currentColumn, currentLine);
				case '%':
					currentPos++;
					return Token(TokenType::OPERATOR, 13, currentColumn, currentLine);
				case ';':
					currentPos++;
					return Token(TokenType::SEMICOLON, currentColumn, currentLine);
				case ',':
					currentPos++;
					return Token(TokenType::SEPARATOR, currentColumn, currentLine);
				case ':':
					currentPos++;
					if(inputLeft() && input[currentPos] == '=') {
						currentPos++;
						return Token(TokenType::DEFINE, currentColumn, currentLine);
					}
					return Token(TokenType::ERROR, currentColumn, currentLine);
				default:
				break;
			}
		}
	} else {
		return Token(TokenType::EOI, currentColumn, currentLine);
	}
	currentPos++;
	return Token(TokenType::ERROR, currentColumn, currentLine);
}

void Lexer::skipWhitespacesAndComments() {
	while(skipWhitespaces() || skipBlockComment() || skipComment()) {}
}

bool Lexer::skipWhitespaces() {
	bool result = false;
	for(unsigned int length = input.length(); currentPos < length && isspace(input[currentPos]); currentPos++) {
		result = true;
		if(input[currentPos] == '\n') {
			currentLine++;
			currentColumn = 1;
		} else {
			currentColumn++;
		}
	}
	return result;
}

bool Lexer::skipBlockComment() {
	bool result = false;
	if(input.length() > currentPos + 1 && input[currentPos] == '/' && input[currentPos + 1] == '/') {
		currentPos += 2;
		result = true;
		while(currentPos < input.length() && input[currentPos] != '\n') {
			currentPos++;
			currentColumn;
		}
		if(currentPos < input.length() && input[currentPos] == '\n') {
			currentColumn = 1;
			currentLine++;
		}
	}
	return result;
}

bool Lexer::skipComment() {
	if(input.length() > currentPos + 3 && input[currentPos] == '/' && input[currentPos+1] == '*') {
		currentPos += 2;
		while(input.length() > currentPos + 1) {
			if(input[currentPos] == '*' && input[currentPos+1] == '/') {
				currentPos += 2;
				return true;
			}
			currentPos++;
		}
		throw "";
	}
	return false;
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