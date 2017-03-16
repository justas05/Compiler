#ifndef TYPE_HPP
#define TYPE_HPP

#include "bindings.hpp"
#include "node.hpp"

#include <memory>
#include <string>

class Type;

typedef std::shared_ptr<Type> TypePtr;


class Type : public Node {
public:
    virtual void print() const;
    virtual void printXml() const;
    virtual VariableStackBindings printAsm(VariableStackBindings bindings) const;

    virtual std::string getType() const = 0;
};


class Specifier : public Type {
public:
    virtual std::string getType() const = 0;
};


class Pointer : public Type {
protected:
    TypePtr pointer_type_;

public:
    Pointer(Type* pointer_type);
    
    virtual std::string getType() const;
};


class Array : public Type {
protected:
    TypePtr array_type_;    
    unsigned size_;

public:
    Array(Type* array_type, unsigned size = 0);
    
    virtual std::string getType() const;
};


class Void : public Specifier {
public:
    Void();
    
    virtual std::string getType() const;
};


class Int : public Specifier {
public:
    Int();
    
    virtual std::string getType() const;
};


class Char : public Specifier {
public:
    Char();
    
    virtual std::string getType() const;
};


#endif
