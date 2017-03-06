#ifndef TYPE_HPP
#define TYPE_HPP

#include "ast.hpp"


class Type : public Node {
public:
    virtual void print() const;
    virtual void printxml() const;
    virtual void printasm() const;

    virtual std::string getType() const = 0;
};


class Specifier : Type {
public:
    virtual std::string getType() const = 0;
};


class Pointer : Type {
protected:
    Type* pointer_type;

public:
    Pointer(Type* _pointer_type);
    
    virtual std::string getType() const;
};


class Array : Type {
protected:
    int32_t size;
    Type* array_type;

public:
    Array(Type* _array_type, int32_t _size = 0);
    
    virtual std::string getType() const;
};


class Void : Specifier {
public:
    Void();
    
    virtual std::string getType() const;
};


class Int : Specifier {
public:
    Int();
    
    virtual std::string getType() const;
};


class Char : Specifier {
public:
    Char();
    
    virtual std::string getType() const;
};


#endif
