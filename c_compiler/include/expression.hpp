#ifndef AST_EXPRESSION_HPP
#define AST_EXPRESSION_HPP

#include "node.hpp"


class Expression : public Node {
public:
    virtual void print() const;
    virtual void printxml() const;
    virtual VariableStackBindings printasm(VariableStackBindings bindings) const = 0;
};


class Identifier : public Expression {
private:
    std::string m_id;
public:
    Identifier(const std::string& id);

    virtual VariableStackBindings printasm(VariableStackBindings bindings) const;
};


class Constant : public Expression {
private:
    int32_t m_constant;
public:
    Constant(const int32_t& constant);

    virtual VariableStackBindings printasm(VariableStackBindings bindings) const;
};


#endif
