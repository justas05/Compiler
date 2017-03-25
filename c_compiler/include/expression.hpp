#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include "bindings.hpp"
#include "node.hpp"
#include "type.hpp"

#include <cstdint>
#include <memory>
#include <string>

class Expression;
typedef std::shared_ptr<Expression> ExpressionPtr;

class Expression : public Node
{
private:
    ExpressionPtr next_expression_;
    
public:
    virtual VariableStackBindings printAsm(VariableStackBindings bindings, unsigned& label_count) const = 0;
    
    virtual int constantFold() const;
    virtual void print() const;
    virtual void printXml() const;
    virtual void countArguments(unsigned &argument_count) const;
    virtual void expressionDepth(unsigned &depth_count) const;
    virtual std::string id() const;
       
    void linkExpression(Expression* next_expression);
    ExpressionPtr nextExpression() const;
};

class OperationExpression : public Expression
{
protected:
    ExpressionPtr lhs_;
    ExpressionPtr rhs_;
public:
    OperationExpression(Expression *lhs, Expression *rhs);
    OperationExpression(ExpressionPtr lhs, Expression *rhs);

    virtual VariableStackBindings printAsm(VariableStackBindings bindings, unsigned &label_count) const = 0;
    
    virtual int constantFold() const;
    virtual void expressionDepth(unsigned &depth_count) const;

    ExpressionPtr getLhs() const;
    ExpressionPtr getRhs() const;
    
    void evaluateExpression(VariableStackBindings bindings, unsigned &label_count) const;
};

class UnaryExpression : public Expression
{
public:
    virtual VariableStackBindings printAsm(VariableStackBindings bindings, unsigned &label_count) const = 0;

    virtual void stackPosition(VariableStackBindings bindings, unsigned &depth_count) const;
};

class PostfixArrayElement : public UnaryExpression
{
private:
    ExpressionPtr postfix_expression_;
    ExpressionPtr index_expression_;
    
public:
    PostfixArrayElement(Expression *postfix_expression, Expression *index_expression);

    virtual VariableStackBindings printAsm(VariableStackBindings bindings, unsigned &label_count) const;
    virtual void expressionDepth(unsigned &depth_count) const;
    virtual void stackPosition(VariableStackBindings bindings, unsigned &depth_count) const;
};

class PostfixFunctionCall : public UnaryExpression
{
private:
    ExpressionPtr postfix_expression_;
    ExpressionPtr argument_expression_list_;
    
public:
    PostfixFunctionCall(Expression *argument_expression_list = nullptr);

    virtual VariableStackBindings printAsm(VariableStackBindings bindings, unsigned &label_count) const;
    virtual void countArguments(unsigned &argument_count) const;
    virtual void expressionDepth(unsigned &depth_count) const;
    
    void setPostfixExpression(Expression *postfix_expression);
};

class PostfixPostIncDecExpression : public UnaryExpression
{
private:
    std::string operator_;
    ExpressionPtr postfix_expression_;

public:
    PostfixPostIncDecExpression(const std::string &_operator, Expression *postfix_expression);

    virtual VariableStackBindings printAsm(VariableStackBindings bindings, unsigned &label_count) const;
    
};

class UnaryPreIncDecExpression : public UnaryExpression
{
private:
    std::string operator_;
    ExpressionPtr unary_expression_;
    
public:
    UnaryPreIncDecExpression(const std::string &_operator, Expression *unary_expression);

    virtual VariableStackBindings printAsm(VariableStackBindings bindings, unsigned &label_count) const;
};

class OperatorUnaryExpression : public UnaryExpression
{
private:
    std::string operator_;
    ExpressionPtr cast_expression_;

public:
    OperatorUnaryExpression(const std::string &_operator, Expression *cast_expression);

    virtual VariableStackBindings printAsm(VariableStackBindings bindings, unsigned &label_count) const;
    virtual void stackPosition(VariableStackBindings bindings, unsigned &depth_count) const;
};


class CastExpression : public Expression
{
private:
    TypePtr type_;
    ExpressionPtr expression_;

public:
    CastExpression(Type *type, Expression *expression);

    virtual VariableStackBindings printAsm(VariableStackBindings bindings, unsigned &label_count) const;
    virtual void expressionDepth(unsigned &depth_count) const;
};

