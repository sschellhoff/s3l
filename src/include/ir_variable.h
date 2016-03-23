#pragma once

#include "variable.h"

template <typename E>
class IRVariable : public Variable {
private:
	E *var;
public:
	IRVariable(Type type, const std::string &name, E *var):Variable(type, name), var(var) {
	}
	E *getIRVariable()const {
		return var;
	}
};