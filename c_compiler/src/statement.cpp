#include "ast.hpp"


// General base Statement definition

Statement::Statement(Statement* statement)
    : next_statement(statement) {}

void Statement::print() const
{
    if(next_statement != nullptr) 
	next_statement->print();
}

void Statement::printxml() const
{
    if(next_statement != nullptr) 
	next_statement->printxml();
}

void Statement::printasm() const
{
    if(next_statement != nullptr) 
	next_statement->printasm();
}


// Compound Statement definition

CompoundStatement::CompoundStatement(Declaration* decl, Statement* statement)
    : Statement(), m_decl(decl), m_statement(statement) {}

CompoundStatement::CompoundStatement(Statement* statement)
    : m_statement(statement) {}

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


// Expression Statement definition

ExpressionStatement::ExpressionStatement(Expression* expr)
    : Statement(), m_expr(expr) {}

void ExpressionStatement::print() const
{}

void ExpressionStatement::printxml() const
{}

void ExpressionStatement::printasm() const
{} 


// Jump Statement definition

JumpStatement::JumpStatement(Expression* expr)
    : m_expr(expr) {}

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


// Iteration Statement definition

IterationStatement::IterationStatement(Statement* statement)
    : m_statement(statement) {}

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
