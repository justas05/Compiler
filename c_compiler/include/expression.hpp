#ifndef AST_EXPRESSION_HPP
#define AST_EXPRESSION_HPP

#include "ast.hpp"

class Expression : public Node {
public:
    Expression(const Node* expr = nullptr);

    virtual void print() const;
    virtual void printxml() const;
    virtual void printasm() const;
};

#endif
