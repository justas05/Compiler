#ifndef AST_FUNCTION_HPP
#define AST_FUNCTION_HPP

#include "ast.hpp"

#include <string>
#include <iostream>

class ast_Function : public ast_Base {
private:
    std::string id;
    const ast_Base* param;
public:
    ast_Function(const std::string& _id, const ast_Base* _param) : id(_id), param(_param) {}

    virtual void print() const {
	std::cout << "<Function id=\"" << id << "\">" << std::endl;
        param->print();
	std::cout << "</Function>" << std::endl;
    }

    virtual void push(const ast_Base* var) const {}
};

class ast_ParamList : public ast_Base {
private:
    mutable std::vector<const ast_Base*> param_list;

public:
    ast_ParamList(const ast_Base* param) {
	param_list.push_back(param);
    }

    virtual void print() const {
	for(size_t i = 0; i < param_list.size(); ++i) {
	    param_list[i]->print();
	}
    }

    virtual void push(const ast_Base* _var) const {
	param_list.push_back(_var);
    }
};

#endif
