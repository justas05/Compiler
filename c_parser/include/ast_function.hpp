#ifndef AST_FUNCTION_HPP
#define AST_FUNCTION_HPP

#include "ast.hpp"

#include <string>
#include <iostream>

class ast_Function : public ast_Base {
private:
    std::string name;
    mutable std::vector<const ast_Base*> param_list;
public:
    ast_Function(const std::string& _name) : name(_name) {}
    ast_Function(const ast_Base* param) {
	param_list.push_back(param);
    }

    virtual void print() const {
	std::cout << "<Function id=\"" << name << "\">" << std::endl;
	for(size_t i = 0; i < param_list.size(); ++i) {
	    param_list[i]->print();
	}
    }

    virtual void push(const ast_Base* var) const {
	param_list.push_back(var);
    }
};

#endif
