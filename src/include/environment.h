#pragma once

#include <string>
#include <map>
#include <assert.h>

template <typename v>
class Environment {
public:
	Environment():parent(nullptr) {}
	Environment(Environment *parent):parent(parent) {}

	bool isDefined(const std::string &name)const {
		auto it = values.find(name);
		return it != values.end() || (parent != nullptr && parent->isDefined(name));
	}

	v getValue(const std::string &name)const {
		auto it = values.find(name);
		if(it == values.end()) {
			assert(parent != nullptr && "use of undefined variable");
			return parent->getValue(name);
		}
		return it->second;
	}

	void add(const std::string &name, v val) {
		values.insert(std::pair<std::string, v>(name, val));
	}
private:
	std::map<std::string, v> values;
	Environment *parent;
};