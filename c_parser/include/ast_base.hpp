#ifndef AST_EXPRESSION_HPP
#define AST_EXPRESSION_HPP

#include <string>
#include <iostream>

class ast_Base {
public:
    virtual ~ast_Base() {}

    virtual void print() const = 0;
};

#endif
