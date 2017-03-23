#include "expression.hpp"

#include <cstdio>
#include <exception>
#include <vector>

// Expression definition

int Expression::constantFold() const
{
    throw std::runtime_error("Error : Cannot constant fold this expression");
}

void Expression::print() const
{}

void Expression::printXml() const
{}

void Expression::countArguments(unsigned& argument_count) const
{
    // by default don't do anything to the count
    (void)argument_count;
}

void Expression::expressionDepth(unsigned &) const
{}

int Expression::postfixStackPosition(VariableStackBindings bindings) const
{
    (void)bindings;
    throw std::runtime_error("Error : Can't call postfixStackExpression() on this type");
}

void Expression::setPostfixExpression(Expression *postfix_expression)
{
    // do nothing if expression isn't a postfix expression
    (void)postfix_expression;
}

std::string Expression::id() const
{
    // by default return empty id, which cannot be valid.
    return "";
}

ExpressionPtr Expression::getLhs() const
{
    throw std::runtime_error("Error : Cannot get lhs");
}

ExpressionPtr Expression::getRhs() const
{
    throw std::runtime_error("Error : Cannot get rhs");
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

OperationExpression::OperationExpression(ExpressionPtr lhs, Expression* rhs)
    : lhs_(lhs), rhs_(rhs)
{}

int OperationExpression::constantFold() const
{
    throw std::runtime_error("Error : Cannot constant fold expression\n");
}

void OperationExpression::expressionDepth(unsigned& depth_count) const
{
    unsigned lhs_depth_count = depth_count;
    unsigned rhs_depth_count = depth_count+1;

    lhs_->expressionDepth(lhs_depth_count);
    rhs_->expressionDepth(rhs_depth_count);

    if(lhs_depth_count > rhs_depth_count)
	depth_count = lhs_depth_count;
    else
	depth_count = rhs_depth_count;
}

ExpressionPtr OperationExpression::getLhs() const
{
    return lhs_;
}

ExpressionPtr OperationExpression::getRhs() const
{
    return rhs_;
}

void OperationExpression::evaluateExpression(VariableStackBindings bindings, unsigned& label_count) const
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
    printf("\tlw\t$2,%d($fp)\n\tlw\t$3,%d($fp)\n",
	   lhs_stack_position, bindings.currentExpressionStackPosition());
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

    while(current_argument != nullptr)
    {
	argument_vector.push_back(current_argument);
	current_argument = current_argument->nextExpression();
    }

    for(auto itr = argument_vector.rbegin(); itr != argument_vector.rend(); ++itr)
    {
	(*itr)->printAsm(bindings, label_count);

	if(argument_counter < 4)
	    printf("\tmove\t$%d,$2\n", 4+argument_counter);
	else
	    printf("\tsw\t$2,%d($fp)\n", 4*argument_counter);

	argument_counter++;
    }

    printf("\tjal\t%s\n\tnop\n\tsw\t$2,%d($fp)\n",
	   postfix_expression_->id().c_str(), bindings.currentExpressionStackPosition());
    return bindings;
}

void PostfixFunctionCall::countArguments(unsigned int &argument_count) const
{
    ExpressionPtr current_argument = argument_expression_list_;

    argument_count = 0;
    
    while(current_argument != nullptr)
    {
	argument_count++;
	current_argument = current_argument->nextExpression();
    }
}

void PostfixFunctionCall::setPostfixExpression(Expression* postfix_expression)
{
    ExpressionPtr expression_ptr(postfix_expression);
    postfix_expression_ = expression_ptr;
}


// Post increment and decrement definition

PostfixPostIncDecExpression::PostfixPostIncDecExpression(const std::string& _operator, Expression* postfix_expression)
    : operator_(_operator), postfix_expression_(postfix_expression)
{}

