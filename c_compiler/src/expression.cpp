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
{
    if(next_expression_ != nullptr)
	next_expression_->print();

    printf("Expression\n");
}

void Expression::printXml() const
{}

void Expression::countArguments(int &) const
{
    // by default don't do anything to the count
}

void Expression::expressionDepth(int &depth_count) const
{
    if(next_expression_ != nullptr)
	next_expression_->expressionDepth(depth_count);
}

std::string Expression::id() const
{
    // by default return empty id, which cannot be valid.
    return "";
}

TypePtr Expression::getType(const VariableStackBindings &) const
{
    // by default return largest size, which is 32 bits
    return std::make_shared<Int>();
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

OperationExpression::OperationExpression(Expression *lhs, Expression *rhs)
    : lhs_(lhs), rhs_(rhs)
{}

OperationExpression::OperationExpression(ExpressionPtr lhs, Expression *rhs)
    : lhs_(lhs), rhs_(rhs)
{}

int OperationExpression::constantFold() const
{
    throw std::runtime_error("Error : Cannot constant fold expression");
}

void OperationExpression::expressionDepth(int &depth_count) const
{
    int lhs_depth_count = depth_count;
    int rhs_depth_count = depth_count+1;

    lhs_->expressionDepth(lhs_depth_count);
    rhs_->expressionDepth(rhs_depth_count);

    if(lhs_depth_count > rhs_depth_count)
	depth_count = lhs_depth_count;
    else
	depth_count = rhs_depth_count;
}

TypePtr OperationExpression::getType(const VariableStackBindings &bindings) const
{
    return lhs_->getType(bindings);
}

ExpressionPtr OperationExpression::getLhs() const
{
    return lhs_;
}

ExpressionPtr OperationExpression::getRhs() const
{
    return rhs_;
}

void OperationExpression::evaluateExpression(VariableStackBindings bindings, int &label_count) const
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
    lhs_->getType(bindings)->load(2, lhs_stack_position);
    printf("\tlw\t$3,%d($fp)\n", bindings.currentExpressionStackPosition());
}


// Unary expression definition

void UnaryExpression::stackPosition(VariableStackBindings, int &) const
{
    throw std::runtime_error("Error : Cannot get stack position of expression");
}


// PostfixArrayElement

PostfixArrayElement::PostfixArrayElement(Expression *postfix_expression, Expression *index_expression)
    : postfix_expression_(postfix_expression), index_expression_(index_expression)
{}

VariableStackBindings PostfixArrayElement::printAsm(VariableStackBindings bindings, int &label_count) const
{
    stackPosition(bindings, label_count);
    TypePtr type_ptr = postfix_expression_->getType(bindings);
    type_ptr->load();
    printf("\tsw\t$2,%d($fp)\n", bindings.currentExpressionStackPosition());
    return bindings;
}

void PostfixArrayElement::stackPosition(VariableStackBindings bindings, int &label_count) const
{
    index_expression_->printAsm(bindings, label_count);

    std::shared_ptr<UnaryExpression> unary_expression;
    unary_expression = std::static_pointer_cast<UnaryExpression>(postfix_expression_);
    unary_expression->stackPosition(bindings, label_count);

    printf("\tli\t$3,%d\n\tmul\t$2,$2,$3\n\taddu\t$t0,$t0,$2\n",
	   unary_expression->getType(bindings)->getSize());
}

void PostfixArrayElement::expressionDepth(int &depth_count) const
{
    if(nextExpression() != nullptr)
	nextExpression()->expressionDepth(depth_count);

    if(index_expression_ != nullptr)
	index_expression_->expressionDepth(depth_count);
}

TypePtr PostfixArrayElement::getType(const VariableStackBindings &bindings) const
{
    return postfix_expression_->getType(bindings);
}


// PostfixFunctionCall

PostfixFunctionCall::PostfixFunctionCall(Expression *argument_expression_list)
    : argument_expression_list_(argument_expression_list)
{}

