#pragma once

#include <string>
#include <sstream>

#include "types.h"
#include "function_prototype.h"

std::string getMangledFunctionName(const std::string &name, std::vector<Type> &types) {
    if(name == "main" && types.empty()) {
        return name;
    }
    std::stringstream sstream;
    sstream << "_Z" << name.length() << name;
    if(types.empty()) {
        sstream << "v";
    } else {
        for(auto t : types) {
            switch(t) {
                case Type::BOOL:
                    sstream << "b";
                    break;
                case Type::REAL:
                    sstream << "d";
                    break;
                case Type::INT:
                    sstream << "i";
                    break;
                default:
                    break;
            }
        }
    }
    return sstream.str();
}