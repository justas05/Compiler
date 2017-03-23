#include "statement.hpp"

#include <cstdio>
#include <exception>
#include <vector>


// General base Statement definition

Statement::Statement(Statement *statement)
    : next_statement_(statement)
{}

int Statement::constantFold() const
{
    throw std::runtime_error("Error : not implemented");
}

ExpressionPtr Statement::getExpression() const
{
    return nullptr;  
}

bool Statement::isDefault() const
{
    return false;
}

void Statement::linkStatement(Statement *next)
{
    StatementPtr statement_ptr(next);
    next_statement_ = statement_ptr;
}

StatementPtr Statement::getNext() const
{
    return next_statement_;
}


// Label Statement definition

LabelStatement::LabelStatement(const std::string &label, Statement *statement)
    : label_(label), statement_(statement)
{}

void LabelStatement::print() const
{
    if(next_statement_ != nullptr)
	next_statement_->print();
    
    printf("Label Statement\n");
}

void LabelStatement::printXml() const
{
    if(next_statement_ != nullptr)
	next_statement_->printXml();
}

VariableStackBindings LabelStatement::printAsm(VariableStackBindings bindings, unsigned &label_count) const
{
    if(next_statement_ != nullptr)
	next_statement_->printAsm(bindings, label_count);

    printf("%s:\n", label_.c_str());

    statement_->printAsm(bindings, label_count);

    return bindings;
}

void LabelStatement::countVariables(unsigned &var_count) const
{
    if(next_statement_ != nullptr)
	next_statement_->countVariables(var_count);

    statement_->countVariables(var_count);
}

void LabelStatement::countArguments(unsigned &argument_count) const
{
    if(next_statement_ != nullptr)
	next_statement_->countArguments(argument_count);

    statement_->countArguments(argument_count);
}

void LabelStatement::countExpressionDepth(unsigned &depth_count) const
{
    unsigned previous_depth_count = depth_count;
    if(next_statement_ != nullptr)
    {
	next_statement_->countExpressionDepth(depth_count);
	if(previous_depth_count > depth_count)
	    depth_count = previous_depth_count;
	previous_depth_count = depth_count;
    }
    
    statement_->countExpressionDepth(depth_count);
    if(previous_depth_count > depth_count)
	depth_count = previous_depth_count;
}


// Case Statement definition

CaseStatement::CaseStatement(Statement *statement, Expression *constant_expression, const bool &_default)
    : constant_expression_(constant_expression), statement_(statement), default_(_default)
{}

void CaseStatement::print() const
{
    if(next_statement_ != nullptr)
	next_statement_->print();
    
    printf("Case Statement\n");
}

void CaseStatement::printXml() const
{
    if(next_statement_ != nullptr)
	next_statement_->printXml();
}

VariableStackBindings CaseStatement::printAsm(VariableStackBindings bindings, unsigned &label_count) const
{
    if(next_statement_ != nullptr)
	next_statement_->printAsm(bindings, label_count);

    statement_->printAsm(bindings, label_count);

    return bindings;
}

void CaseStatement::countVariables(unsigned &var_count) const
{
    if(next_statement_ != nullptr)
	next_statement_->countVariables(var_count);

    statement_->countVariables(var_count);
}

void CaseStatement::countArguments(unsigned &argument_count) const
{
    if(next_statement_ != nullptr)
	next_statement_->countArguments(argument_count);

    statement_->countArguments(argument_count);
}

void CaseStatement::countExpressionDepth(unsigned &depth_count) const
{
    unsigned previous_depth_count = depth_count;
    if(next_statement_ != nullptr)
    {
	next_statement_->countExpressionDepth(depth_count);
	if(previous_depth_count > depth_count)
	    depth_count = previous_depth_count;
	previous_depth_count = depth_count;
    }
    
    statement_->countExpressionDepth(depth_count);
    if(previous_depth_count > depth_count)
	depth_count = previous_depth_count;    
}

int CaseStatement::constantFold() const
{
    return constant_expression_->constantFold();
}

ExpressionPtr CaseStatement::getExpression() const
{
    return constant_expression_;
}

bool CaseStatement::isDefault() const
{
    return default_;
}


// Compound Statement definition

CompoundStatement::CompoundStatement(Declaration *declaration, Statement *statement)
    : Statement(), declaration_(declaration), statement_(statement)
{}

CompoundStatement::CompoundStatement(Statement *statement)
    : statement_(statement)
{}

void CompoundStatement::print() const
{
    if(declaration_ != nullptr)
	declaration_->print();
    
    if(statement_ != nullptr)
	statement_->print();
}