VariableStackBindings PostfixFunctionCall::printAsm(VariableStackBindings bindings, int &label_count) const
{
    std::vector<ExpressionPtr> argument_vector;
    ExpressionPtr current_argument = argument_expression_list_;
    int argument_counter = 0;

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
	    (*itr)->getType(bindings)->store(4*argument_counter);

	argument_counter++;
    }

    printf("\tjal\t%s\n\tnop\n\tsw\t$2,%d($fp)\n",
	   postfix_expression_->id().c_str(), bindings.currentExpressionStackPosition());
    return bindings;
}

void PostfixFunctionCall::countArguments(int &argument_count) const
{
    ExpressionPtr current_argument = argument_expression_list_;

    argument_count = 0;
    
    while(current_argument != nullptr)
    {
	argument_count++;
	current_argument = current_argument->nextExpression();
    }
}

void PostfixFunctionCall::setPostfixExpression(Expression *postfix_expression)
{
    ExpressionPtr expression_ptr(postfix_expression);
    postfix_expression_ = expression_ptr;
}

void PostfixFunctionCall::expressionDepth(int &depth_count) const
{
    if(argument_expression_list_ != nullptr)
	argument_expression_list_->expressionDepth(depth_count);
}

TypePtr PostfixFunctionCall::getType(const VariableStackBindings &) const
{
    return std::make_shared<Int>();
}


// Post increment and decrement definition

PostfixPostIncDecExpression::PostfixPostIncDecExpression(const std::string &_operator, Expression *postfix_expression)
    : operator_(_operator), postfix_expression_(postfix_expression)
{}

VariableStackBindings PostfixPostIncDecExpression::printAsm(VariableStackBindings bindings, int &label_count) const
{
    postfix_expression_->printAsm(bindings, label_count);
    if(operator_ == "++")
	printf("\taddiu\t$3,$2,1\n");
    else if(operator_ == "--")
	printf("\tsubiu\t$3,$2,1\n");
    else
	throw std::runtime_error("Error : '"+operator_+"' not recognized");

    std::shared_ptr<UnaryExpression> unary_expression;
    unary_expression = std::static_pointer_cast<UnaryExpression>(postfix_expression_);

    printf("\tsw\t$2,%d($fp)\n", bindings.currentExpressionStackPosition());
    unary_expression->stackPosition(bindings, label_count);
    TypePtr tmp_ptr = postfix_expression_->getType(bindings);
    if(std::dynamic_pointer_cast<Char>(tmp_ptr))
    {	
	printf("\tsb\t$3,0($t0)\n");
    }
    else if(std::dynamic_pointer_cast<Short>(tmp_ptr))
    {
	printf("\tsh\t$3,0($t0)\n");
    }
    else
    {
	printf("\tsw\t$3,0($t0)\n");
    }
    
    return bindings;
}

TypePtr PostfixPostIncDecExpression::getType(const VariableStackBindings &bindings) const
{
    return postfix_expression_->getType(bindings);
}


// Pre increment and decrement implementation

UnaryPreIncDecExpression::UnaryPreIncDecExpression(const std::string &_operator, Expression *unary_expression)
    : operator_(_operator), unary_expression_(unary_expression)
{}

VariableStackBindings UnaryPreIncDecExpression::printAsm(VariableStackBindings bindings, int &label_count) const
{
    unary_expression_->printAsm(bindings, label_count);
    if(operator_ == "++")
	printf("\taddi\t$2,$2,1\n");
    else if(operator_ == "--")
	printf("\tsubi\t$2,$2,1\n");
    else
	throw std::runtime_error("Error : '"+operator_+"' not recognized");

    std::shared_ptr<UnaryExpression> unary_expression;
    unary_expression = std::static_pointer_cast<UnaryExpression>(unary_expression_);

    printf("\tsw\t$2,%d($fp)\n", bindings.currentExpressionStackPosition());
    unary_expression->stackPosition(bindings, label_count);
    unary_expression_->getType(bindings)->store();
    return bindings;
}

TypePtr UnaryPreIncDecExpression::getType(const VariableStackBindings &bindings) const
{
    return unary_expression_->getType(bindings);
}


