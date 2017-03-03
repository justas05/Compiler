#ifndef AST_STATEMENT_HPP
#define AST_STATEMENT_HPP

#include "ast.hpp"


class Statement : public BaseNode {
public:
    Statement(const Base* _left = new EmptyNode, const Base* _right = new EmptyNode);
};

class StatementList : public BaseList { 
public:
    StatementList(const Base* _statement);
};

class CompoundStatement : public Statement {
public:
    CompoundStatement(const Base* _dec = new EmptyNode, const Base* _statement = new EmptyNode);

    virtual void printxml() const override;
};

class SelectionStatement : public Statement {
public:
    SelectionStatement(const Base* _if, const Base* _else = new EmptyNode);
};

class ExpressionStatement : public Statement {
public:
    ExpressionStatement(const Base* expr = new EmptyNode);
};

class JumpStatement : public Statement {
public:
    JumpStatement(const Base* _el);
 
    virtual void printasm() const override;
};

class IterationStatement : public Statement {
public:
    IterationStatement(const Base* _el);
};

#endif
