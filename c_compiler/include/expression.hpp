#ifndef AST_EXPRESSION_HPP
#define AST_EXPRESSION_HPP

#include "ast.hpp"

#include <string>
#include <iostream>

class Expression : public Base {
private:
public:
    Expression() {}

    virtual void print() const {
	
    }

    virtual void push(const Base* _base) const {
	std::cerr << "Can't call this function for this type" << std::endl;
	(void)_base;
    }
};

#endif
