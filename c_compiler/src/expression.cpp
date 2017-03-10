#include "expression.hpp"
#include "bindings.hpp"

#include <iostream>

// Expression definition

Expression::~Expression()
{}

void Expression::print() const
{}

void Expression::printxml() const
{}

int32_t Expression::getPostfixStackPosition(VariableStackBindings bindings) const
{
    return -1;
}


// OperationExpression definition

OperationExpression::OperationExpression(Expression* _lhs, Expression* _rhs)
    : lhs(_lhs), rhs(_rhs)
{}

OperationExpression::~OperationExpression()
{
    delete lhs;
    delete rhs;
}


// Assignment Expression definition

AssignmentExpression::AssignmentExpression(Expression* _lhs, Expression* _rhs)
    : OperationExpression(_lhs, _rhs)
{}

VariableStackBindings AssignmentExpression::printasm(VariableStackBindings bindings) const
{
    int32_t store_stack_position = lhs->getPostfixStackPosition(bindings);
    rhs->printasm(bindings);

    std::cout << "\tsw\t$2," << store_stack_position << "($fp)" << std::endl;
								   
    return bindings;
}


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

int32_t Identifier::getPostfixStackPosition(VariableStackBindings bindings) const
{
    if(bindings.bindingExists(m_id)) {
	return bindings.getStackPosition(m_id);
    }

    return -1;
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

