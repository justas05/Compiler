#include "translation_unit.hpp"
#include "bindings.hpp"

#include <iostream>

// Translation Unit definition

TranslationUnit::TranslationUnit(Node* decl)
{
    push(decl);
}

void TranslationUnit::print() const
{
    for(auto& node : translation_unit) {
	node->print();
    }
}

void TranslationUnit::printxml() const
{
    std::cout << "<?xml version=\"1.0\"?>\n<Program>" << std::endl;
    for(auto& node : translation_unit) {
	node->printxml();
    }
    std::cout << "</Program>" << std::endl;
}

VariableStackBindings TranslationUnit::printasm(VariableStackBindings bindings) const
{
    for(auto& node : translation_unit) {
	node->printasm(bindings);
    }

    return bindings;
}

void TranslationUnit::push(Node* decl)
{
    NodePtr node_ptr(decl);
    translation_unit.push_back(node_ptr);
}
