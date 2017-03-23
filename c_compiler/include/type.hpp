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
    virtual void print() const = 0;
    virtual void printXml() const = 0;
    virtual VariableStackBindings printAsm(VariableStackBindings bindings, unsigned &label_count) const = 0;
    
    virtual TypePtr type() = 0;
    virtual TypePtr type(Type *type_ptr) = 0;
    virtual TypePtr type(TypePtr type_ptr) = 0;
    
    virtual void setSigned(bool _signed);
    virtual void setExtern(bool _extern);
    virtual void setStatic(bool _static);
    virtual void setConst(bool _const);
    virtual void setSize(int size);    
};

class Array : public Type
{
private:
    int size_;
    TypePtr type_;
public:
    Array(const int &size, TypePtr type_);

    virtual void print() const;
    virtual void printXml() const;
    virtual VariableStackBindings printAsm(VariableStackBindings bindings, unsigned &label_count) const;
    
    virtual TypePtr type();
    virtual TypePtr type(Type *type_ptr);
    virtual TypePtr type(TypePtr type_ptr);
};

class Pointer : public Type
{
private:
    TypePtr type_;
public:
    Pointer();

    virtual void print() const;
    virtual void printXml() const;
    virtual VariableStackBindings printAsm(VariableStackBindings bindings, unsigned &label_count) const;
    
    virtual TypePtr type();
    virtual TypePtr type(Type *type_ptr);
    virtual TypePtr type(TypePtr type_ptr);
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

    virtual void print() const;
    virtual void printXml() const;
    virtual VariableStackBindings printAsm(VariableStackBindings bindings, unsigned &label_count) const;
    
    virtual TypePtr type();    
    virtual TypePtr type(Type *type_ptr);
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
    virtual void print() const = 0;
    virtual void printXml() const = 0;
    virtual VariableStackBindings printAsm(VariableStackBindings bindings, unsigned &label_count) const = 0;
    
    virtual TypePtr type();
    virtual TypePtr type(Type *type_ptr);
    virtual TypePtr type(TypePtr type_ptr);
};

class Int : public Specifier
{
public:
    Int();

    virtual void print() const;
    virtual void printXml() const;
    virtual VariableStackBindings printAsm(VariableStackBindings bindings, unsigned &label_count) const;
};

class Void : public Specifier
{
public:
    Void();

    virtual void print() const;
    virtual void printXml() const;
    virtual VariableStackBindings printAsm(VariableStackBindings bindings, unsigned &label_count) const;
};

class Char : public Specifier
{
public:
    Char();

    virtual void print() const;
    virtual void printXml() const;
    virtual VariableStackBindings printAsm(VariableStackBindings bindings, unsigned &label_count) const;
};

class Float : public Specifier
{
public:
    Float();

    virtual void print() const;
    virtual void printXml() const;
    virtual VariableStackBindings printAsm(VariableStackBindings bindings, unsigned &label_count) const;
};

#endif
