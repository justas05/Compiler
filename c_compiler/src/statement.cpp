#include "ast.hpp"


// General base Statement definition

Statement::Statement(const Base* _left, const Base* _right)
    : BaseNode(_left, _right) {}


// Statement list definition

StatementList::StatementList(const Base* _statement)
    : BaseList(_statement) {}


// Compound Statement definition

CompoundStatement::CompoundStatement(const Base* _dec, const Base* _statement)
    : Statement(_dec, _statement) {}

void CompoundStatement::printxml() const {
    std::cout << "<Scope>" << std::endl;
    leftNode->printxml();
    rightNode->printxml();
    std::cout << "</Scope>" << std::endl;
}


// Selection Statement definition

SelectionStatement::SelectionStatement(const Base* _if, const Base* _else)
    : Statement(_if, _else) {}


// Expression Statement definition

ExpressionStatement::ExpressionStatement(const Base* _expr)
    : Statement(_expr) {}


// Jump Statement definition

JumpStatement::JumpStatement(const Base* _el) : Statement(_el) {}

void JumpStatement::printasm() const {
    leftNode->printasm();
    std::cout << "\tlw\t$2,8($fp)" << std::endl;
}


// Iteration Statement definition

IterationStatement::IterationStatement(const Base* _el) : Statement(_el) {}