VariableStackBindings PostfixPostIncDecExpression::printAsm(VariableStackBindings bindings, unsigned& label_count) const
{
    postfix_expression_->printAsm(bindings, label_count);
    if(operator_ == "++")
	printf("\taddi\t$3,$2,1\n");
    else if(operator_ == "--")
	printf("\tsubi\t$3,$2,1\n");
    else
	throw std::runtime_error("Error : '"+operator_+"' not recognized");

    printf("\tsw\t$2,%d($fp)\n\tsw\t$3,%d($fp)\n", bindings.currentExpressionStackPosition(),
	   postfix_expression_->postfixStackPosition(bindings));
    return bindings;
}


// Pre increment and decrement implementation

UnaryPreIncDecExpression::UnaryPreIncDecExpression(const std::string& _operator, Expression* unary_expression)
    : operator_(_operator), unary_expression_(unary_expression)
{}

VariableStackBindings UnaryPreIncDecExpression::printAsm(VariableStackBindings bindings, unsigned& label_count) const
{
    unary_expression_->printAsm(bindings, label_count);
    if(operator_ == "++")
	printf("\taddi\t$2,$2,1\n");
    else if(operator_ == "--")
	printf("\tsubi\t$2,$2,1\n");
    else
	throw std::runtime_error("Error : '"+operator_+"' not recognized");
    
    printf("\tsw\t$2,%d($fp)\n\tsw\t$2,%d($fp)\n", bindings.currentExpressionStackPosition(),
	   unary_expression_->postfixStackPosition(bindings));
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
    evaluateExpression(bindings, label_count);
    
    // TODO currently using signed and sub because I only have signed numbers implemented
    // must update this as I add more types
    if(operator_ == "+")
	printf("\tadd\t$2,$2,$3\n");
    else if(operator_ == "-")
	printf("\tsub\t$2,$2,$3\n");
    else
	throw std::runtime_error("Error : '"+operator_+"' not recognized");

    // now I have to store it back into the original stack position
    printf("\tsw\t$2,%d($fp)\n", bindings.currentExpressionStackPosition());

    return bindings;
}

int AdditiveExpression::constantFold() const
{
    if(operator_ == "+")
	return lhs_->constantFold()+rhs_->constantFold();
    return lhs_->constantFold()-rhs_->constantFold();
}


// Multiplicative Expression definition


MultiplicativeExpression::MultiplicativeExpression(Expression* lhs, const std::string& _operator, Expression* rhs)
    : OperationExpression(lhs, rhs), operator_(_operator)
{}

VariableStackBindings MultiplicativeExpression::printAsm(VariableStackBindings bindings, unsigned& label_count) const
{
    evaluateExpression(bindings, label_count);

    // then perform the right operation
    if(operator_ == "*")
    {
	printf("\tmul\t$2,$2,$3\n");	
    }
    else if(operator_ == "/" || operator_ == "%")
    {
	printf("\tdiv\t$2,$3\n");
	if(operator_ == "/")
	    printf("\tmflo\t$2\n");
	else
	    printf("\tmfhi\t$2\n");	
    }
    else
    {
	throw std::runtime_error("Error : '"+operator_+"' not recognized");
    }
	

    // finally store result back into the stack position
    printf("\tsw\t$2,%d($fp)\n", bindings.currentExpressionStackPosition());

    return bindings;
}

int MultiplicativeExpression::constantFold() const
{
    if(operator_ == "*")
	return lhs_->constantFold()*rhs_->constantFold();
    else if(operator_ == "/")
	return lhs_->constantFold()/rhs_->constantFold();
    return lhs_->constantFold()%rhs_->constantFold();
}


// ShiftExpression definition

ShiftExpression::ShiftExpression(Expression* lhs, const std::string& _operator, Expression* rhs)
    : OperationExpression(lhs, rhs), operator_(_operator)
{}

