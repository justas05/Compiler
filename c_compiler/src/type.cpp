#include "type.hpp"

#include <iostream>


// Type definition

void Type::print() const
{
    std::cout << getType() << " " << std::endl;
}

void Type::printXml() const
{}

VariableStackBindings Type::printAsm(VariableStackBindings bindings) const
{
    return bindings;
}


// Pointer definition

Pointer::Pointer(Type* pointer_type) : pointer_type_(pointer_type)
{}

std::string Pointer::getType() const
{
    return "pointer " + pointer_type_->getType();
}


// Array definition

Array::Array(Type* array_type, unsigned size)
    : array_type_(array_type), size_(size)
{}

std::string Array::getType() const
{
    return "array " + array_type_->getType();
}


// Void definition

Void::Void()
{}

std::string Void::getType() const
{
    return "void";
}


// Int defintion

Int::Int()
{}

std::string Int::getType() const
{
    return "int";
}


// Char definition

Char::Char()
{}

std::string Char::getType() const
{
    return "char";
}
