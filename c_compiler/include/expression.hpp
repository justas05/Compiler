#ifndef AST_EXPRESSION_HPP
#define AST_EXPRESSION_HPP

#include "ast.hpp"

class Expression : public BaseNode {
private:
public:
    Expression() : BaseNode() {}

    virtual void print() const override {}
    virtual void printxml() const override {}
    virtual void printasm() const override {}
};

#endif
