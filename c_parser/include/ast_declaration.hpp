#ifndef AST_DECLARATION_HPP
#define AST_DECLARATION_HPP

#include "ast.hpp"

#include <vector>

// Declaration that holds a list of declarations

class ast_Declaration : public ast_Base {
private:
    
public:
    virtual void print() const = 0;
};

class ast_VariableDeclaration : public ast_Declaration {
private:
    mutable std::vector<const ast_Base*> var_list;
    
public:
    ast_VariableDeclaration(const ast_Base* _var) {
	var_list.push_back(_var);
    }

    virtual void print() const {
        for(int i = 0; i < var_list.size(); ++i) {
	    var_list[i]->print();
	}
    }

    virtual void push(const ast_Base* _var) const {
	var_list.push_back(_var);
    }
};

#endif
