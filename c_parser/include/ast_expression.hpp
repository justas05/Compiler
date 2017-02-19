#ifndef AST_EXPRESSION_HPP
#define AST_EXPRESSION_HPP

#include "ast.hpp"

#include <string>
#include <iostream>

class ast_Expression : public ast_Base {
private:
public:
    ast_Expression() {}

    virtual void print() const {
	
    }

    virtual void push(const ast_Base* _base) const {
	std::cerr << "Can't call this function for this type" << std::endl;
	(void)_base;
    }
};

#endif
