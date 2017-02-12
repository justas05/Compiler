#ifndef AST_EXPRESSION_HPP
#define AST_EXPRESSION_HPP

#include <string>
#include <iostream>

class Expression {
public:
    virtual ~Expression() {}

    virtual void print() const = 0;
};

#endif
