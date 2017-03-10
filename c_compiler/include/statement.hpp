#ifndef AST_STATEMENT_HPP
#define AST_STATEMENT_HPP

#include "node.hpp"

class Declaration;
class Expression;


class Statement : public Node {
protected:
    Statement* next_statement;
    
public:
    Statement(Statement* statement = nullptr);

    virtual void print() const = 0;
    virtual void printxml() const = 0;
    virtual VariableStackBindings printasm(VariableStackBindings bindings) const = 0;

    virtual void count_variables(int32_t& var_count) const = 0;

    void addStatement(Statement* _next);
};


class CompoundStatement : public Statement {
protected:
    Declaration* m_decl;
    Statement* m_statement;
    
public:
    CompoundStatement(Declaration* decl = nullptr, Statement* statement = nullptr);
    CompoundStatement(Statement* statement);

    virtual void print() const;
    virtual void printxml() const;
    virtual VariableStackBindings printasm(VariableStackBindings bindings) const;

    virtual void count_variables(int32_t& var_count) const;
};


class SelectionStatement : public Statement {
protected:
    Statement* m_if;
    Statement* m_else;
public:
    SelectionStatement(Statement* _if = nullptr, Statement* _else = nullptr);

    virtual void print() const;
    virtual void printxml() const;
    virtual VariableStackBindings printasm(VariableStackBindings bindings) const;

    virtual void count_variables(int32_t& var_count) const;
};


class ExpressionStatement : public Statement {
protected:
    Expression* m_expr;
public:
    ExpressionStatement(Expression* expr = nullptr);

    virtual void print() const;
    virtual void printxml() const;
    virtual VariableStackBindings printasm(VariableStackBindings bindings) const;

    virtual void count_variables(int32_t& var_count) const;
};


class JumpStatement : public Statement {
protected:
    Expression* m_expr;
public:
    JumpStatement(Expression* expr = nullptr);

    virtual void print() const;
    virtual void printxml() const;
    virtual VariableStackBindings printasm(VariableStackBindings bindings) const;

    virtual void count_variables(int32_t& var_count) const;
};


class IterationStatement : public Statement {
protected:
    Statement* m_statement;
public:
    IterationStatement(Statement* statement);

    virtual void print() const;
    virtual void printxml() const;
    virtual VariableStackBindings printasm(VariableStackBindings bindings) const;

    virtual void count_variables(int32_t& var_count) const;
};


#endif
