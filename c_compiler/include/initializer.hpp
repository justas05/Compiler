#ifndef INITIALIZER_HPP
#define INITIALIZER_HPP

#include "ast.hpp"


class Initializer : public Node {
public:
    Initializer() {}

    virtual void print() const {}
    virtual void printxml() const {}
    virtual void printasm() const {}
};


class Integer : public Initializer {
public:
    Integer() : Initializer() {}
};


class StringLiteral : public Initializer {
public:
    StringLiteral() : Initializer() {}
};


#endif