void CompoundStatement::printXml() const
{
    if(next_statement_ != nullptr)
	next_statement_->printXml();
    
    printf("<Scope>\n");
    
    if(declaration_ != nullptr)
	declaration_->printXml();
    
    if(statement_ != nullptr)
	statement_->printXml();
    
    printf("</Scope>\n");
}

VariableStackBindings CompoundStatement::printAsm(VariableStackBindings bindings, unsigned &label_count) const
{
    VariableStackBindings outer_scope_bindings = bindings;
    
    if(next_statement_ != nullptr)
	next_statement_->printAsm(bindings, label_count);
    
    if(declaration_ != nullptr)
	bindings = declaration_->localAsm(bindings, label_count);

    if(statement_ != nullptr)
	statement_->printAsm(bindings, label_count);

    return outer_scope_bindings;
}

void CompoundStatement::countVariables(unsigned &var_count) const
{
    DeclarationPtr declaration = declaration_;

    if(next_statement_ != nullptr)
	next_statement_->countVariables(var_count);

    if(statement_ != nullptr)
	statement_->countVariables(var_count);
    
    while(declaration != nullptr) {
        DeclarationPtr declaration_list = declaration->getNextListItem();

	while(declaration_list != nullptr) {
	    var_count++;

	    declaration_list = declaration_list->getNextListItem();
	}

	var_count++;
	
	declaration = declaration->getNext();
    }
}

void CompoundStatement::countArguments(unsigned &argument_count) const
{
    if(next_statement_ != nullptr)
	next_statement_->countArguments(argument_count);

    if(statement_ != nullptr)
	statement_->countArguments(argument_count);
}

void CompoundStatement::countExpressionDepth(unsigned &depth_count) const
{
    unsigned previous_depth_count = depth_count;
    if(next_statement_ != nullptr)
    {
	next_statement_->countExpressionDepth(depth_count);
	if(previous_depth_count > depth_count)
	    depth_count = previous_depth_count;
	previous_depth_count = depth_count;
    }

    if(statement_ != nullptr)
    {
	statement_->countExpressionDepth(depth_count);
	if(previous_depth_count > depth_count)
	    depth_count = previous_depth_count;
    }
}

StatementPtr CompoundStatement::getStatementList() const
{
    return statement_;
}


// If Else Statement definition

IfElseStatement::IfElseStatement(Expression *condition, Statement *_if, Statement *_else)
    : Statement(), condition_(condition), if_(_if), else_(_else) {}

void IfElseStatement::print() const
{
    condition_->print();
    if_->print();
    if(else_ != nullptr)
	else_->print();
}

void IfElseStatement::printXml() const
{
    if(next_statement_ != nullptr)
	next_statement_->printXml();
    
    if(if_ != nullptr)
	if_->printXml();
    
    if(else_ != nullptr)
	else_->printXml();
}

VariableStackBindings IfElseStatement::printAsm(VariableStackBindings bindings, unsigned &label_count) const
{
    if(next_statement_ != nullptr)
	next_statement_->printAsm(bindings, label_count);
    
    unsigned if_label = label_count++;
	
    condition_->printAsm(bindings, label_count);
    printf("\tbeq\t$2,$0,$%d_else\n\tnop\n", if_label);
    
    if_->printAsm(bindings, label_count);

    printf("\tb\t$%d_if_end\n\tnop\n$%d_else:\n", if_label, if_label);

    if(else_ != nullptr)
	else_->printAsm(bindings, label_count);

    printf("$%d_if_end:\n", if_label);

    return bindings;
}

void IfElseStatement::countVariables(unsigned &var_count) const
{
    if(next_statement_ != nullptr)
	next_statement_->countVariables(var_count);

    if(if_ != nullptr)
	if_->countVariables(var_count);

    if(else_ != nullptr)
	else_->countVariables(var_count);
}

void IfElseStatement::countArguments(unsigned &argument_count) const
{
    if(next_statement_ != nullptr)
	next_statement_->countArguments(argument_count);

    if(if_ != nullptr)
	if_->countArguments(argument_count);

    if(else_ != nullptr)
	else_->countArguments(argument_count);
}

