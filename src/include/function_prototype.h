#pragma once

#include "types.h"

#include <string>
#include <vector>

#include "variable.h"

class FunctionPrototype {
private:
	std::string name;
	Type result;
	std::vector<Variable> arguments;
public:
	FunctionPrototype(const std::string &name, Type resultType, std::vector<Variable> arguments):name(name), result(resultType), arguments(arguments) {}
	const std::string &getName()const {
		return name;
	}
	Type getResultType()const {
		return result;
	}
	bool equals(const std::string &name, Type resultType, std::vector<Type> &argumentTypes) {
		if(this->name != name)
			return false;
		if(this->result != resultType)
			return false;
		if(this->arguments.size() != argumentTypes.size())
			return false;
		
		auto currentArg = arguments.begin();
		auto currentType = argumentTypes.begin();
		while(currentArg != arguments.end()) {
			if(currentArg->getType() != *currentType) {
				return false;
			}
			currentArg++;
			currentType++;
		}
		return true;
	}
};