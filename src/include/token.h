#pragma once

enum class TokenType {
	ERROR,
	EOI,
	IDENTIFIER,
	INT,
	REAL,
	BOOL,
	OPERATOR,
	LPAR,
	RPAR,
	LBLOCK,
	RBLOCK,
	SEMICOLON,
	UNARYOPERATOR,
	SEPARATOR,
	RETURN,
	ASSIGN,
	DEFINE,
	IF,
	ELSE,
	LOOP
};

class Token {
private:
	int valueIndex;
	TokenType type;
public:
	Token(TokenType type):type(type) {}
	Token(TokenType type, int valueIndex):valueIndex(valueIndex), type(type) {}

	int getValueIndex() const {
		return valueIndex;
	}

	TokenType getTokenType() const {
		return type;
	}
};