// Operator unary definition

OperatorUnaryExpression::OperatorUnaryExpression(const std::string &_operator, Expression *cast_expression)
    : operator_(_operator), cast_expression_(cast_expression)
{}

VariableStackBindings OperatorUnaryExpression::printAsm(VariableStackBindings bindings, int &label_count) const
{
    cast_expression_->printAsm(bindings, label_count);
    if(operator_ == "!")
    {
	printf("\tsltu\t$2,$2,1\n\tandi\t$2,$2,0x00ff\n");
    }
    else if(operator_ == "~")
    {
	printf("\tnor\t$2,$0,$2\n");
    }
    else if(operator_ == "&")
    {
	std::shared_ptr<UnaryExpression> unary_expression;
	unary_expression = std::static_pointer_cast<UnaryExpression>(cast_expression_);
	unary_expression->stackPosition(bindings, label_count);
	printf("\tmove\t$2,$t0\n");
    }
    else if(operator_ == "-")
    {
	printf("\tsubu\t$2,$0,$2\n");
    }
    else if(operator_ == "*")
    {
	printf("\tlw\t$2,0($2)\n");
    }

    printf("\tsw\t$2,%d($fp)\n", bindings.currentExpressionStackPosition());

    return bindings;
}

void OperatorUnaryExpression::stackPosition(VariableStackBindings bindings, int &label_count) const
{
    if(operator_ == "*")
    {	
	std::shared_ptr<UnaryExpression> unary_expression;
	unary_expression = std::static_pointer_cast<UnaryExpression>(cast_expression_);
	unary_expression->stackPosition(bindings, label_count);
    }
}

TypePtr OperatorUnaryExpression::getType(const VariableStackBindings &bindings) const
{
    return cast_expression_->getType(bindings);
}


// CastExpression definition

CastExpression::CastExpression(Type *type, Expression *expression)
    : type_(type), expression_(expression)
{}

VariableStackBindings CastExpression::printAsm(VariableStackBindings bindings, int &label_count) const
{
    return bindings;
}

void CastExpression::expressionDepth(int &depth_count) const
{
    if(nextExpression() != nullptr)
	nextExpression()->expressionDepth(depth_count);

    expression_->expressionDepth(depth_count);
}

TypePtr CastExpression::getType(const VariableStackBindings &) const
{
    return type_;
}


// Additive Expression definition

AdditiveExpression::AdditiveExpression(Expression *lhs, const std::string &_operator, Expression *rhs)
    : OperationExpression(lhs, rhs), operator_(_operator)
{}

