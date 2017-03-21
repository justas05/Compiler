#ifndef TYPE_HPP
#define TYPE_HPP

#include "bindings.hpp"
#include "node.hpp"

#include <memory>
#include <string>

class Type;
typedef std::shared_ptr<Type> TypePtr;

class Type : public Node
{
public:
    virtual void print() const;
    virtual void printXml() const;
    virtual VariableStackBindings printAsm(VariableStackBindings bindings, unsigned& label_count) const;

    virtual std::string getType() const = 0;

    virtual TypePtr type();    
    virtual TypePtr type(Type* type_ptr);
    virtual TypePtr type(TypePtr type_ptr);
    virtual void setSigned(bool _signed);
    virtual void setExtern(bool _extern);
    virtual void setStatic(bool _static);
    virtual void setConst(bool _const);
    virtual void setSize(int size);    
};

class Array : public Type
{
public:
    Array();
    virtual std::string getType() const;
};

class Pointer : public Type
{
public:
    Pointer();
    virtual std::string getType() const;
};

class TypeContainer : public Type
{
protected:
    TypePtr type_;
    int size_;
    bool extern_;
    bool static_;
    bool const_;
    bool signed_;

public:
    TypeContainer();

    virtual std::string getType() const;
    virtual TypePtr type();    
    virtual TypePtr type(Type* type_ptr);
    virtual TypePtr type(TypePtr type_ptr);
    virtual void setSigned(bool _signed);
    virtual void setExtern(bool _extern);
    virtual void setStatic(bool _static);
    virtual void setConst(bool _const);
    virtual void setSize(int size);
};

class Specifier : public Type
{
public:
    virtual std::string getType() const = 0;
};

class Int : public Specifier
{
public:
    Int();
    
    virtual std::string getType() const;
};

class Void : public Specifier
{
public:
    Void();
    
    virtual std::string getType() const;    
};

class Char : public Specifier
{
public:
    Char();
    
    virtual std::string getType() const;    
};

class Float : public Specifier
{
public:
    Float();
    
    virtual std::string getType() const;    
};

#endif
