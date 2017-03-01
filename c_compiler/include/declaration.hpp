#ifndef AST_DECLARATION_HPP
#define AST_DECLARATION_HPP

#include "ast.hpp"

#include <vector>

// Declaration that holds a list of declarations

class DeclarationList : public Base {
private:
    mutable std::vector<const Base*> dec_list;

public:
    DeclarationList(const Base* _dec) {
	dec_list.push_back(_dec);
    }

    virtual void print() const {
	for(size_t i = 0; i < dec_list.size(); ++i) {
	    dec_list[i]->print();
	}
    }

    virtual void push(const Base* _dec) const {
	dec_list.push_back(_dec);
    }
};

class VariableDeclaration : public Base {
private:
    mutable std::vector<const Base*> var_list;

public:
    VariableDeclaration(const Base* _var) {
	var_list.push_back(_var);
    }

    virtual void print() const {
	for(size_t i = 0; i < var_list.size(); ++i) {
	    var_list[i]->print();
	}
    }

    virtual void push(const Base* _var) const {
	var_list.push_back(_var);
    }
};

#endif
