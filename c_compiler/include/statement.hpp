#ifndef AST_STATEMENT_HPP
#define AST_STATEMENT_HPP

#include "bindings.hpp"
#include "declaration.hpp"
#include "expression.hpp"
#include "node.hpp"

#include <memory>

class Statement;

typedef std::shared_ptr<Statement> StatementPtr;


class Statement : public Node {
protected:
    StatementPtr next_statement_;
    
public:
    Statement(Statement* statement = nullptr);

    virtual void print() const = 0;
    virtual void printXml() const = 0;
    virtual VariableStackBindings printAsm(VariableStackBindings bindings, unsigned& label_count) const = 0;

    virtual void countVariables(unsigned& var_count) const = 0;
    virtual void countArguments(unsigned& argument_count) const = 0;

    void linkStatement(Statement* next);
};


class CompoundStatement : public Statement {
protected:
    DeclarationPtr declaration_;
    StatementPtr statement_;
    
public:
    CompoundStatement(Declaration* declaration = nullptr, Statement* statement = nullptr);
    CompoundStatement(Statement* statement);

    virtual void print() const;
    virtual void printXml() const;
    virtual VariableStackBindings printAsm(VariableStackBindings bindings, unsigned& label_count) const;

    virtual void countVariables(unsigned& var_count) const;
    virtual void countArguments(unsigned& argument_count) const;    
};


class SelectionStatement : public Statement {
protected:
    ExpressionPtr condition_;
    StatementPtr if_;
    StatementPtr else_;
public:
    SelectionStatement(Expression* condition, Statement* _if, Statement* _else = nullptr);

    virtual void print() const;
    virtual void printXml() const;
    virtual VariableStackBindings printAsm(VariableStackBindings bindings, unsigned& label_count) const;

    virtual void countVariables(unsigned& var_count) const;
    virtual void countArguments(unsigned& argument_count) const;    
};


class ExpressionStatement : public Statement {
protected:
    ExpressionPtr expression_;
public:
    ExpressionStatement(Expression* expression = nullptr);

    virtual void print() const;
    virtual void printXml() const;
    virtual VariableStackBindings printAsm(VariableStackBindings bindings, unsigned& label_count) const;

    virtual void countVariables(unsigned& var_count) const;
    virtual void countArguments(unsigned& argument_count) const;    
};


class JumpStatement : public Statement {
protected:
    ExpressionPtr expression_;
public:
    JumpStatement(Expression* expression = nullptr);

    virtual void print() const;
    virtual void printXml() const;
    virtual VariableStackBindings printAsm(VariableStackBindings bindings, unsigned& label_count) const;

    virtual void countVariables(unsigned& var_count) const;
    virtual void countArguments(unsigned& argument_count) const;
};


class IterationStatement : public Statement {
protected:
    StatementPtr statement_;
public:
    IterationStatement(Statement* statement);

    virtual void print() const;
    virtual void printXml() const;
    virtual VariableStackBindings printAsm(VariableStackBindings bindings, unsigned& label_count) const;

    virtual void countVariables(unsigned& var_count) const;
    virtual void countArguments(unsigned& argument_count) const;    
};


#endif
