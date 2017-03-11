#ifndef AST_STATEMENT_HPP
#define AST_STATEMENT_HPP

#include "node.hpp"

#include <memory>

class Declaration;
class Expression;
class Statement;

typedef std::shared_ptr<Declaration> DeclarationPtr;
typedef std::shared_ptr<Expression> ExpressionPtr;
typedef std::shared_ptr<Statement> StatementPtr;


class Statement : public Node {
protected:
    StatementPtr next_statement;
    
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
    DeclarationPtr m_decl;
    StatementPtr m_statement;
    
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
    StatementPtr m_if;
    StatementPtr m_else;
public:
    SelectionStatement(Statement* _if = nullptr, Statement* _else = nullptr);

    virtual void print() const;
    virtual void printxml() const;
    virtual VariableStackBindings printasm(VariableStackBindings bindings) const;

    virtual void count_variables(int32_t& var_count) const;
};


class ExpressionStatement : public Statement {
protected:
    ExpressionPtr m_expr;
public:
    ExpressionStatement(Expression* expr = nullptr);

    virtual void print() const;
    virtual void printxml() const;
    virtual VariableStackBindings printasm(VariableStackBindings bindings) const;

    virtual void count_variables(int32_t& var_count) const;
};


class JumpStatement : public Statement {
protected:
    ExpressionPtr m_expr;
public:
    JumpStatement(Expression* expr = nullptr);

    virtual void print() const;
    virtual void printxml() const;
    virtual VariableStackBindings printasm(VariableStackBindings bindings) const;

    virtual void count_variables(int32_t& var_count) const;
};


class IterationStatement : public Statement {
protected:
    StatementPtr m_statement;
public:
    IterationStatement(Statement* statement);

    virtual void print() const;
    virtual void printxml() const;
    virtual VariableStackBindings printasm(VariableStackBindings bindings) const;

    virtual void count_variables(int32_t& var_count) const;
};


#endif
