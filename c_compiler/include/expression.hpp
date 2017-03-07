#ifndef AST_EXPRESSION_HPP
#define AST_EXPRESSION_HPP

#include "ast.hpp"

class Expression : public Node {
public:
    virtual void printasm() const = 0;
    
    virtual void print() const;
    virtual void printxml() const;
};


class Identifier : public Expression {
private:
    std::string m_id;
public:
    Identifier(const std::string& id);

    virtual void printasm() const;
};


class Constant : public Expression {
private:
    int32_t m_constant;
public:
    Constant(const int32_t& constant);

    virtual void printasm() const;
};


#endif
