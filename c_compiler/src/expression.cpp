#include "expression.hpp"

#include <iostream>
#include <vector>

// Expression definition

void Expression::print() const
{
    std::cerr << "This expression has not been implemented yet" << std::endl;
}

void Expression::printXml() const
{
    // Does nothing as I do not want it to appear in the xml output
}

void Expression::countArguments(unsigned int &argument_count) const
{
    (void)argument_count;
}

int Expression::postfixStackPosition(VariableStackBindings bindings) const
{
    std::cerr << "Error : Can't call 'getPostfixStackPosition(VariableStackBindings " <<
	"bindings)' on this type of expression" << std::endl;
    (void)bindings;
    return -1;
}

void Expression::setPostfixExpression(Expression *postfix_expression)
{
    (void)postfix_expression;
}

std::string Expression::id() const
{
    return "";
}

void Expression::linkExpression(Expression *next_expression)
{
    ExpressionPtr expression_ptr(next_expression);
    next_expression_ = expression_ptr;
}

ExpressionPtr Expression::nextExpression() const
{
    return next_expression_;
}


// OperationExpression definition

OperationExpression::OperationExpression(Expression* lhs, Expression* rhs)
    : lhs_(lhs), rhs_(rhs)
{}


// PostfixExpression definition

PostfixExpression::PostfixExpression()
{}

VariableStackBindings PostfixExpression::printAsm(VariableStackBindings bindings, unsigned& label_count) const
{
    return bindings;
}


// PostfixArrayElement

PostfixArrayElement::PostfixArrayElement()
{}

VariableStackBindings PostfixArrayElement::printAsm(VariableStackBindings bindings, unsigned& label_count) const
{
    return bindings;
}


// PostfixFunctionCall

PostfixFunctionCall::PostfixFunctionCall(Expression* argument_expression_list)
    : argument_expression_list_(argument_expression_list)
{}

VariableStackBindings PostfixFunctionCall::printAsm(VariableStackBindings bindings, unsigned& label_count) const
{
    std::vector<ExpressionPtr> argument_vector;
    ExpressionPtr current_argument = argument_expression_list_;
    unsigned argument_counter = 0;

    while(current_argument != nullptr) {
	argument_vector.push_back(current_argument);
	current_argument = current_argument->nextExpression();
    }

    for(auto itr = argument_vector.rbegin(); itr != argument_vector.rend(); ++itr) {
	(*itr)->printAsm(bindings, label_count);

	if(argument_counter < 4)
	    std::cout << "\tmove\t$" << 4+argument_counter << ",$2\n";
	else
	    std::cout << "\tsw\t$2," << 4*argument_counter << "($fp)\n";

	argument_counter++;
    }

    std::cout << "\tjal\t" << postfix_expression_->id() << "\n\tnop\n";
    
    return bindings;
}

void PostfixFunctionCall::countArguments(unsigned int &argument_count) const
{
    ExpressionPtr current_argument = argument_expression_list_;

    argument_count = 0;
    
    while(current_argument != nullptr) {
	argument_count++;
	current_argument = current_argument->nextExpression();
    }
}

void PostfixFunctionCall::setPostfixExpression(Expression* postfix_expression)
{
    ExpressionPtr expression_ptr(postfix_expression);
    postfix_expression_ = expression_ptr;
}


// UnaryExpression definition

UnaryExpression::UnaryExpression()
{}

VariableStackBindings UnaryExpression::printAsm(VariableStackBindings bindings, unsigned& label_count) const
{
    return bindings;
}


// CastExpression definition

CastExpression::CastExpression(Type* type, Expression* expression)
    : type_(type), expression_(expression)
{}

VariableStackBindings CastExpression::printAsm(VariableStackBindings bindings, unsigned& label_count) const
{
    return bindings;
}


// Additive Expression definition

AdditiveExpression::AdditiveExpression(Expression* lhs, const std::string& _operator, Expression* rhs)
    : OperationExpression(lhs, rhs), operator_(_operator)
{}

