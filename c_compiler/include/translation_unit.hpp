#ifndef EXTERNAL_HPP
#define EXTERNAL_HPP

#include "ast.hpp"


class TranslationUnit : public Node {
protected:
    std::vector<Node* > translation_unit;
    
public:
    TranslationUnit(Node* decl);

    virtual void print() const;
    virtual void printxml() const;
    virtual void printasm() const;

    void push(Node* decl);
};


#endif
