#pragma once

enum class Operator {
	ADD,
	SUB,
	MULT,
	DIV,
	LT,
	LE,
	GT,
	GE,
	EQ,
	NE,
	AND,
	OR,
	IMPL,
	MOD
};

enum class UnaryOperator {
	NOT,
	NEG
};