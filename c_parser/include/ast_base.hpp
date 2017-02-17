#ifndef AST_BASE_HPP
#define AST_BASE_HPP

#include <string>
#include <iostream>
#include <vector>

class ast_Base {
public:
    virtual ~ast_Base() {}

    virtual void print() const = 0;
    virtual void push(const ast_Base* var) const = 0;
};

#endif
