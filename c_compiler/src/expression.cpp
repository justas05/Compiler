#include "expression.hpp"

#include <iostream>

// Expression definition

void Expression::print() const
{}

void Expression::printxml() const
{}


// Identifier definition

Identifier::Identifier(const std::string& id)
    : m_id(id)
{}

void Identifier::printasm(VariableStackBindings bindings) const
{}


// Constant definition

Constant::Constant(const int32_t& constant)
    : m_constant(constant)
{}

void Constant::printasm(VariableStackBindings bindings) const
{
    std::cout << "\tli\t$2," << m_constant << std::endl;
}