class AdditiveExpression : public OperationExpression
{
private:
    std::string operator_;

public:
    AdditiveExpression(Expression *lhs, const std::string &_operator, Expression *rhs);

    virtual VariableStackBindings printAsm(VariableStackBindings bindings, unsigned &label_count) const;
    virtual int constantFold() const;
};

class MultiplicativeExpression : public OperationExpression
{
private:
    std::string operator_;

public:
    MultiplicativeExpression(Expression *lhs, const std::string &_operator, Expression *rhs);

    virtual VariableStackBindings printAsm(VariableStackBindings bindings, unsigned &label_count) const;
    virtual int constantFold() const;
};

class ShiftExpression : public OperationExpression
{
private:
    std::string operator_;
public:
    ShiftExpression(Expression *lhs, const std::string &_operator, Expression *rhs);

    virtual VariableStackBindings printAsm(VariableStackBindings bindings, unsigned &label_count) const;
    virtual int constantFold() const;
};

class RelationalExpression : public OperationExpression
{
private:
    std::string operator_;
public:
    RelationalExpression(Expression *lhs, const std::string &_operator, Expression *rhs);

    virtual VariableStackBindings printAsm(VariableStackBindings bindings, unsigned &label_count) const;
    virtual int constantFold() const;
};

class EqualityExpression : public OperationExpression
{
private:
    std::string operator_;
public:
    EqualityExpression(Expression *lhs, const std::string &_operator, Expression *rhs);

    virtual VariableStackBindings printAsm(VariableStackBindings bindings, unsigned &label_count) const;
    virtual int constantFold() const;
};

class AndExpression : public OperationExpression
{
public:
    AndExpression(Expression *lhs, Expression *rhs);

    virtual VariableStackBindings printAsm(VariableStackBindings bindings, unsigned &label_count) const;
    virtual int constantFold() const;
};

class ExclusiveOrExpression : public OperationExpression
{
public:
    ExclusiveOrExpression(Expression *lhs, Expression *rhs);

    virtual VariableStackBindings printAsm(VariableStackBindings bindings, unsigned &label_count) const;
    virtual int constantFold() const;
};

class InclusiveOrExpression : public OperationExpression
{
public:
    InclusiveOrExpression(Expression *lhs, Expression *rhs);

    virtual VariableStackBindings printAsm(VariableStackBindings bindings, unsigned &label_count) const;
    virtual int constantFold() const; 
};

class LogicalAndExpression : public OperationExpression
{
public:
    LogicalAndExpression(Expression *lhs, Expression *rhs);

    virtual VariableStackBindings printAsm(VariableStackBindings bindings, unsigned &label_count) const;
    virtual int constantFold() const;
};

class LogicalOrExpression : public OperationExpression
{
public:
    LogicalOrExpression(Expression *lhs, Expression *rhs);

    virtual VariableStackBindings printAsm(VariableStackBindings bindings, unsigned &label_count) const;
    virtual int constantFold() const;    
};

class ConditionalExpression : public Expression
{
private:
    ExpressionPtr logical_or_;
    ExpressionPtr expression_;
    ExpressionPtr conditional_expression_;

public:
    ConditionalExpression(Expression *logical_or, Expression *expression,
			  Expression *conditional_expression);
    
    virtual VariableStackBindings printAsm(VariableStackBindings bindings, unsigned &label_count) const;
};

class AssignmentExpression : public OperationExpression
{
public:
    AssignmentExpression(Expression *lhs, Expression *rhs);
    AssignmentExpression(ExpressionPtr lhs, Expression *rhs);

    virtual VariableStackBindings printAsm(VariableStackBindings bindings, unsigned &label_count) const;
};

class Identifier : public UnaryExpression
{
private:
    std::string id_;
public:
    Identifier(const std::string &id);

    virtual VariableStackBindings printAsm(VariableStackBindings bindings, unsigned &label_count) const;
    virtual void stackPosition(VariableStackBindings bindings, unsigned &depth_count) const;
    virtual std::string id() const;
};

class Constant : public UnaryExpression
{
private:
    int32_t constant_;
public:
    Constant(const int32_t &constant);

    virtual VariableStackBindings printAsm(VariableStackBindings bindings, unsigned &label_count) const;
    virtual int constantFold() const;
};

#endif