void IfElseStatement::countExpressionDepth(unsigned &depth_count) const
{
    unsigned previous_depth_count = depth_count;

    if(next_statement_ != nullptr)
    {
	next_statement_->countExpressionDepth(depth_count);
	if(previous_depth_count > depth_count)
	    depth_count = previous_depth_count;
	previous_depth_count = depth_count;
    }

    depth_count = 1;
    condition_->expressionDepth(depth_count);
    if(previous_depth_count > depth_count)
	depth_count = previous_depth_count;
    previous_depth_count = depth_count;
    
    if_->countExpressionDepth(depth_count);
    if(previous_depth_count > depth_count)
	depth_count = previous_depth_count;
    previous_depth_count = depth_count;

    if(else_ != nullptr)
    {
	else_->countExpressionDepth(depth_count);
	if(previous_depth_count > depth_count)
	    depth_count = previous_depth_count;
    }
}


// Switch Statement definition

SwitchStatement::SwitchStatement(Expression *condition, Statement *statement)
    : condition_(condition), statement_(statement)
{}

void SwitchStatement::print() const
{
    if(next_statement_ != nullptr)
	next_statement_->print();
    
    printf("Switch Statement\n");
    condition_->print();
    statement_->print();
}

void SwitchStatement::printXml() const
{
    if(next_statement_ != nullptr)
	next_statement_->printXml();

    condition_->printXml();
    statement_->printXml();
}

VariableStackBindings SwitchStatement::printAsm(VariableStackBindings bindings, unsigned &label_count) const
{
    unsigned switch_count = label_count++;
    std::shared_ptr<CompoundStatement> comp_statement;
    StatementPtr case_statement_list;
    std::vector<StatementPtr> case_statement_vector;
    
    if(next_statement_ != nullptr)
	next_statement_->printAsm(bindings, label_count);

    condition_->printAsm(bindings, label_count);

    comp_statement = std::dynamic_pointer_cast<CompoundStatement>(statement_);
    if(comp_statement == nullptr)
	throw std::runtime_error("Error : not implemented");

    bindings.breakLabel("$"+std::to_string(switch_count)+"_break_switch");

    case_statement_list = comp_statement->getStatementList();
    while(case_statement_list != nullptr)
    {
	case_statement_vector.push_back(case_statement_list);
	case_statement_list = case_statement_list->getNext();
    }

    bool is_default = false;

    for(auto itr = case_statement_vector.rbegin(); itr != case_statement_vector.rend(); ++itr)
    {
	if((*itr)->getExpression() != nullptr)
	{
	    int jump_label = (*itr)->constantFold();
	    printf("\tli\t$3,%d\n\tbeq\t$2,$3,$%d_%d_switch\n\tnop\n",
		   jump_label, switch_count, jump_label);
	}
	if(!is_default)
	{
	    is_default = (*itr)->isDefault();
	}
    }

    if(is_default)
	printf("\tb\t$%d_default_switch\n\tnop\n", switch_count);

    for(auto itr = case_statement_vector.rbegin(); itr != case_statement_vector.rend(); ++itr)
    {
	if((*itr)->getExpression() != nullptr)
	    printf("$%d_%d_switch:\n", switch_count, (*itr)->constantFold());
	
	if((*itr)->isDefault())
	    printf("$%d_default_switch:\n", switch_count);
	(*itr)->linkStatement(nullptr);
	(*itr)->printAsm(bindings, label_count);
    }

    printf("$%d_break_switch:\n", switch_count);
    return bindings;
}

void SwitchStatement::countVariables(unsigned &label_count) const
{
    if(next_statement_ != nullptr)
	next_statement_->countVariables(label_count);

    statement_->countVariables(label_count);
}

void SwitchStatement::countArguments(unsigned &argument_count) const
{
    if(next_statement_ != nullptr)
	next_statement_->countArguments(argument_count);

    statement_->countArguments(argument_count);
    unsigned previous_argument_count = argument_count;
    condition_->countArguments(argument_count);

    if(previous_argument_count > argument_count)
	argument_count = previous_argument_count;
}

void SwitchStatement::countExpressionDepth(unsigned &depth_count) const
{
    unsigned previous_depth_count = depth_count;

    if(next_statement_ != nullptr)
    {
	next_statement_->countExpressionDepth(depth_count);
	if(previous_depth_count > depth_count)
	    depth_count = previous_depth_count;
	previous_depth_count = depth_count;
    }

    depth_count = 1;
    statement_->countExpressionDepth(depth_count);
    if(previous_depth_count > depth_count)
	depth_count = previous_depth_count;
    previous_depth_count = depth_count;
    
    depth_count = 1;
    condition_->expressionDepth(depth_count);
    if(previous_depth_count > depth_count)
	depth_count = previous_depth_count;   
}


// Expression Statement definition

ExpressionStatement::ExpressionStatement(Expression *expr)
    : Statement(), expression_(expr)
{}

