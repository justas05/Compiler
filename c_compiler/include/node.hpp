#ifndef AST_BASE_HPP
#define AST_BASE_HPP

#include "ast.hpp"


class Node {
public:
    virtual ~Node() {}

    virtual void print() const = 0;
    virtual void printxml() const = 0;
    virtual void printasm() const = 0;
};

    
#endif
