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
    Declaration(const std::string& _id = "") : id(_id) {}

    virtual void print() const {
	std::cout << id << std::endl;
	if(decl != nullptr)
	    decl->print();
    }
    virtual void printxml() const {}
    virtual void printasm() const {}

    void addDeclaration(Declaration* _decl) {
	decl = _decl;
    }
};

#endif
