#ifndef AST_DECLARATION_HPP
#define AST_DECLARATION_HPP

#include "node.hpp"

class Expression;


class Declaration : public Node {
private:
    Type* type;
    std::string id;
    Expression* init;
    Declaration* next_decl;
    Declaration* list_next_decl;
    
public:
    Declaration(const std::string& _id = "");

    virtual void print() const;
    virtual void printxml() const;
    virtual VariableStackBindings printasm(VariableStackBindings bindings) const;

    void addDeclaration(Declaration* _next_decl);
    void addList(Declaration* _next_decl);

    void setType(Type* _type);

    Declaration* getNext() const;
    Declaration* getNextListItem() const;
    std::string getId() const;
    std::string getType() const;
};

#endif
