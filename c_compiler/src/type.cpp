#include "type.hpp"

#include <cstdio>
#include <exception>


// Type definition

void Type::setSigned(bool)
{
    throw std::runtime_error("Error : cannot set sign");
}

void Type::setExtern(bool)
{
    throw std::runtime_error("Error : cannot set extern");
}

void Type::setStatic(bool)
{
    throw std::runtime_error("Error : cannot set static");
}

void Type::setConst(bool)
{
    throw std::runtime_error("Error : cannot set const");
}

void Type::setSize(int)
{
    throw std::runtime_error("Error : cannot set size");
}


// Array definition

Array::Array(const int &size, TypePtr type)
    : size_(size), type_(type)
{}

void Array::print() const
{
    printf("Array\n");
}

void Array::printXml() const
{}

VariableStackBindings Array::printAsm(VariableStackBindings bindings, unsigned &label_count) const
{
    return bindings;
}

TypePtr Array::type()
{
    return type_;
}

TypePtr Array::type(Type *type_ptr)
{
    TypePtr sh_type_ptr(type_ptr);
    type_ = sh_type_ptr;
    return type_;
}

TypePtr Array::type(TypePtr type_ptr)
{
    type_ = type_ptr;
    return type_;
}


// Pointer definition

Pointer::Pointer()
{}

void Pointer::print() const
{
    printf("Pointer\n");
}

void Pointer::printXml() const
{}

VariableStackBindings Pointer::printAsm(VariableStackBindings bindings, unsigned &label_count) const
{
    return bindings;
}

TypePtr Pointer::type()
{
    return type_;
}

TypePtr Pointer::type(Type *type_ptr)
{
    TypePtr sh_type_ptr(type_ptr);
    type_ = sh_type_ptr;
    return type_;
}

TypePtr Pointer::type(TypePtr type_ptr)
{
    type_ = type_ptr;
    return type_;
}




// TypeContainer definition

TypeContainer::TypeContainer()
    : type_(nullptr), size_(32), extern_(false), static_(false), const_(false), signed_(true)
{}

void TypeContainer::print() const
{
    printf("TypeContainer : ");
    type_->print();
}

void TypeContainer::printXml() const
{}

VariableStackBindings TypeContainer::printAsm(VariableStackBindings bindings, unsigned &label_count) const
{
    return bindings;
}

TypePtr TypeContainer::type()
{
    return type_;
}

TypePtr TypeContainer::type(Type *type_ptr)
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


// Specifier definitions

TypePtr Specifier::type()
{
    throw std::runtime_error("Error : Cannot get type");
}

TypePtr Specifier::type(Type *)
{
    throw std::runtime_error("Error : Cannot get type");
}

TypePtr Specifier::type(TypePtr)
{
    throw std::runtime_error("Error : Cannot get type");
}


// Int definition

Int::Int()
{}

void Int::print() const
{
    printf("Int\n");
}

void Int::printXml() const
{}

VariableStackBindings Int::printAsm(VariableStackBindings bindings, unsigned &label_count) const
{
    return bindings;
}


// Void definition

Void::Void()
{}

void Void::print() const
{
    printf("Void\n");
}

void Void::printXml() const
{}

VariableStackBindings Void::printAsm(VariableStackBindings bindings, unsigned &label_count) const
{
    return bindings;
}


// Char definition

Char::Char()
{}

void Char::print() const
{
    printf("Char\n");
}

void Char::printXml() const
{}

VariableStackBindings Char::printAsm(VariableStackBindings bindings, unsigned &label_count) const
{
    return bindings;
}


// Float definition

Float::Float()
{}

void Float::print() const
{
    printf("Float\n");
}

void Float::printXml() const
{}

VariableStackBindings Float::printAsm(VariableStackBindings bindings, unsigned &label_count) const
{
    return bindings;
}
