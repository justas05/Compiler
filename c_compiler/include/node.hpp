#ifndef NODE_HPP
#define NODE_HPP

#include <cstdint>
#include <map>
#include <string>

struct VarLocation;
class Type;

typedef std::map<std::string, VarLocation> VariableStackBindings;


struct VarLocation {
    Type* type;
    int32_t stack_position;
};


class Node {
public:
    virtual ~Node() {}

    virtual void print() const = 0;
    virtual void printxml() const = 0;
    virtual VariableStackBindings printasm(VariableStackBindings bindings) const = 0;
};

    
#endif
