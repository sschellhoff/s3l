#pragma once

#include "ast.h"

#include <string>
#include <vector>
#include <memory>

class FunctionCallAST : public AST {
private:
std::string name;
std::vector<std::unique_ptr<AST> > argumentValues;
public:
	FunctionCallAST(const std::string &name, std::vector<std::unique_ptr<AST> > &&arguments);
	const std::string &getName()const;
	const std::vector<std::unique_ptr<AST> > &getArguments()const;
	virtual void accept(Visitor &visitor);
};