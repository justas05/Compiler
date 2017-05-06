#include "translation_unit.hpp"

#include <cstdio>

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
	printf("<?xml version=\"1.0\"?>\n<Program>\n");
	for(auto& node : translation_unit_) {
		node->printXml();
	}
	printf("</Program>\n");
}

Bindings TranslationUnit::printAsm(Bindings bindings, int& label_count) const
{
	for(auto& node : translation_unit_) {
		bindings = node->printAsm(bindings, label_count);
	}

	return bindings;
}

void TranslationUnit::push(Node* external_declaration)
{
	NodePtr node_ptr(external_declaration);
	translation_unit_.push_back(node_ptr);
}
