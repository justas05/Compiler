#ifndef TYPE_HPP
#define TYPE_HPP

#include "ast.hpp"


class Type : public Node {
public:
    Type();

    virtual void print() const {}
    virtual void printxml() const {}
    virtual void printasm() const {}
};


#endif
