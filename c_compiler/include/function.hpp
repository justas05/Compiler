#ifndef AST_FUNCTION_HPP
#define AST_FUNCTION_HPP

#include "ast.hpp"


class Function : public BaseNode {
protected:
    std::string id;
    
public:
    Function(const std::string& _id, const BaseList* _param_list, const BaseNode* _comp_statement)
	: BaseNode(_param_list, _comp_statement), id(_id) {}

    virtual void printxml() const override {
	std::cout << "<Function id=\"" << id << "\">" << std::endl;
        leftNode->printxml();
	rightNode->printxml();
	std::cout << "</Function>" << std::endl;
    }
};


class ParamList : public BaseList {
public:
    ParamList() : BaseList() {}
    ParamList(const Base* _param) : BaseList(_param) {}
};


#endif
