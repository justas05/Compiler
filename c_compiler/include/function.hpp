#ifndef AST_FUNCTION_HPP
#define AST_FUNCTION_HPP

#include "ast.hpp"


class Function : public BaseNode {
protected:
    std::string id;
    
public:
    Function(const std::string& _id, const BaseList* _param_list, const BaseNode* _comp_statement);

    virtual void printxml() const override;
    virtual void printasm() const override;
};


#endif
