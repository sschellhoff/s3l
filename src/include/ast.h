#pragma once

#include "types.h"
#include "visitor.h"

class AST {
private:
	Type type;
public:
	AST(Type type):type(type) {}
	virtual ~AST() {}

	Type getType() const {
		return type;
	}

	virtual void accept(Visitor &visitor) = 0;
};