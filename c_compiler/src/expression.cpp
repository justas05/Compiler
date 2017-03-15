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
    // TODO add stack and store results in there, also for addition and multiplication.

    // get the current location of lhs in the stack so that I can store result there
    int store_stack_position = lhs_->postfixStackPosition(bindings);

    // get the current available stack position
    int expression_stack_position = bindings.currentExpressionStackPosition();

    // evaluate rhs and get the result back at the stack position I assigned
    // don't have to change the stack position as there is no lhs to evaluate
    rhs_->printAsm(bindings);

    // now the result of the rhs will be in that stack position, so we can load it into $2
    std::cout << "\tlw\t$2," << expression_stack_position << "($fp)" << std::endl;

    // we are assigning so we don't have to evaluate the lhs as it will be overwritten anyways
    std::cout << "\tsw\t$2," << store_stack_position << "($fp)" << std::endl;
    return bindings;
}


// Additive Expression definition

AdditiveExpression::AdditiveExpression(Expression* lhs, const std::string& operation, Expression* rhs)
    : OperationExpression(lhs, rhs), operation_(operation)
{}

VariableStackBindings AdditiveExpression::printAsm(VariableStackBindings bindings) const
{
    // I can just evaluate the lhs with the same entry stack position
    lhs_->printAsm(bindings);

    // store this stack position
    int lhs_stack_position = bindings.currentExpressionStackPosition();

    // now have to increase the expression stack position for the rhs
    bindings.nextExpressionStackPosition();
    rhs_->printAsm(bindings);

    // now I have them evaluated at two positions in the stack and can load both into registers
    // $2 and $3

    std::cout << "\tlw\t$2," << lhs_stack_position << "($fp)" << std::endl;
    std::cout << "\tlw\t$3," << bindings.currentExpressionStackPosition() << "($fp)" << std::endl;
    
    // TODO currently using signed and sub because I only have signed numbers implemented
    // must update this as I add more types
    if(operation_ == "+")
	std::cout << "\tadd\t$2,$2,$3" << std::endl;
    else if(operation_ == "-")
	std::cout << "\tsub\t$2,$2,$3" << std::endl;
    else
	std::cerr << "Don't recognize symbol: '" << operation_ << "'" << std::endl;

    // now I have to store it back into the original stack position
    std::cout << "\tsw\t$2," << lhs_stack_position << "($fp)" << std::endl;

    return bindings;
}


// Multiplicative Expression definition


MultiplicativeExpression::MultiplicativeExpression(Expression* lhs, const std::string& operation, Expression* rhs)
    : OperationExpression(lhs, rhs), operation_(operation)
{}

VariableStackBindings MultiplicativeExpression::printAsm(VariableStackBindings bindings) const
{
    // I can just evaluate lhs without increasing stack count
    lhs_->printAsm(bindings);

    // store current stack position
    int lhs_stack_position = bindings.currentExpressionStackPosition();

    // increase stack position to store next result in
    bindings.nextExpressionStackPosition();
    rhs_->printAsm(bindings);

    std::cout << "\tlw\t$2," << lhs_stack_position << "($fp)" << std::endl;
    std::cout << "\tlw\t$3," << bindings.currentExpressionStackPosition() << "($fp)" << std::endl;

    // then perform the right operation
    if(operation_ == "*")
	std::cout << "\tmul\t$2,$2,$3" << std::endl;
    else if(operation_ == "/" || operation_ == "%") {
	std::cout << "\tdiv\t$2,$3" << std::endl;
	if(operation_ == "/")
	    std::cout << "\tmflo\t$2" << std::endl;
	else
	    std::cout << "\tmfhi\t$2" << std::endl;
    } else
	std::cerr << "Don't recognize symbol '" << operation_ << "'" << std::endl;

    // finally store result back into the stack position
    std::cout << "\tsw\t$2," << lhs_stack_position << "($fp)" << std::endl;    

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

    std::cout << "\tsw\t$2," << bindings.currentExpressionStackPosition() << "($fp)" << std::endl;
	
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
    : constant_(constant)
{}

VariableStackBindings Constant::printAsm(VariableStackBindings bindings) const
{
    // constant only has to load to $2 because the other expression will take care of the rest
    std::cout << "\tli\t$2," << constant_ << std::endl;
    std::cout << "\tsw\t$2," << bindings.currentExpressionStackPosition() << "($fp)" << std::endl;
    return bindings;
}

