#include "type.hpp"

#include <exception>
#include <iostream>


// Type definition

void Type::print() const
{
    std::cout << getType() << " " << std::endl;
}

void Type::printXml() const
{}

VariableStackBindings Type::printAsm(VariableStackBindings bindings, unsigned& label_count) const
{
    (void)label_count;
    return bindings;
}

TypePtr Type::type()
{
    throw std::runtime_error("Error : does not have a type");
}

TypePtr Type::type(Type* type_ptr)
{
    (void) type_ptr;
    throw std::runtime_error("Error : cannot assign type");
}

TypePtr Type::type(TypePtr type_ptr)
{
    (void)type_ptr;
    throw std::runtime_error("Error : cannot assign type");
}

void Type::setSigned(bool _signed)
{
    (void)_signed;
    throw std::runtime_error("Error : cannot set sign");
}

void Type::setExtern(bool _extern)
{
    (void)_extern;
    throw std::runtime_error("Error : cannot set extern");
}

void Type::setStatic(bool _static)
{
    (void)_static;
    throw std::runtime_error("Error : cannot set static");
}

void Type::setConst(bool _const)
{
    (void)_const;
    throw std::runtime_error("Error : cannot set const");
}

void Type::setSize(int _size)
{
    (void)_size;
    throw std::runtime_error("Error : cannot set size");
}


// Pointer definition

Pointer::Pointer()
{}

std::string Pointer::getType() const
{
    return "pointer";
}


// Array definition

Array::Array()
{}

std::string Array::getType() const
{
    return "array";
}


// TypeContainer definition

TypeContainer::TypeContainer()
    : type_(nullptr), size_(32), extern_(false), static_(false), const_(false), signed_(true)
{}

std::string TypeContainer::getType() const
{
    return type_->getType();
}

TypePtr TypeContainer::type()
{
    return type_;
}

TypePtr TypeContainer::type(Type* type_ptr)
{
    TypePtr new_type_ptr(type_ptr);
    type_ = new_type_ptr;

    return type_;
}

TypePtr TypeContainer::type(TypePtr type_ptr)
{
    type_ = type_ptr;
    
    return type_;
}

void TypeContainer::setSigned(bool _signed)
{
    signed_ = _signed;
}

void TypeContainer::setExtern(bool _extern)
{
    extern_ = _extern;
}

void TypeContainer::setStatic(bool _static)
{
    static_ = _static;
}

void TypeContainer::setConst(bool _const)
{
    const_ = _const;
}

void TypeContainer::setSize(int size)
{
    size_ = size;
}


// Int definition

Int::Int()
{}

std::string Int::getType() const
{
    return "int";
}


// Void definition

Void::Void()
{}

std::string Void::getType() const
{
    return "void";
}


// Char definition

Char::Char()
{}

std::string Char::getType() const
{
    return "char";
}


// Float definition

Float::Float()
{}

std::string Float::getType() const
{
    return "float";
}
