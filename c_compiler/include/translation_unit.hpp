#ifndef TRANSLATION_UNIT_HPP
#define TRANSLATION_UNIT_HPP

#include "node.hpp"

#include <vector>

class TranslationUnit : public Node {
protected:
    std::vector<Node* > translation_unit;
    
public:
    TranslationUnit(Node* decl);

    virtual void print() const;
    virtual void printxml() const;
    virtual VariableStackBindings printasm(VariableStackBindings bindings) const;

    void push(Node* decl);
};


#endif
