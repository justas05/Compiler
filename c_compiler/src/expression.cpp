#include "expression.hpp"
#include "bindings.hpp"

#include <iostream>

// Expression definition

// There are no values to delete so it is just empty
Expression::~Expression()
{}

void Expression::print() const
{
    std::cerr << "This expression has not been implemented yet" << std::endl;
}

void Expression::printxml() const
{
    // Does nothing as I do not want it to appear in the xml output
}

int32_t Expression::getPostfixStackPosition(VariableStackBindings bindings) const
{
    std::cerr << "Error: Can't call 'getPostfixStackPosition(VariableStackBindings " <<
	"bindings)' on this type of expression" << std::endl;
    (void)bindings;
    return -1;
}


// OperationExpression definition

OperationExpression::OperationExpression(Expression* _lhs, Expression* _rhs)
    : lhs(_lhs), rhs(_rhs)
{}

// deletes the two member variables that have been initialized
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
    // the lhs is forced to have a stack position due to it being a function, array or other type of variable
    int32_t store_stack_position = lhs->getPostfixStackPosition(bindings);
    rhs->printasm(bindings);

    // we are assigning so we don't have to evaluate the lhs as it will be overwritten anyways
    std::cout << "\tsw\t$2," << store_stack_position << "($fp)" << std::endl;
    return bindings;
}


// Additive Expression definition

AdditiveExpression::AdditiveExpression(Expression* _lhs, const std::string& _operation, Expression* _rhs)
    : OperationExpression(_lhs, _rhs), operation(_operation)
{}

VariableStackBindings AdditiveExpression::printasm(VariableStackBindings bindings) const
{
    rhs->printasm(bindings);

    // move the rhs out of the way to be able to evaluate the lhs
    std::cout << "\tmove\t$3,$2" << std::endl;

    lhs->printasm(bindings);

    // then perform the right operation
    
    // currently using signed and sub because I only have signed numbers implemented
    // must update this as I add more types
    if(operation == "+")
	std::cout << "\tadd\t$2,$2,$3" << std::endl;
    else if(operation == "-")
	std::cout << "\tsub\t$2,$2,$3" << std::endl;
    else
	std::cerr << "Don't recognize symbol: '" << operation << "'" << std::endl;

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
    // constant only has to load to $2 because the other expression will take care of the rest
    std::cout << "\tli\t$2," << m_constant << std::endl;

    return bindings;
}