VariableStackBindings AdditiveExpression::printAsm(VariableStackBindings bindings, unsigned& label_count) const
{
    // I can just evaluate the lhs with the same entry stack position
    lhs_->printAsm(bindings, label_count);

    // store this stack position
    int lhs_stack_position = bindings.currentExpressionStackPosition();

    // now have to increase the expression stack position for the rhs
    bindings.nextExpressionStackPosition();
    rhs_->printAsm(bindings, label_count);

    // now I have them evaluated at two positions in the stack and can load both into registers
    // $2 and $3

    std::cout << "\tlw\t$2," << lhs_stack_position << "($fp)" << std::endl;
    std::cout << "\tlw\t$3," << bindings.currentExpressionStackPosition() << "($fp)" << std::endl;
    
    // TODO currently using signed and sub because I only have signed numbers implemented
    // must update this as I add more types
    if(operator_ == "+")
	std::cout << "\tadd\t$2,$2,$3" << std::endl;
    else if(operator_ == "-")
	std::cout << "\tsub\t$2,$2,$3" << std::endl;
    else
	std::cerr << "Don't recognize symbol: '" << operator_ << "'" << std::endl;

    // now I have to store it back into the original stack position
    std::cout << "\tsw\t$2," << lhs_stack_position << "($fp)" << std::endl;

    return bindings;
}


// Multiplicative Expression definition


MultiplicativeExpression::MultiplicativeExpression(Expression* lhs, const std::string& _operator, Expression* rhs)
    : OperationExpression(lhs, rhs), operator_(_operator)
{}

VariableStackBindings MultiplicativeExpression::printAsm(VariableStackBindings bindings, unsigned& label_count) const
{
    // I can just evaluate lhs without increasing stack count
    lhs_->printAsm(bindings, label_count);

    // store current stack position
    int lhs_stack_position = bindings.currentExpressionStackPosition();

    // increase stack position to store next result in
    bindings.nextExpressionStackPosition();
    rhs_->printAsm(bindings, label_count);

    std::cout << "\tlw\t$2," << lhs_stack_position << "($fp)" << std::endl;
    std::cout << "\tlw\t$3," << bindings.currentExpressionStackPosition() << "($fp)" << std::endl;

    // then perform the right operation
    if(operator_ == "*")
	std::cout << "\tmul\t$2,$2,$3" << std::endl;
    else if(operator_ == "/" || operator_ == "%") {
	std::cout << "\tdiv\t$2,$3" << std::endl;
	if(operator_ == "/")
	    std::cout << "\tmflo\t$2" << std::endl;
	else
	    std::cout << "\tmfhi\t$2" << std::endl;
    } else
	std::cerr << "Don't recognize symbol '" << operator_ << "'" << std::endl;

    // finally store result back into the stack position
    std::cout << "\tsw\t$2," << lhs_stack_position << "($fp)" << std::endl;    

    return bindings;
}


// ShiftExpression definition

ShiftExpression::ShiftExpression(Expression* lhs, Expression* rhs)
    : OperationExpression(lhs, rhs)
{}

VariableStackBindings ShiftExpression::printAsm(VariableStackBindings bindings, unsigned& label_count) const
{
    return bindings;
}


// RelationalExpression definition

RelationalExpression::RelationalExpression(Expression* lhs, Expression* rhs)
    : OperationExpression(lhs, rhs)
{}

VariableStackBindings RelationalExpression::printAsm(VariableStackBindings bindings, unsigned& label_count) const
{
    return bindings;
}


// EqualityExpression definition

EqualityExpression::EqualityExpression(Expression* lhs, const std::string& _operator, Expression* rhs)
    : OperationExpression(lhs, rhs), operator_(_operator)
{}

VariableStackBindings EqualityExpression::printAsm(VariableStackBindings bindings, unsigned& label_count) const
{
    (void)label_count;

    // I can just evaluate lhs without increasing stack count
    lhs_->printAsm(bindings, label_count);

    // store current stack position
    int lhs_stack_position = bindings.currentExpressionStackPosition();

    // increase stack position to store next result in
    bindings.nextExpressionStackPosition();
    rhs_->printAsm(bindings, label_count);

    std::cout << "\txor\t$2,$2,$3\n";
    
    return bindings;
}


