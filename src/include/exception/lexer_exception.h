#pragma once

#include <string>
#include <sstream>
#include <exception>

class LexerException : public std::exception {
private:
    unsigned short line;
    unsigned short column;
    std::string msg;
public:
    ParseException(const std::string &msg, unsigned short line, unsigned short column):line(line), column(column), msg(msg) {}
    unsigned short getLine()const { return line; }
    unsigned short getColumn()const { return column; }
    virtual const char *what()const throw() {
        std::stringstream sstream;
        sstream << "LEXING ERROR - line: " << line << ", column: "  << column << ", \"" << msg << "\"" << std::endl;
        std::string result = sstream.str();
        return result.c_str();
    }
};