VariableStackBindings ShiftExpression::printAsm(VariableStackBindings bindings, unsigned& label_count) const
{
    evaluateExpression(bindings, label_count);

    if(operator_ == "<<")
    {
	printf("\tsll\t$2,$2,$3\n");
    }
    else if(operator_ == ">>")
    {
	printf("\tsra\t$2,$2,$3\n");
    }
    else
    {
	throw std::runtime_error("Error : '"+operator_+"' not recognized");
    }

    printf("\tsw\t$2,%d($fp)\n", bindings.currentExpressionStackPosition());

    return bindings;
}

int ShiftExpression::constantFold() const
{
    if(operator_ == "<<")
	return lhs_->constantFold()<<rhs_->constantFold();
    return lhs_->constantFold()>>rhs_->constantFold();
}


// RelationalExpression definition

RelationalExpression::RelationalExpression(Expression* lhs, const std::string& _operator, Expression* rhs)
    : OperationExpression(lhs, rhs), operator_(_operator)
{}

VariableStackBindings RelationalExpression::printAsm(VariableStackBindings bindings, unsigned& label_count) const
{
    evaluateExpression(bindings, label_count);

    if(operator_ == "<")
    {
	printf("\tslt\t$2,$2,$3\n");
    }
    else if(operator_ == ">")
    {
	printf("\tslt\t$2,$3,$2\n");
    }
    else if(operator_ == "<=")
    {
	printf("\tslt\t$2,$3,$2\n\txori\t$2,$2,0x1\n");
    }
    else if(operator_ == ">=")
    {
	printf("\tslt\t$2,$2,$3\n\txori\t$2,$2,0x1\n");
    }
    else
    {
	throw std::runtime_error("Error : '"+operator_+"' not recognized");
    }

    // TODO might get rid of this
    printf("\tandi\t$2,$2,0x00ff\n\tsw\t$2,%d($fp)\n", bindings.currentExpressionStackPosition());
    return bindings;
}

int RelationalExpression::constantFold() const
{
    if(operator_ == "<")
    {
	return lhs_->constantFold()<rhs_->constantFold();
    }
    else if(operator_ == ">")
    {
	return lhs_->constantFold()>rhs_->constantFold();
    }
    else if(operator_ == "<=")
    {
	return lhs_->constantFold()<=rhs_->constantFold();
    }
    
    return lhs_->constantFold()>=rhs_->constantFold();
}


// EqualityExpression definition

EqualityExpression::EqualityExpression(Expression* lhs, const std::string& _operator, Expression* rhs)
    : OperationExpression(lhs, rhs), operator_(_operator)
{}

VariableStackBindings EqualityExpression::printAsm(VariableStackBindings bindings, unsigned& label_count) const
{
    evaluateExpression(bindings, label_count);
    printf("\txor\t$2,$2,$3\n");

    if(operator_ == "==")
    {
	printf("\tsltiu\t$2,$2,1\n");
    }
    else if(operator_ == "!=")
    {
	printf("\tsltu\t$2,$0,$2\n");
    }
    else
    {
	throw std::runtime_error("Error : '"+operator_+"' not recognized");
    }

    // TODO Work out why it is necessary to remove bytes 3 and 2.
    printf("\tandi\t$2,$2,0x00ff\n\tsw\t$2,%d($fp)\n", bindings.currentExpressionStackPosition());
    return bindings;
}

int EqualityExpression::constantFold() const
{
    if(operator_ == "==")
	return lhs_->constantFold()==rhs_->constantFold();
    return lhs_->constantFold()!=rhs_->constantFold();
}


// AndExpression definition

AndExpression::AndExpression(Expression* lhs, Expression* rhs)
    : OperationExpression(lhs, rhs)
{}

VariableStackBindings AndExpression::printAsm(VariableStackBindings bindings, unsigned& label_count) const
{
    evaluateExpression(bindings, label_count);
    printf("\tand\t$2,$2,$3\n\tsw\t$2,%d($fp)\n", bindings.currentExpressionStackPosition());
    return bindings;
}

int AndExpression::constantFold() const
{
    return lhs_->constantFold()&rhs_->constantFold();
}


