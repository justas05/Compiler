#include "type.hpp"
#include "bindings.hpp"

#include <iostream>


// Type definition

void Type::print() const
{
    std::cout << getType() << " " << std::endl;
}

void Type::printxml() const
{}

VariableStackBindings Type::printasm(VariableStackBindings bindings) const
{
    return bindings;
}


// Pointer definition

Pointer::Pointer(Type* _pointer_type) : pointer_type(_pointer_type)
{}

std::string Pointer::getType() const
{
    return "pointer " + pointer_type->getType();
}


// Array definition

Array::Array(Type* _array_type, int32_t _size) : size(_size), array_type(_array_type)
{}

std::string Array::getType() const
{
    return "array " + array_type->getType();
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
