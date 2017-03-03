#ifndef AST_STATEMENT_HPP
#define AST_STATEMENT_HPP

#include "ast.hpp"


class Statement : public Node {
public:
    Statement(const Node* _left = new EmptyNode, const Node* _right = new EmptyNode);
};

class CompoundStatement : public Statement {
public:
    CompoundStatement(const Node* _dec = new EmptyNode, const Node* _statement = new EmptyNode);

    virtual void printxml() const override;
};

class SelectionStatement : public Statement {
public:
    SelectionStatement(const Node* _if, const Node* _else = new EmptyNode);
};

class ExpressionStatement : public Statement {
public:
    ExpressionStatement(const Node* expr = new EmptyNode);
};

class JumpStatement : public Statement {
public:
    JumpStatement(const Node* _el);
 
    virtual void printasm() const override;
};

class IterationStatement : public Statement {
public:
    IterationStatement(const Node* _el);
};

#endif
