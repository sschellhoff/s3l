#pragma once

#include <string>
#include <sstream>
#include <exception>

class IRException : public std::exception {
private:
    std::string msg;
public:
    IRException(const std::string &msg) : msg(msg) {}
    virtual const char *what()const throw() {
        std::stringstream sstream;
        sstream << "IR ERROR - " << msg << std::endl;
        std::string result = sstream.str();
        return result.c_str();
    }
};