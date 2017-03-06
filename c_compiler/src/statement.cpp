#include "ast.hpp"


// General base Statement definition

Statement::Statement(Statement* statement)
    : next_statement(statement)
{}

void Statement::addStatement(Statement* _next)
{
    next_statement = _next;
}


// Compound Statement definition

CompoundStatement::CompoundStatement(Declaration* decl, Statement* statement)
    : Statement(), m_decl(decl), m_statement(statement)
{}

CompoundStatement::CompoundStatement(Statement* statement)
    : m_statement(statement)
{}

void CompoundStatement::print() const
{
    if(m_decl != nullptr)
	m_decl->print();
    
    if(m_statement != nullptr)
	m_statement->print();
}

void CompoundStatement::printxml() const
{
    if(next_statement != nullptr)
	next_statement->printxml();
    
    std::cout << "<Scope>" << std::endl;
    
    if(m_decl != nullptr)
	m_decl->printxml();
    
    if(m_statement != nullptr)
	m_statement->printxml();
    
    std::cout << "</Scope>" << std::endl;
}

void CompoundStatement::printasm() const
{}

void CompoundStatement::count_variables(int32_t& var_count) const
{
    Declaration* declaration = m_decl;

    if(next_statement != nullptr)
	next_statement->count_variables(var_count);

    if(m_statement != nullptr)
	m_statement->count_variables(var_count);
    
    while(declaration != nullptr) {
        Declaration* declaration_list = declaration->getNextListItem();

	while(declaration_list != nullptr) {
	    var_count++;

	    std::cout << declaration_list->getType() << std::endl;

	    declaration_list = declaration_list->getNextListItem();
	}

	std::cout << declaration->getType() << std::endl;

	var_count++;
	
	declaration = declaration->getNext();
    }
}


// Selection Statement definition

SelectionStatement::SelectionStatement(Statement* _if, Statement* _else)
    : Statement(), m_if(_if), m_else(_else) {}

void SelectionStatement::print() const
{
    m_if->print();
    m_else->print();
}

void SelectionStatement::printxml() const
{
    if(next_statement != nullptr)
	next_statement->printxml();
    if(m_if != nullptr)
	m_if->printxml();
    if(m_else != nullptr)
	m_else->printxml();
}

void SelectionStatement::printasm() const
{}

void SelectionStatement::count_variables(int32_t& var_count) const
{
    if(next_statement != nullptr)
	next_statement->count_variables(var_count);

    if(m_if != nullptr)
	m_if->count_variables(var_count);

    if(m_else != nullptr)
	m_else->count_variables(var_count);
}


// Expression Statement definition

ExpressionStatement::ExpressionStatement(Expression* expr)
    : Statement(), m_expr(expr)
{}

void ExpressionStatement::print() const
{}

void ExpressionStatement::printxml() const
{}

void ExpressionStatement::printasm() const
{}

void ExpressionStatement::count_variables(int32_t& var_count) const
{
    if(next_statement != nullptr)
	next_statement->count_variables(var_count);
}


// Jump Statement definition

JumpStatement::JumpStatement(Expression* expr)
    : m_expr(expr)
{}

void JumpStatement::print() const
{}

void JumpStatement::printxml() const
{
    if(next_statement != nullptr)
	next_statement->printxml();
}

void JumpStatement::printasm() const
{
    m_expr->printasm();
    std::cout << "\tlw\t$2,8($fp)" << std::endl;
}

void JumpStatement::count_variables(int32_t& var_count) const
{
    if(next_statement != nullptr)
	next_statement->count_variables(var_count);
}


// Iteration Statement definition

IterationStatement::IterationStatement(Statement* statement)
    : m_statement(statement)
{}

void IterationStatement::print() const
{}

void IterationStatement::printxml() const
{
    if(next_statement != nullptr)
	next_statement->printxml();
    if(m_statement != nullptr)
	m_statement->printxml();
}

void IterationStatement::printasm() const
{}

void IterationStatement::count_variables(int32_t& var_count) const
{
    if(next_statement != nullptr)
	next_statement->count_variables(var_count);

    if(m_statement != nullptr)
	m_statement->count_variables(var_count);
}
