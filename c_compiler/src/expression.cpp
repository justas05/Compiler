#include "expression.hpp"

#include <iostream>

// Expression definition

void Expression::print() const
{
    std::cerr << "This expression has not been implemented yet" << std::endl;
}

void Expression::printXml() const
{
    // Does nothing as I do not want it to appear in the xml output
}

int Expression::postfixStackPosition(VariableStackBindings bindings) const
{
    std::cerr << "Error : Can't call 'getPostfixStackPosition(VariableStackBindings " <<
	"bindings)' on this type of expression" << std::endl;
    (void)bindings;
    return -1;
}


// OperationExpression definition

OperationExpression::OperationExpression(Expression* lhs, Expression* rhs)
    : lhs_(lhs), rhs_(rhs)
{}


// Assignment Expression definition

AssignmentExpression::AssignmentExpression(Expression* lhs, Expression* rhs)
    : OperationExpression(lhs, rhs)
{}

VariableStackBindings AssignmentExpression::printAsm(VariableStackBindings bindings) const
{
    // TODO
    // the lhs is forced to have a stack position due to it being a function, array or other type of variable
    /*unsigned current_stack = bindings.currentRegister();
    // std::cout << "Current Register: " << current_reg << std::endl;
    bindings.increaseRegister();
    
    int store_stack_position = lhs->getPostfixStackPosition(bindings);
    
    rhs->printAsm(bindings);

    // we are assigning so we don't have to evaluate the lhs as it will be overwritten anyways
    std::cout << "\tsw\t$" << current_reg << "," << store_stack_position
    << "($fp)" << std::endl; */
    return bindings;
}


// Additive Expression definition

AdditiveExpression::AdditiveExpression(Expression* lhs, const std::string& operation, Expression* rhs)
    : OperationExpression(lhs, rhs), operation_(operation)
{}

VariableStackBindings AdditiveExpression::printAsm(VariableStackBindings bindings) const
{
    lhs_->printAsm(bindings);

    // move the rhs out of the way to be able to evaluate the lhs
    std::cout << "\tmove\t$3,$2" << std::endl;

    rhs_->printAsm(bindings);

    // then perform the right operation
    
    // currently using signed and sub because I only have signed numbers implemented
    // must update this as I add more types
    if(operation_ == "+")
	std::cout << "\tadd\t$2,$3,$2" << std::endl;
    else if(operation_ == "-")
	std::cout << "\tsub\t$2,$3,$2" << std::endl;
    else
	std::cerr << "Don't recognize symbol: '" << operation_ << "'" << std::endl;

    return bindings;
}


// Multiplicative Expression definition


MultiplicativeExpression::MultiplicativeExpression(Expression* lhs, const std::string& operation, Expression* rhs)
    : OperationExpression(lhs, rhs), operation_(operation)
{}

VariableStackBindings MultiplicativeExpression::printAsm(VariableStackBindings bindings) const
{
    lhs_->printAsm(bindings);

    std::cout << "\tmove\t$3,$2" << std::endl;

    rhs_->printAsm(bindings);

    // then perform the right operation
    if(operation_ == "*")
	std::cout << "\tmul\t$2,$3,$2" << std::endl;
    else if(operation_ == "/" || operation_ == "%") {
	std::cout << "\tdiv\t$3,$2" << std::endl;
	if(operation_ == "/")
	    std::cout << "\tmflo\t$2" << std::endl;
	else
	    std::cout << "\tmfhi\t$2" << std::endl;
    } else
	std::cerr << "Don't recognize symbol '" << operation_ << "'" << std::endl;

    return bindings;
}


// Identifier definition

Identifier::Identifier(const std::string& id)
    : id_(id)
{}

VariableStackBindings Identifier::printAsm(VariableStackBindings bindings) const
{
    if(bindings.bindingExists(id_))
	std::cout << "\tlw\t$2," << bindings.stackPosition(id_) << "($fp)" << std::endl;
    else
	std::cerr << "Can't find identifier '" << id_ << "' in current scope binding" << std::endl;
	
    return bindings;
}

int Identifier::postfixStackPosition(VariableStackBindings bindings) const
{
    if(bindings.bindingExists(id_)) {
	return bindings.stackPosition(id_);
    }

    return -1;
}


// Constant definition

Constant::Constant(const int32_t& constant)
    : m_constant(constant)
{}

VariableStackBindings Constant::printAsm(VariableStackBindings bindings) const
{
    // constant only has to load to $2 because the other expression will take care of the rest
    std::cout << "\tli\t$2," << m_constant << std::endl;

    return bindings;
}

