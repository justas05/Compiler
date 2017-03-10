#ifndef AST_EXPRESSION_HPP
#define AST_EXPRESSION_HPP

#include "node.hpp"


class Expression : public Node
{
public:
    virtual ~Expression();
    
    virtual VariableStackBindings printasm(VariableStackBindings bindings) const = 0;
    
    virtual void print() const;
    virtual void printxml() const;
    virtual int32_t getPostfixStackPosition(VariableStackBindings bindings) const;
};


class OperationExpression : public Expression
{
protected:
    Expression* lhs;
    Expression* rhs;
public:
    OperationExpression(Expression* _lhs, Expression* _rhs);
    virtual ~OperationExpression();

    virtual VariableStackBindings printasm(VariableStackBindings bindings) const = 0;
};


class AssignmentExpression : public OperationExpression
{
public:
    AssignmentExpression(Expression* _lhs, Expression* _rhs);

    virtual VariableStackBindings printasm(VariableStackBindings bindings) const;
};


class AdditiveExpression : public OperationExpression
{
private:
    std::string operation;

public:
    AdditiveExpression(Expression* _lhs, const std::string& _operation, Expression* _rhs);

    virtual VariableStackBindings printasm(VariableStackBindings bindings) const;
};


class Identifier : public Expression
{
private:
    std::string m_id;
public:
    Identifier(const std::string& id);

    virtual VariableStackBindings printasm(VariableStackBindings bindings) const;
    virtual int32_t getPostfixStackPosition(VariableStackBindings bindings) const;
};


class Constant : public Expression
{
private:
    int32_t m_constant;
public:
    Constant(const int32_t& constant);

    virtual VariableStackBindings printasm(VariableStackBindings bindings) const;
};


#endif
