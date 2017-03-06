 #ifndef AST_FUNCTION_HPP
#define AST_FUNCTION_HPP

#include "ast.hpp"


struct VarLocation {
    Type* type;
    int32_t stack_position;
};


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
    virtual void printasm() const;
};


#endif
