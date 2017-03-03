#ifndef AST_FUNCTION_HPP
#define AST_FUNCTION_HPP

#include "ast.hpp"


class Function : public Node {
protected:
    const Type* type;
    std::string id;
    const DeclarationList* parameter_list;
    const Statement* statement;
    
public:
    Function(const std::string& _id, const Statement* _comp_statement);

    virtual ~Function();

    virtual void printxml() const override;
    virtual void printasm() const override;

    void push_parameter(const Declaration* declaration) const;
};


#endif