void ExpressionStatement::print() const
{
    if(next_statement_ != nullptr)
	next_statement_->print();

    printf("Expression Statement\n");
    if(expression_ != nullptr)
	expression_->print();
}

void ExpressionStatement::printXml() const
{
    if(next_statement_ != nullptr)
	next_statement_->printXml();
}

VariableStackBindings ExpressionStatement::printAsm(VariableStackBindings bindings, unsigned &label_count) const
{
    if(next_statement_ != nullptr)
	next_statement_->printAsm(bindings, label_count);
    
    if(expression_ != nullptr)
	expression_->printAsm(bindings, label_count);
    
    return bindings;
}

void ExpressionStatement::countVariables(unsigned &var_count) const
{
    if(next_statement_ != nullptr)
	next_statement_->countVariables(var_count);
}

void ExpressionStatement::countArguments(unsigned &argument_count) const
{
    if(next_statement_ != nullptr)
	next_statement_->countArguments(argument_count);

    unsigned tmp_argument_count = argument_count;

    if(expression_ != nullptr)
	expression_->countArguments(argument_count);

    if(tmp_argument_count > argument_count)
	argument_count = tmp_argument_count;
}

void ExpressionStatement::countExpressionDepth(unsigned &depth_count) const
{
    unsigned previous_depth_count = depth_count;

    if(next_statement_ != nullptr)
    {
	next_statement_->countExpressionDepth(depth_count);
	if(previous_depth_count > depth_count)
	    depth_count = previous_depth_count;
	previous_depth_count = depth_count;
    }

    if(expression_ != nullptr)
    {
	depth_count = 1;
	expression_->expressionDepth(depth_count);
	if(previous_depth_count > depth_count)
	    depth_count = previous_depth_count;
    }
}


// Jump Statement definition

void JumpStatement::print() const
{}

void JumpStatement::printXml() const
{
    if(next_statement_ != nullptr)
	next_statement_->printXml();
}

void JumpStatement::countVariables(unsigned &var_count) const
{
    if(next_statement_ != nullptr)
	next_statement_->countVariables(var_count);
}

void JumpStatement::countArguments(unsigned &argument_count) const
{
    if(next_statement_ != nullptr)
	next_statement_->countArguments(argument_count);
}

void JumpStatement::countExpressionDepth(unsigned &depth_count) const
{
    if(next_statement_ != nullptr)
	next_statement_->countExpressionDepth(depth_count);
}


// Return statement definition

ReturnStatement::ReturnStatement(Expression *expression)
    : expression_(expression)
{}

VariableStackBindings ReturnStatement::printAsm(VariableStackBindings bindings, unsigned &label_count) const
{
    if(next_statement_ != nullptr)
	next_statement_->printAsm(bindings, label_count);
    
    if(expression_ != nullptr)
	expression_->printAsm(bindings, label_count);

    printf("\tj\t0f\n\tnop\n");
    
    return bindings;
}

void ReturnStatement::countVariables(unsigned &var_count) const
{
    if(next_statement_ != nullptr)
	next_statement_->countVariables(var_count);
}

void ReturnStatement::countArguments(unsigned &argument_count) const
{
    if(next_statement_ != nullptr)
	next_statement_->countArguments(argument_count);

    unsigned tmp_argument_count = argument_count;

    if(expression_ != nullptr)
	expression_->countArguments(argument_count);

    if(tmp_argument_count > argument_count)
	argument_count = tmp_argument_count;
}

void ReturnStatement::countExpressionDepth(unsigned &depth_count) const
{
    unsigned previous_depth_count = depth_count;

    if(next_statement_ != nullptr)
    {
	next_statement_->countExpressionDepth(depth_count);
	if(previous_depth_count > depth_count)
	    depth_count = previous_depth_count;
	previous_depth_count = depth_count;
    }

    if(expression_ != nullptr)
    {
	depth_count = 1;
	expression_->expressionDepth(depth_count);
	if(previous_depth_count > depth_count)
	    depth_count = previous_depth_count;
    }
}


// Break statement definition

BreakStatement::BreakStatement()
{}

VariableStackBindings BreakStatement::printAsm(VariableStackBindings bindings, unsigned &label_count) const
{
    if(next_statement_ != nullptr)
	next_statement_->printAsm(bindings, label_count);
    
    printf("\tb\t%s\n\tnop\n", bindings.breakLabel().c_str());
    return bindings;
}


// Continue statement

ContinueStatement::ContinueStatement()
{}

