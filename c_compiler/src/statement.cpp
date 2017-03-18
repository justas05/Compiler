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

SelectionStatement::SelectionStatement(Expression* condition, Statement* _if, Statement* _else)
    : Statement(), condition_(condition), if_(_if), else_(_else) {}

void SelectionStatement::print() const
{
    condition_->print();
    if_->print();
    if(else_ != nullptr)
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

VariableStackBindings SelectionStatement::printAsm(VariableStackBindings bindings,
						   unsigned& label_count) const
{
    unsigned if_label = label_count++;
	
    condition_->printAsm(bindings, label_count);
    std::cout << "\tbeq\t$2,$0,$" << if_label << "_else\n\tnop\n";
    
    if_->printAsm(bindings, label_count);

    std::cout << "\tb\t$" << if_label << "_if_end\n\tnop\n$" << if_label << "_else:\n";

    if(else_ != nullptr)
	else_->printAsm(bindings, label_count);

    std::cout << "$" << if_label << "_if_end:\n";

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

    std::cout << "\tj\t0f\n\tnop\n";
    
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

void IterationStatement::countArguments(unsigned int &argument_count) const
{
    if(next_statement_ != nullptr)
	next_statement_->countArguments(argument_count);

    if(statement_ != nullptr)
	statement_->countArguments(argument_count);
}


// While Loop definition

WhileLoop::WhileLoop(Expression* condition, Statement* statement)
    : IterationStatement(condition, statement)
{}

VariableStackBindings WhileLoop::printAsm(VariableStackBindings bindings, unsigned& label_count) const
{
    int while_label = label_count++;
    std::cout << "\tb\t$" << while_label << "_while_cond\n\tnop\n$" << while_label
	      << "_while_body:\n";

    statement_->printAsm(bindings, label_count);

    std::cout << "$" << while_label << "_while_cond:\n";

    condition_->printAsm(bindings, label_count);

    std::cout << "\tbne\t$2,$0,$" << while_label << "_while_body\n\tnop\n";
    
    return bindings;
}
