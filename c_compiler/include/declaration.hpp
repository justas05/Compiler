#ifndef AST_DECLARATION_HPP
#define AST_DECLARATION_HPP

#include "ast.hpp"

// Declaration that holds a list of declarations

class Declaration : public Node {
protected:
    Type* type;
    std::string id;
    Initializer* init;
    Declaration* next_decl;
    Declaration* list_next_decl;
    
public:
    Declaration(const std::string& _id = "");

    virtual void print() const;
    virtual void printxml() const;
    virtual void printasm() const;

    void addDeclaration(Declaration* _next_decl);
    void addList(Declaration* _next_decl);

    Declaration* getNext() const;
    Declaration* getNextListItem() const;
    std::string getId() const;
};

#endif
