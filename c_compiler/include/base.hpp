#ifndef AST_BASE_HPP
#define AST_BASE_HPP

#include <string>
#include <iostream>
#include <vector>

class Base {
public:
    virtual ~Base() {}

    virtual void print() const = 0;
    virtual void push(const Base* _var) const = 0;
};

#endif
