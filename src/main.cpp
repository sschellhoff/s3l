#include <iostream>
#include <fstream>

#include "include/token.h"
#include "include/lexer.h"
#include "include/parser.h"
#include "include/ir_visitor.h"

int main(int argc, char *argv[]) {
	Parser parser(std::cerr);
	auto result = (argc == 1 ? parser.parse("acab() aabb() { true * 3 + 1337 } acab() { 42 / 2 }") : parser.parseFile(argv[1]));
	if(nullptr != result) {
		std::cout << "parsing complete" << std::endl;

		IRVisitor ir_builder;
		for(auto &ast : *result) {
			ast->accept(ir_builder);
		}
		ir_builder.printModule();
		std::ofstream file;
		file.open("check.ir");
		ir_builder.writeModule(file);
		file.close();
		return 0;
	} else {
		std::cout << "error while parsing" << std::endl;
		return -1;
	}
}