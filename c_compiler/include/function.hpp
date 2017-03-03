#ifndef AST_FUNCTION_HPP
#define AST_FUNCTION_HPP

#include "ast.hpp"


class Function : public Node {
protected:
    Type* type;
    std::string id;
    Declaration* parameter_list;
    Statement* statement;
    
public:
    Function(const std::string& _id, Declaration* _parameter_list) : id(_id), parameter_list(_parameter_list) {}

    virtual void print() const {
	std::cout << id << std::endl;
	if(parameter_list != nullptr)
	    parameter_list->print();
    }
    
    virtual void printxml() const {}
    virtual void printasm() const {}
};


#endif
