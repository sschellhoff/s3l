#pragma once

#include "types.h"

#include <string>

class Variable {
private:
	Type type;
	std::string name;
public:
	Variable(Type type, const std::string &name):type(type), name(name) {}
	Type getType()const {
		return type;
	}
	const std::string &getName()const {
		return name;
	}
};