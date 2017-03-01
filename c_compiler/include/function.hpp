#ifndef AST_FUNCTION_HPP
#define AST_FUNCTION_HPP

#include "ast.hpp"

#include <string>
#include <iostream>

class Function : public Base {
private:
    std::string id;
    const Base* param;
    const Base* comp_statement;
public:
    Function(const std::string& _id, const Base* _param, const Base* _comp_statement) :
	id(_id), param(_param), comp_statement(_comp_statement) {}

    virtual void print() const {
	std::cout << "<Function id=\"" << id << "\">" << std::endl;
        param->print();
	comp_statement->print();
	std::cout << "</Function>" << std::endl;
    }

    virtual void push(const Base* var) const {
        std::cerr << "Error: Can't call this function on this class" << std::endl;
	(void)var;
    }
};

class ParamList : public Base {
private:
    mutable std::vector<const Base*> param_list;

public:
    ParamList() {}
    
    ParamList(const Base* param) {
	param_list.push_back(param);
    }

    virtual void print() const {
	for(size_t i = 0; i < param_list.size(); ++i) {
	    param_list[i]->print();
	}
    }

    virtual void push(const Base* _var) const {
	param_list.push_back(_var);
    }
};

#endif
