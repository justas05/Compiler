#ifndef EXTERNAL_HPP
#define EXTERNAL_HPP

#include "ast.hpp"


class TranslationUnit : public Node {
protected:
    std::vector<Node* > m_transUnit;
public:
    TranslationUnit(Node* decl) {
	m_transUnit.push_back(decl);
    }

    virtual void print() const {
	for(auto& i : m_transUnit) {
	    i->print();
	}
    }
    virtual void printxml() const {}
    virtual void printasm() const {}

    void push(Node* decl) {
	m_transUnit.push_back(decl);
    }
};


#endif
