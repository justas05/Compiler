#include "statement.hpp"

#include <iostream>


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
    
    std::cout << "<Scope>" << std::endl;
    
    if(declaration_ != nullptr)
	declaration_->printXml();
    
    if(statement_ != nullptr)
	statement_->printXml();
    
    std::cout << "</Scope>" << std::endl;
}

VariableStackBindings CompoundStatement::printAsm(VariableStackBindings bindings, unsigned& label_count) const
{
    VariableStackBindings outer_scope_bindings = bindings;
    
    if(next_statement_ != nullptr)
	next_statement_->printAsm(bindings, label_count);

    if(declaration_ != nullptr)
	bindings = declaration_->printAsm(bindings, label_count);

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


// Selection Statement definition

SelectionStatement::SelectionStatement(Statement* _if, Statement* _else)
    : Statement(), if_(_if), else_(_else) {}

void SelectionStatement::print() const
{
    if_->print();
    else_->print();
}

void SelectionStatement::printXml() const
{
    if(next_statement_ != nullptr)
	next_statement_->printXml();
    
    if(if_ != nullptr)
	if_->printXml();
    
    if(else_ != nullptr)
	else_->printXml();
}

VariableStackBindings SelectionStatement::printAsm(VariableStackBindings bindings, unsigned& label_count) const
{
    return bindings;
}

void SelectionStatement::countVariables(unsigned& var_count) const
{
    if(next_statement_ != nullptr)
	next_statement_->countVariables(var_count);

    if(if_ != nullptr)
	if_->countVariables(var_count);

    if(else_ != nullptr)
	else_->countVariables(var_count);
}

void SelectionStatement::countArguments(unsigned& argument_count) const
{
    if(next_statement_ != nullptr)
	next_statement_->countArguments(argument_count);

    if(if_ != nullptr)
	if_->countArguments(argument_count);

    if(else_ != nullptr)
	else_->countArguments(argument_count);
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


// Jump Statement definition

JumpStatement::JumpStatement(Expression* expression)
    : expression_(expression)
{}

void JumpStatement::print() const
{}

void JumpStatement::printXml() const
{
    if(next_statement_ != nullptr)
	next_statement_->printXml();
}

VariableStackBindings JumpStatement::printAsm(VariableStackBindings bindings, unsigned& label_count) const
{
    if(next_statement_ != nullptr)
	next_statement_->printAsm(bindings, label_count);
    
    if(expression_ != nullptr)
	expression_->printAsm(bindings, label_count);

    std::cout << "\tj\t0f\n";
    
    return bindings;
}

void JumpStatement::countVariables(unsigned& var_count) const
{
    if(next_statement_ != nullptr)
	next_statement_->countVariables(var_count);
}

void JumpStatement::countArguments(unsigned& argument_count) const
{
    if(next_statement_ != nullptr)
	next_statement_->countArguments(argument_count);

    unsigned tmp_argument_count = argument_count;

    if(expression_ != nullptr)
	expression_->countArguments(argument_count);

    if(tmp_argument_count > argument_count)
	argument_count = tmp_argument_count;
}


// Iteration Statement definition

IterationStatement::IterationStatement(Statement* statement)
    : statement_(statement)
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

VariableStackBindings IterationStatement::printAsm(VariableStackBindings bindings, unsigned& label_count) const
{
    return bindings;
}

void IterationStatement::countVariables(unsigned& var_count) const
{
    if(next_statement_ != nullptr)
	next_statement_->countVariables(var_count);

    if(statement_ != nullptr)
	statement_->countVariables(var_count);
}

void IterationStatement::countArguments(unsigned int &argument_count) const
{
    if(next_statement_ != nullptr)
	next_statement_->countArguments(argument_count);

    if(statement_ != nullptr)
	statement_->countArguments(argument_count);
}
