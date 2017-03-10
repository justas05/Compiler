#include "expression.hpp"
#include "bindings.hpp"

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

VariableStackBindings Identifier::printasm(VariableStackBindings bindings) const
{
    if(bindings.bindingExists(m_id)) {
	int32_t stack_position = bindings.getStackPosition(m_id);

	std::cout << "\tlw\t$2," << stack_position << "($fp)" << std::endl;
    } else
	std::cerr << "Can't find identifier '" << m_id << "' in current scope binding" << std::endl;
	
    return bindings;
}


// Constant definition

Constant::Constant(const int32_t& constant)
    : m_constant(constant)
{}

VariableStackBindings Constant::printasm(VariableStackBindings bindings) const
{
    std::cout << "\tli\t$2," << m_constant << std::endl;

    return bindings;
}

