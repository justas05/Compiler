#ifndef AST_EXPRESSION_HPP
#define AST_EXPRESSION_HPP

#include "ast.hpp"

class Expression : public Node {
private:
public:
    Expression(const Node* expr = new EmptyNode);

    virtual void printasm() const override;
};

#endif
