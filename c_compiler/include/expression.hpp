#ifndef AST_EXPRESSION_HPP
#define AST_EXPRESSION_HPP

#include "node.hpp"
#include "bindings.hpp"

#include <cstdint>
#include <memory>
#include <string>

class Expression;

typedef std::shared_ptr<Expression> ExpressionPtr;


class Expression : public Node
{
public:
    virtual VariableStackBindings printAsm(VariableStackBindings bindings) const = 0;
    
    virtual void print() const;
    virtual void printXml() const;
    virtual int postfixStackPosition(VariableStackBindings bindings) const;
};


class OperationExpression : public Expression
{
protected:
    ExpressionPtr lhs_;
    ExpressionPtr rhs_;
public:
    OperationExpression(Expression* lhs, Expression* rhs);

    virtual VariableStackBindings printAsm(VariableStackBindings bindings) const = 0;
};


class AssignmentExpression : public OperationExpression
{
public:
    AssignmentExpression(Expression* lhs, Expression* rhs);

    virtual VariableStackBindings printAsm(VariableStackBindings bindings) const;
};


class AdditiveExpression : public OperationExpression
{
private:
    std::string operation_;

public:
    AdditiveExpression(Expression* lhs, const std::string& operation, Expression* rhs);

    virtual VariableStackBindings printAsm(VariableStackBindings bindings) const;
};


class MultiplicativeExpression : public OperationExpression
{
private:
    std::string operation_;

public:
    MultiplicativeExpression(Expression* lhs, const std::string& operation, Expression* rhs);

    virtual VariableStackBindings printAsm(VariableStackBindings bindings) const;
};


class Identifier : public Expression
{
private:
    std::string id_;
public:
    Identifier(const std::string& id);

    virtual VariableStackBindings printAsm(VariableStackBindings bindings) const;
    virtual int postfixStackPosition(VariableStackBindings bindings) const;
};


class Constant : public Expression
{
private:
    int32_t constant_;
public:
    Constant(const int32_t& constant);

    virtual VariableStackBindings printAsm(VariableStackBindings bindings) const;
};


#endif