VariableStackBindings ContinueStatement::printAsm(VariableStackBindings bindings, unsigned &label_count) const
{
    if(next_statement_ != nullptr)
	next_statement_->printAsm(bindings, label_count);
    
    printf("\tb\t%s\n\tnop\n", bindings.continueLabel().c_str());
    return bindings;
}


// Goto statement

GotoStatement::GotoStatement(const std::string &label)
    : label_(label)
{}

VariableStackBindings GotoStatement::printAsm(VariableStackBindings bindings, unsigned &label_count) const
{
    if(next_statement_ != nullptr)
	next_statement_->printAsm(bindings, label_count);
    
    printf("\tb\t%s\n\tnop\n", label_.c_str());
    return bindings;
}


// Iteration Statement definition

IterationStatement::IterationStatement(Expression *condition, Statement *statement)
    : condition_(condition), statement_(statement)
{}

void IterationStatement::print() const
{}

void IterationStatement::printXml() const
{
    if(next_statement_ != nullptr)
	next_statement_->printXml();
    
    if(statement_ != nullptr)
	statement_->printXml();
}

void IterationStatement::countVariables(unsigned &var_count) const
{
    if(next_statement_ != nullptr)
	next_statement_->countVariables(var_count);

    if(statement_ != nullptr)
	statement_->countVariables(var_count);
}

void IterationStatement::countArguments(unsigned &argument_count) const
{
    if(next_statement_ != nullptr)
	next_statement_->countArguments(argument_count);

    if(statement_ != nullptr)
	statement_->countArguments(argument_count);
}

void IterationStatement::countExpressionDepth(unsigned &depth_count) const
{
    unsigned previous_depth_count = depth_count;

    if(next_statement_ != nullptr)
    {
	next_statement_->countExpressionDepth(depth_count);
	if(previous_depth_count > depth_count)
	    depth_count = previous_depth_count;
	previous_depth_count = depth_count;
    }
    
    depth_count = 1;
    condition_->expressionDepth(depth_count);
    if(previous_depth_count > depth_count)
	depth_count = previous_depth_count;
    previous_depth_count = depth_count;

    statement_->countExpressionDepth(depth_count);
    if(previous_depth_count > depth_count)
	depth_count = previous_depth_count;
}


// While Loop definition

WhileLoop::WhileLoop(Expression *condition, Statement *statement, const bool &is_while)
    : IterationStatement(condition, statement), is_while_(is_while)
{}

VariableStackBindings WhileLoop::printAsm(VariableStackBindings bindings, unsigned &label_count) const
{
    if(next_statement_ != nullptr)
	next_statement_->printAsm(bindings, label_count);

    VariableStackBindings initial_bindings = bindings;
    
    int while_label = label_count++;

    bindings.continueLabel("$"+std::to_string(while_label)+"_while_cond");
    bindings.breakLabel("$"+std::to_string(while_label)+"_while_break");

    if(is_while_)
	printf("\tb\t$%d_while_cond\n\tnop\n", while_label);
    printf("$%d_while_body:\n", while_label);
    statement_->printAsm(bindings, label_count);
    printf("$%d_while_cond:\n", while_label);
    condition_->printAsm(bindings, label_count);
    printf("\tbne\t$2,$0,$%d_while_body\n\tnop\n$%d_while_break:\n", while_label, while_label);
    
    return initial_bindings;
}

ForLoop::ForLoop(Expression *initializer, Expression *condition, Expression *incrementer, Statement *statement)
    : IterationStatement(condition, statement), initializer_(initializer), incrementer_(incrementer)
{}

VariableStackBindings ForLoop::printAsm(VariableStackBindings bindings, unsigned &label_count) const
{
    if(next_statement_ != nullptr)
	next_statement_->printAsm(bindings, label_count);

    VariableStackBindings initial_bindings = bindings;
    
    int for_label = label_count++;

    bindings.continueLabel("$"+std::to_string(for_label)+"_for_cond");
    bindings.breakLabel("$"+std::to_string(for_label)+"_for_break");
    
    initializer_->printAsm(bindings, label_count);
    printf("\tb\t$%d_for_cond\n\tnop\n$%d_for_body:\n", for_label, for_label);
    statement_->printAsm(bindings, label_count);
    incrementer_->printAsm(bindings, label_count);
    printf("$%d_for_cond:\n", for_label);
    condition_->printAsm(bindings, label_count);
    printf("\tbne\t$2,$0,$%d_for_body\n\tnop\n$%d_for_break:\n", for_label, for_label);

    return initial_bindings;
}