// ExclusiveOrExpression definition

ExclusiveOrExpression::ExclusiveOrExpression(Expression* lhs, Expression* rhs)
    : OperationExpression(lhs, rhs)
{}

VariableStackBindings ExclusiveOrExpression::printAsm(VariableStackBindings bindings, unsigned& label_count) const
{
    evaluateExpression(bindings, label_count);
    printf("\txor\t$2,$2,$3\n\tsw\t$2,%d($fp)\n", bindings.currentExpressionStackPosition());
    return bindings;
}

int ExclusiveOrExpression::constantFold() const
{
    return lhs_->constantFold()^rhs_->constantFold();
}


// InclusiveOrExpression definition

InclusiveOrExpression::InclusiveOrExpression(Expression* lhs, Expression* rhs)
    : OperationExpression(lhs, rhs)
{}

VariableStackBindings InclusiveOrExpression::printAsm(VariableStackBindings bindings, unsigned& label_count) const
{
    evaluateExpression(bindings, label_count);
    printf("\tor\t$2,$2,$3\n\tsw\t$2,%d($fp)\n", bindings.currentExpressionStackPosition());
    return bindings;
}

int InclusiveOrExpression::constantFold() const
{
    return lhs_->constantFold()|rhs_->constantFold();
}


// LogicalAndExpression definition

LogicalAndExpression::LogicalAndExpression(Expression* lhs, Expression* rhs)
    : OperationExpression(lhs, rhs)
{}

VariableStackBindings LogicalAndExpression::printAsm(VariableStackBindings bindings, unsigned& label_count) const
{
    return bindings;
}

int LogicalAndExpression::constantFold() const
{
    return lhs_->constantFold()&&rhs_->constantFold();
}


// LogicalOrExpression definition

LogicalOrExpression::LogicalOrExpression(Expression* lhs, Expression* rhs)
    : OperationExpression(lhs, rhs)
{}

VariableStackBindings LogicalOrExpression::printAsm(VariableStackBindings bindings, unsigned& label_count) const
{
    return bindings;
}

int LogicalOrExpression::constantFold() const
{
    return lhs_->constantFold()&&rhs_->constantFold();
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

AssignmentExpression::AssignmentExpression(ExpressionPtr lhs, Expression* rhs)
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
    printf("\tlw\t$2,%d($fp)\n", expression_stack_position);

    // we are assigning so we don't have to evaluate the lhs as it will be overwritten anyways
    printf("\tsw\t$2,%d($fp)\n", store_stack_position);
    return bindings;
}


// Identifier definition

Identifier::Identifier(const std::string& id)
    : id_(id)
{}

VariableStackBindings Identifier::printAsm(VariableStackBindings bindings, unsigned& label_count) const
{
    (void)label_count;
    
    if(bindings.bindingExists(id_))
    {
	if(bindings.stackPosition(id_) == -1)
	{
	    // it's a global variable
	    printf("\tlui\t$2,%%hi(%s)\n\tlw\t$2,%%lo(%s)($2)\n", id_.c_str(), id_.c_str());   
	}
	else
	{
	    printf("\tlw\t$2,%d($fp)\n", bindings.stackPosition(id_));
	}
    }
    else
    {
	throw std::runtime_error("Error : Can't find '"+id_+"' in current scope binding");
    }

    printf("\tsw\t$2,%d($fp)\n", bindings.currentExpressionStackPosition());
    return bindings;
}

int Identifier::postfixStackPosition(VariableStackBindings bindings) const
{
    if(bindings.bindingExists(id_))
    {
	return bindings.stackPosition(id_);
    }

    throw std::runtime_error("Error : Variable '"+id_+"' not yet declared");
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
    printf("\tli\t$2,%d\n", constant_);
    printf("\tsw\t$2,%d($fp)\n", bindings.currentExpressionStackPosition());
    return bindings;
}

int Constant::constantFold() const
{
    return constant_;
}
