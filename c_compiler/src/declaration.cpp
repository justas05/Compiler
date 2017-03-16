#include "declaration.hpp"
#include "bindings.hpp"
#include "type.hpp"
#include "expression.hpp"

#include <iostream>


// Declaration definition

Declaration::Declaration(const std::string& id, Expression* initializer)
    : id_(id), initializer_(initializer)
{}

void Declaration::print() const
{
    if(next_declaration_ != nullptr)
	next_declaration_->print();
    
    if(id_ != "")
	std::cout << id_ << std::endl;
}

void Declaration::printXml() const
{    
    if(next_declaration_ != nullptr)
	next_declaration_->printXml();

    if(next_list_declaration_ != nullptr) {
	next_list_declaration_->printXml();
    }

    if(id_ != "")
	std::cout << "<Variable id=\""<< id_ << "\" />" << std::endl;
}

VariableStackBindings Declaration::printAsm(VariableStackBindings bindings) const
{
    // if(init == nullptr)
    // 	std::cout << "\t.comm\t" << id << ",4,4" << std::endl;
    // else {
    // 	std::cout << "\t.data\n\t.globl\t" << id << std::endl;
    // 	std::cout << id << ":\n\t.word\t" << std::endl;
    // }

    // return bindings;

    if(next_declaration_ != nullptr)
	bindings = next_declaration_->printAsm(bindings);

    if(next_list_declaration_ != nullptr)
	bindings = next_list_declaration_->printAsm(bindings);
    
    if(id_ != "") {
	if(initializer_ != nullptr)
	    initializer_->printAsm(bindings);
	else
	    std::cout << "\tmove\t$2,$0" << std::endl;

	int32_t stack_position = bindings.currentStackPosition();

	std::cout << "\tsw\t$2," << stack_position << "($fp)" << std::endl;

	bindings.insertBinding(id_, type_, stack_position);

	bindings.increaseStackPosition();
    }
    
    return bindings;
}

void Declaration::linkDeclaration(Declaration* next_declaration)
{
    DeclarationPtr decl_ptr(next_declaration);
    next_declaration_ = decl_ptr;
}

void Declaration::linkListDeclaration(Declaration* next_declaration)
{
    DeclarationPtr decl_ptr(next_declaration);
    next_list_declaration_ = decl_ptr;
}

void Declaration::setType(Type* type)
{
    TypePtr type_ptr(type);
    type_ = type_ptr;
}

DeclarationPtr Declaration::getNext() const
{
    return next_declaration_;
}

DeclarationPtr Declaration::getNextListItem() const
{
    return next_list_declaration_;
}

std::string Declaration::getId() const
{
    return id_;
}

TypePtr Declaration::getType() const
{
    return type_;
}
