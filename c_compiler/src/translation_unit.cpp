#include "translation_unit.hpp"

#include <iostream>

// Translation Unit definition

TranslationUnit::TranslationUnit(Node* external_declaration)
{
    push(external_declaration);
}

void TranslationUnit::print() const
{
    for(auto& node : translation_unit_) {
	node->print();
    }
}

void TranslationUnit::printXml() const
{
    std::cout << "<?xml version=\"1.0\"?>\n<Program>" << std::endl;
    for(auto& node : translation_unit_) {
	node->printXml();
    }
    std::cout << "</Program>" << std::endl;
}

VariableStackBindings TranslationUnit::printAsm(VariableStackBindings bindings) const
{
    for(auto& node : translation_unit_) {
	node->printAsm(bindings);
    }

    return bindings;
}

void TranslationUnit::push(Node* external_declaration)
{
    NodePtr node_ptr(external_declaration);
    translation_unit_.push_back(node_ptr);
}