// AndExpression definition

AndExpression::AndExpression(Expression* lhs, Expression* rhs)
    : OperationExpression(lhs, rhs)
{}

VariableStackBindings AndExpression::printAsm(VariableStackBindings bindings, unsigned& label_count) const
{
    return bindings;
}


// ExclusiveOrExpression definition

ExclusiveOrExpression::ExclusiveOrExpression(Expression* lhs, Expression* rhs)
    : OperationExpression(lhs, rhs)
{}

VariableStackBindings ExclusiveOrExpression::printAsm(VariableStackBindings bindings, unsigned& label_count) const
{
    return bindings;
}


// InclusiveOrExpression definition

InclusiveOrExpression::InclusiveOrExpression(Expression* lhs, Expression* rhs)
    : OperationExpression(lhs, rhs)
{}

VariableStackBindings InclusiveOrExpression::printAsm(VariableStackBindings bindings, unsigned& label_count) const
{
    return bindings;
}


// LogicalAndExpression definition

LogicalAndExpression::LogicalAndExpression(Expression* lhs, Expression* rhs)
    : OperationExpression(lhs, rhs)
{}

VariableStackBindings LogicalAndExpression::printAsm(VariableStackBindings bindings, unsigned& label_count) const
{
    return bindings;
}


// LogicalOrExpression definition

LogicalOrExpression::LogicalOrExpression(Expression* lhs, Expression* rhs)
    : OperationExpression(lhs, rhs)
{}

VariableStackBindings LogicalOrExpression::printAsm(VariableStackBindings bindings, unsigned& label_count) const
{
    return bindings;
}


// ConditionalExpression definition

ConditionalExpression::ConditionalExpression(Expression* logical_or,
					     Expression* expression,
					     Expression* conditional_expression)
    : logical_or_(logical_or), expression_(expression),
      conditional_expression_(conditional_expression)
{}

VariableStackBindings ConditionalExpression::printAsm(VariableStackBindings bindings, unsigned& label_count) const
{
    return bindings;
}


// Assignment Expression definition

AssignmentExpression::AssignmentExpression(Expression* lhs, Expression* rhs)
    : OperationExpression(lhs, rhs)
{}

VariableStackBindings AssignmentExpression::printAsm(VariableStackBindings bindings, unsigned& label_count) const
{
    // TODO add stack and store results in there, also for addition and multiplication.

    // get the current location of lhs in the stack so that I can store result there
    int store_stack_position = lhs_->postfixStackPosition(bindings);

    // get the current available stack position
    int expression_stack_position = bindings.currentExpressionStackPosition();

    // evaluate rhs and get the result back at the stack position I assigned
    // don't have to change the stack position as there is no lhs to evaluate
    rhs_->printAsm(bindings, label_count);

    // now the result of the rhs will be in that stack position, so we can load it into $2
    std::cout << "\tlw\t$2," << expression_stack_position << "($fp)" << std::endl;

    // we are assigning so we don't have to evaluate the lhs as it will be overwritten anyways
    std::cout << "\tsw\t$2," << store_stack_position << "($fp)" << std::endl;
    return bindings;
}


// Identifier definition

Identifier::Identifier(const std::string& id)
    : id_(id)
{}

VariableStackBindings Identifier::printAsm(VariableStackBindings bindings, unsigned& label_count) const
{
    (void)label_count;
    if(bindings.bindingExists(id_)) {
	std::cout << "\tlw\t$2," << bindings.stackPosition(id_) << "($fp)" << std::endl;
    } else
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

std::string Identifier::id() const
{
    return id_;
}


// Constant definition

Constant::Constant(const int32_t& constant)
    : constant_(constant)
{}

VariableStackBindings Constant::printAsm(VariableStackBindings bindings, unsigned& label_count) const
{
    (void)label_count;
    // constant only has to load to $2 because the other expression will take care of the rest
    std::cout << "\tli\t$2," << constant_ << std::endl;
    std::cout << "\tsw\t$2," << bindings.currentExpressionStackPosition() << "($fp)" << std::endl;
    return bindings;
}

