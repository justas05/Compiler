#ifndef AST_EXPRESSION_HPP
#define AST_EXPRESSION_HPP

#include "ast.hpp"

#include <string>
#include <iostream>

class ast_Expression : public ast_Base {
private:
    std::string id;
public:
    ast_Expression(const std::string& _id) : id(_id) {}

    virtual void print() const {
	
    }

    virtual void push(const ast_Base* _base) const {
	std::cerr << "Can't call this function for this type" << std::endl;
	(void)_base;
    }
};

class ast_ReturnExpression : public ast_Expression {
public:
    ast_ReturnExpression(const std::string& _id) : ast_Expression(_id) {}
};

#endif
