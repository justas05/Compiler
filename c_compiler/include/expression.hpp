#ifndef AST_EXPRESSION_HPP
#define AST_EXPRESSION_HPP

#include "ast.hpp"

class Expression : public BaseNode {
private:
public:
    Expression(const Base* expr = new EmptyNode);

    virtual void printasm() const override;
};

#endif
