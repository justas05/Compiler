#ifndef AST_DECLARATION_HPP
#define AST_DECLARATION_HPP

#include "ast.hpp"

// Declaration that holds a list of declarations

class Declaration : public Node {
protected:
    Type* type;
    std::string id;
    Initializer* init;
    Declaration* decl;
    
public:
    Declaration(const Type* _type = nullptr,
		const std::string _id = "",
		const Initializer* _init = nullptr);

    virtual void print() const;
    virtual void printxml() const;
    virtual void printasm() const;

    
};

#endif