VariableStackBindings AdditiveExpression::printAsm(VariableStackBindings bindings, int &label_count) const
{
    evaluateExpression(bindings, label_count);
    
    // TODO currently using signed and sub because I only have signed numbers implemented
    // must update this as I add more types
    if(operator_ == "+")
	printf("\taddu\t$2,$2,$3\n");
    else if(operator_ == "-")
	printf("\tsubu\t$2,$2,$3\n");
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


MultiplicativeExpression::MultiplicativeExpression(Expression *lhs, const std::string &_operator, Expression *rhs)
    : OperationExpression(lhs, rhs), operator_(_operator)
{}

VariableStackBindings MultiplicativeExpression::printAsm(VariableStackBindings bindings, int &label_count) const
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

ShiftExpression::ShiftExpression(Expression* lhs, const std::string &_operator, Expression *rhs)
    : OperationExpression(lhs, rhs), operator_(_operator)
{}

VariableStackBindings ShiftExpression::printAsm(VariableStackBindings bindings, int &label_count) const
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

RelationalExpression::RelationalExpression(Expression* lhs, const std::string &_operator, Expression *rhs)
    : OperationExpression(lhs, rhs), operator_(_operator)
{}

VariableStackBindings RelationalExpression::printAsm(VariableStackBindings bindings, int &label_count) const
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

EqualityExpression::EqualityExpression(Expression *lhs, const std::string &_operator, Expression *rhs)
    : OperationExpression(lhs, rhs), operator_(_operator)
{}

VariableStackBindings EqualityExpression::printAsm(VariableStackBindings bindings, int &label_count) const
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

AndExpression::AndExpression(Expression *lhs, Expression *rhs)
    : OperationExpression(lhs, rhs)
{}

VariableStackBindings AndExpression::printAsm(VariableStackBindings bindings, int &label_count) const
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

ExclusiveOrExpression::ExclusiveOrExpression(Expression *lhs, Expression *rhs)
    : OperationExpression(lhs, rhs)
{}

VariableStackBindings ExclusiveOrExpression::printAsm(VariableStackBindings bindings, int &label_count) const
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

InclusiveOrExpression::InclusiveOrExpression(Expression *lhs, Expression *rhs)
    : OperationExpression(lhs, rhs)
{}

VariableStackBindings InclusiveOrExpression::printAsm(VariableStackBindings bindings, int &label_count) const
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

LogicalAndExpression::LogicalAndExpression(Expression *lhs, Expression *rhs)
    : OperationExpression(lhs, rhs)
{}

VariableStackBindings LogicalAndExpression::printAsm(VariableStackBindings bindings, int &label_count) const
{
    int log_and = label_count++;
    lhs_->printAsm(bindings, label_count);
    printf("\tbeq\t$2,$0,$%d_log_and_load_0\n\tnop\n", log_and);
    rhs_->printAsm(bindings, label_count);
    printf("\tbeq\t$2,$0,$%d_log_and_load_0\n\tnop\n", log_and);
    printf("\tli\t$2,1\n\tb\t$%d_log_and_end\n\tnop\n", log_and);
    printf("$%d_log_and_load_0:\n\tmove\t$2,$0\n$%d_log_and_end:\n", log_and, log_and);
    printf("\tsw\t$2,%d($fp)\n", bindings.currentExpressionStackPosition());
    return bindings;
}

int LogicalAndExpression::constantFold() const
{
    return lhs_->constantFold()&&rhs_->constantFold();
}


// LogicalOrExpression definition

LogicalOrExpression::LogicalOrExpression(Expression *lhs, Expression *rhs)
    : OperationExpression(lhs, rhs)
{}

VariableStackBindings LogicalOrExpression::printAsm(VariableStackBindings bindings, int &label_count) const
{
    int log_or = label_count++;
    lhs_->printAsm(bindings, label_count);
    printf("\tbne\t$2,$0,$%d_log_or_load_1\n\tnop\n", log_or);
    rhs_->printAsm(bindings, label_count);
    printf("\tbeq\t$2,$0,$%d_log_or_load_0\n\tnop\n", log_or);
    printf("$%d_log_or_load_1:\n\tli\t$2,1\n\tb\t$%d_log_or_end\n\tnop\n", log_or, log_or);
    printf("$%d_log_or_load_0:\n\tmove\t$2,$0\n$%d_log_or_end:\n", log_or, log_or);
    printf("\tsw\t$2,%d($fp)\n", bindings.currentExpressionStackPosition());
    return bindings;
}

int LogicalOrExpression::constantFold() const
{
    return lhs_->constantFold()&&rhs_->constantFold();
}


// ConditionalExpression definition

ConditionalExpression::ConditionalExpression(Expression *logical_or,
					     Expression *expression,
					     Expression *conditional_expression)
    : logical_or_(logical_or), expression_(expression),
      conditional_expression_(conditional_expression)
{}

VariableStackBindings ConditionalExpression::printAsm(VariableStackBindings bindings, int &label_count) const
{
    return bindings;
}

TypePtr ConditionalExpression::getType(const VariableStackBindings &bindings) const
{
    return std::make_shared<Int>();
}


// Assignment Expression definition

AssignmentExpression::AssignmentExpression(Expression *lhs, Expression *rhs)
    : OperationExpression(lhs, rhs)
{}

AssignmentExpression::AssignmentExpression(ExpressionPtr lhs, Expression *rhs)
    : OperationExpression(lhs, rhs)
{}

VariableStackBindings AssignmentExpression::printAsm(VariableStackBindings bindings, int &label_count) const
{
    // TODO add stack and store results in there, also for addition and multiplication.

    // get the current location of lhs in the stack so that I can store result there
    std::shared_ptr<UnaryExpression> lhs_postfix;
    lhs_postfix = std::static_pointer_cast<UnaryExpression>(lhs_);

    // get the current available stack position
    int expression_stack_position = bindings.currentExpressionStackPosition();

    // evaluate rhs and get the result back at the stack position I assigned
    // don't have to change the stack position as there is no lhs to evaluate
    rhs_->printAsm(bindings, label_count);
    bindings.nextExpressionStackPosition();

    // we are assigning so we don't have to evaluate the lhs as it will be overwritten anyways
    lhs_postfix->stackPosition(bindings, label_count);
    
    // now the result of the rhs will be in that stack position, so we can load it into $2
    TypePtr lhs_type = lhs_->getType(bindings);
    lhs_type->load(2, expression_stack_position);

    // check if lhs is trying to access an array
    std::shared_ptr<PostfixArrayElement> lhs_tmp;
    lhs_tmp = std::dynamic_pointer_cast<PostfixArrayElement>(lhs_);
    if(lhs_tmp != nullptr)
    {
	std::shared_ptr<Pointer> lhs_pointer_type;
	lhs_pointer_type = std::dynamic_pointer_cast<Pointer>(lhs_type);
	if(lhs_pointer_type != nullptr)
	{
	    lhs_pointer_type->pointerStore();
	}
	else
	{
	    lhs_type->store();
	}
    }
    else
    {
	lhs_type->store();
    }

    return bindings;
}


// Identifier definition

Identifier::Identifier(const std::string &id)
    : id_(id)
{}

VariableStackBindings Identifier::printAsm(VariableStackBindings bindings, int &) const
{
    if(bindings.bindingExists(id_))
    {
	int stack_position = bindings.stackPosition(id_);
	if(stack_position == -1)
	{
	    // it's a global variable
	    printf("\tlui\t$2,%%hi(%s)\n\tlw\t$2,%%lo(%s)($2)\n", id_.c_str(), id_.c_str());   
	}
	else
	{
	    if(std::dynamic_pointer_cast<Array>(bindings.getType(id_)) != nullptr)
	    {
		printf("\taddiu\t$2,$fp,%d\n", stack_position);
	    }
	    else
	    {
		bindings.getType(id_)->load(2, stack_position);
	    }
	}
    }
    else
    {
	throw std::runtime_error("Error : Can't find '"+id_+"' in current scope binding");
    }

    printf("\tsw\t$2,%d($fp)\n", bindings.currentExpressionStackPosition());
    return bindings;
}

void Identifier::stackPosition(VariableStackBindings bindings, int &) const
{
    if(bindings.bindingExists(id_))
    {
	if(std::dynamic_pointer_cast<Pointer>(bindings.getType(id_)) != nullptr)
	    printf("\tlw\t$3,%d($fp)\n\tmove\t$t0,$3\n", bindings.stackPosition(id_));
	else
	    printf("\taddiu\t$t0,$fp,%d\n", bindings.stackPosition(id_));
	return;
    }

    throw std::runtime_error("Error : '"+id_+"' not yet declared");
}

std::string Identifier::id() const
{
    return id_;
}

TypePtr Identifier::getType(const VariableStackBindings &bindings) const
{
    return bindings.getType(id_);
}


// Constant definition

Constant::Constant(const int32_t &constant)
    : constant_(constant)
{}

VariableStackBindings Constant::printAsm(VariableStackBindings bindings, int &) const
{
    // constant only has to load to $2 because the other expression will take care of the rest
    printf("\tli\t$2,%d\n", constant_);
    printf("\tsw\t$2,%d($fp)\n", bindings.currentExpressionStackPosition());
    return bindings;
}

int Constant::constantFold() const
{
    return constant_;
}

TypePtr Constant::getType(const VariableStackBindings &) const
{
    return std::make_shared<Int>();
}
