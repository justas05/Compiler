#ifndef AST_DECLARATION_HPP
#define AST_DECLARATION_HPP

#include "ast.hpp"

class ast_Declaration : public ast_Base {
private:
    
public:
    virtual void print() const override = 0;
};

class ast_VariableDeclaration : public ast_Declartaion {
private:
    const std::string id;
    const std::string type;
    
public:
    ast_VariableDeclaration(const std::string& _id, const std::string& _type) :
	id(_id), type(_type) {}

    virtual void print() const override {}
};

#endif
