#ifndef AST_DECLARATION_HPP
#define AST_DECLARATION_HPP

#include "ast.hpp"

#include <vector>

// Declaration that holds a list of declarations

class ast_DeclarationList : public ast_Base {
private:
    mutable std::vector<const ast_Base*> dec_list;
    
public:
    ast_DeclarationList(const ast_Base* _dec) {
	dec_list.push_back(_dec);
    }

    virtual void print() const {
	for(size_t i = 0; i < dec_list.size(); ++i) {
	    dec_list[i]->print();
	}
    }

    virtual void push(const ast_Base* _dec) const {
	dec_list.push_back(_dec);
    }
};

class ast_VariableDeclaration : public ast_Base {
private:
    mutable std::vector<const ast_Base*> var_list;
    
public:
    ast_VariableDeclaration(const ast_Base* _var) {
	var_list.push_back(_var);
    }

    virtual void print() const {
        for(size_t i = 0; i < var_list.size(); ++i) {
	    var_list[i]->print();
	}
    }

    virtual void push(const ast_Base* _var) const {
	var_list.push_back(_var);
    }
};

#endif
