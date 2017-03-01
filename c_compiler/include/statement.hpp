#ifndef AST_STATEMENT_HPP
#define AST_STATEMENT_HPP

#include "ast.hpp"

class Statement : public Base {
protected:
    mutable std::vector<const Base*> list;
    
public:
    Statement() {}
    
    Statement(const Base* _el) {
        list.push_back(_el);
    }

    Statement(const Base* _dec, const Base* _statement) {
        list.push_back(_dec);
        list.push_back(_statement);
    }
    virtual void print() const {
	for(size_t i = 0; i < list.size(); ++i) {
	    list[i]->print();
	}
    }

    virtual void push(const Base* _var) const {
        list.push_back(_var);
    }
};

class StatementList : public Statement {  
public:
    StatementList(const Base* _statement) : Statement(_statement) {}
};

class CompoundStatement : public Statement {
public:
    CompoundStatement() : Statement() {}
    CompoundStatement(const Base* _el) : Statement(_el) {}
    CompoundStatement(const Base* _dec, const Base* _statement) :
        Statement(_dec, _statement) {}

    virtual void print() const override {
	std::cout << "<Scope>" << std::endl;
	for(size_t i = 0; i < list.size(); ++i) {
	    list[i]->print();
	}
	std::cout << "</Scope>" << std::endl;
    }
};

class SelectionStatement : public Statement {
public:
    SelectionStatement() : Statement() {}
    SelectionStatement(const Base* _el) : Statement(_el) {}
    SelectionStatement(const Base* _if, const Base* _else) :
        Statement(_if, _else) {}
};

class ExpressionStatement : public Statement {
public:
    ExpressionStatement() : Statement() {}
    ExpressionStatement(const Base* _el) : Statement(_el) {}
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
    IterationStatement(const Base* _if, const Base* _else) :
        Statement(_if, _else) {}
};

#endif
