#ifndef AST_FUNCTION_HPP
#define AST_FUNCTION_HPP

#include "node.hpp"

class Declaration;
class Statement;


class Function : public Node {
protected:
    Type* type;
    std::string id;
    Declaration* parameter_list;
    Statement* statement;
    
public:
    Function(const std::string& _id, Declaration* _parameter_list, Statement* _statement);

    virtual void print() const;
    virtual void printxml() const;
    virtual VariableStackBindings printasm(VariableStackBindings bindings) const;
};


#endif
