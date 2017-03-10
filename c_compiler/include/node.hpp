#ifndef NODE_HPP
#define NODE_HPP

#include <cstdint>
#include <map>
#include <string>

class Type;
class VariableStackBindings; 


// base node class
class Node
{
public:
    virtual ~Node() {}

    virtual void print() const = 0;
    virtual void printxml() const = 0;
    virtual VariableStackBindings printasm(VariableStackBindings bindings) const = 0;
};

    
#endif
