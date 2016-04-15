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
	unsigned short column;
	unsigned short line;
public:
	Token(TokenType type, unsigned short col, unsigned short line):type(type), column(col), line(line) {}
	Token(TokenType type, int valueIndex, unsigned short col, unsigned short line):valueIndex(valueIndex), type(type), column(col), line(line)  {}

	int getValueIndex() const {
		return valueIndex;
	}

	TokenType getTokenType() const {
		return type;
	}

	unsigned short getLine()const {
		return line;
	}

	unsigned short getColumn()const {
		return column;
	}
};