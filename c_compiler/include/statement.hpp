#ifndef AST_STATEMENT_HPP
#define AST_STATEMENT_HPP

#include "ast.hpp"


class Statement : public BaseNode {
public:
    Statement() : BaseNode() {}
    
    Statement(const Base* _el) : BaseNode(_el) {}
};

class StatementList : public BaseList { 
public:
    StatementList(const Base* _statement) : BaseList(_statement) {}
};

class CompoundStatement : public Statement {
public:
    CompoundStatement() : Statement() {}
    CompoundStatement(const Base* _el) : Statement(_el) {}
    
    CompoundStatement(const Base* _dec, const Base* _statement) {
        leftNode = _dec;
        rightNode = _statement;
    }

    virtual void printxml() const override {
	std::cout << "<Scope>" << std::endl;
        leftNode->printxml();
	rightNode->printxml();
	std::cout << "</Scope>" << std::endl;
    }
};

class SelectionStatement : public Statement {
public:
    SelectionStatement() : Statement() {}
    SelectionStatement(const Base* _el) : Statement(_el) {}
    
    SelectionStatement(const Base* _if, const Base* _else) {
	leftNode = _if;
	rightNode = _else;
    }
};

class ExpressionStatement : public Statement {
public:
    ExpressionStatement() : Statement() {}
    ExpressionStatement(const Base* expr) : Statement(expr) {}
};

class JumpStatement : public Statement {
public:
    JumpStatement() : Statement() {}
    JumpStatement(const Base* _el) : Statement(_el) {}
};

class IterationStatement : public Statement {
public:
    IterationStatement() : Statement() {}
    IterationStatement(const Base* _el) : Statement(_el) {}
};

#endif
