#include "ast.hpp"


// Translation Unit definition

TranslationUnit::TranslationUnit(Node* decl) {
    push(decl);
}

void TranslationUnit::print() const {
    for(auto& node : translation_unit) {
	node->print();
    }
}

void TranslationUnit::printxml() const {
    std::cout << "<?xml version=\"1.0\"?>\n<Program>" << std::endl;
    for(auto& node : translation_unit) {
	node->printxml();
    }
    std::cout << "</Program>" << std::endl;
}

void TranslationUnit::printasm() const {
    for(auto& node : translation_unit) {
	node->printasm();
    }
}

void TranslationUnit::push(Node* decl) {
    translation_unit.push_back(decl);
}
