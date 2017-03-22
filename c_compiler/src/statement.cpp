#include "statement.hpp"

#include <cstdio>


// General base Statement definition

Statement::Statement(Statement* statement)
    : next_statement_(statement)
{}

void Statement::linkStatement(Statement* next)
{
    StatementPtr statement_ptr(next);
    next_statement_ = statement_ptr;
}


// Compound Statement definition

CompoundStatement::CompoundStatement(Declaration* declaration, Statement* statement)
    : Statement(), declaration_(declaration), statement_(statement)
{}

CompoundStatement::CompoundStatement(Statement* statement)
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

VariableStackBindings CompoundStatement::printAsm(VariableStackBindings bindings, unsigned& label_count) const
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

void CompoundStatement::countVariables(unsigned& var_count) const
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

void CompoundStatement::countArguments(unsigned& argument_count) const
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


// Selection Statement definition

IfElseStatement::IfElseStatement(Expression* condition, Statement* _if, Statement* _else)
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

VariableStackBindings IfElseStatement::printAsm(VariableStackBindings bindings, unsigned& label_count) const
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

void IfElseStatement::countVariables(unsigned& var_count) const
{
    if(next_statement_ != nullptr)
	next_statement_->countVariables(var_count);

    if(if_ != nullptr)
	if_->countVariables(var_count);

    if(else_ != nullptr)
	else_->countVariables(var_count);
}

void IfElseStatement::countArguments(unsigned& argument_count) const
{
    if(next_statement_ != nullptr)
	next_statement_->countArguments(argument_count);

    if(if_ != nullptr)
	if_->countArguments(argument_count);

    if(else_ != nullptr)
	else_->countArguments(argument_count);
}

void IfElseStatement::countExpressionDepth(unsigned& depth_count) const
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

// Expression Statement definition

ExpressionStatement::ExpressionStatement(Expression* expr)
    : Statement(), expression_(expr)
{}

void ExpressionStatement::print() const
{}

void ExpressionStatement::printXml() const
{}

VariableStackBindings ExpressionStatement::printAsm(VariableStackBindings bindings, unsigned& label_count) const
{
    if(next_statement_ != nullptr)
	next_statement_->printAsm(bindings, label_count);
    
    if(expression_ != nullptr)
	expression_->printAsm(bindings, label_count);
    
    return bindings;
}

void ExpressionStatement::countVariables(unsigned& var_count) const
{
    if(next_statement_ != nullptr)
	next_statement_->countVariables(var_count);
}

void ExpressionStatement::countArguments(unsigned& argument_count) const
{
    if(next_statement_ != nullptr)
	next_statement_->countArguments(argument_count);

    unsigned tmp_argument_count = argument_count;

    if(expression_ != nullptr)
	expression_->countArguments(argument_count);

    if(tmp_argument_count > argument_count)
	argument_count = tmp_argument_count;
}

void ExpressionStatement::countExpressionDepth(unsigned& depth_count) const
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

ReturnStatement::ReturnStatement(Expression* expression)
    : expression_(expression)
{}

VariableStackBindings ReturnStatement::printAsm(VariableStackBindings bindings, unsigned& label_count) const
{
    if(next_statement_ != nullptr)
	next_statement_->printAsm(bindings, label_count);
    
    if(expression_ != nullptr)
	expression_->printAsm(bindings, label_count);

    printf("\tj\t0f\n\tnop\n");
    
    return bindings;
}

void ReturnStatement::countVariables(unsigned& var_count) const
{
    if(next_statement_ != nullptr)
	next_statement_->countVariables(var_count);
}

void ReturnStatement::countArguments(unsigned& argument_count) const
{
    if(next_statement_ != nullptr)
	next_statement_->countArguments(argument_count);

    unsigned tmp_argument_count = argument_count;

    if(expression_ != nullptr)
	expression_->countArguments(argument_count);

    if(tmp_argument_count > argument_count)
	argument_count = tmp_argument_count;
}

void ReturnStatement::countExpressionDepth(unsigned& depth_count) const
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

VariableStackBindings BreakStatement::printAsm(VariableStackBindings bindings, unsigned &) const
{
    printf("\tb\t%s\n\tnop\n", bindings.breakLabel().c_str());
    return bindings;
}


// Continue statement

ContinueStatement::ContinueStatement()
{}

VariableStackBindings ContinueStatement::printAsm(VariableStackBindings bindings, unsigned &) const
{
    printf("\tb\t%s\n\tnop\n", bindings.continueLabel().c_str());
    return bindings;
}


// Goto statement

GotoStatement::GotoStatement(const std::string &label)
    : label_(label)
{}

VariableStackBindings GotoStatement::printAsm(VariableStackBindings bindings, unsigned &) const
{
    printf("\tb\t%s\n\tnop\n", label_.c_str());
    return bindings;
}


// Iteration Statement definition

IterationStatement::IterationStatement(Expression* condition, Statement* statement)
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

void IterationStatement::countVariables(unsigned& var_count) const
{
    if(next_statement_ != nullptr)
	next_statement_->countVariables(var_count);

    if(statement_ != nullptr)
	statement_->countVariables(var_count);
}

void IterationStatement::countArguments(unsigned& argument_count) const
{
    if(next_statement_ != nullptr)
	next_statement_->countArguments(argument_count);

    if(statement_ != nullptr)
	statement_->countArguments(argument_count);
}

void IterationStatement::countExpressionDepth(unsigned& depth_count) const
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

WhileLoop::WhileLoop(Expression* condition, Statement* statement, const bool &is_while)
    : IterationStatement(condition, statement), is_while_(is_while)
{}

VariableStackBindings WhileLoop::printAsm(VariableStackBindings bindings,
					  unsigned& label_count) const
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

ForLoop::ForLoop(Expression* initializer, Expression* condition,
		 Expression* incrementer, Statement* statement)
    : IterationStatement(condition, statement), initializer_(initializer), incrementer_(incrementer)
{}

VariableStackBindings ForLoop::printAsm(VariableStackBindings bindings, unsigned& label_count) const
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
