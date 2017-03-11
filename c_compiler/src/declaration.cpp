#include "declaration.hpp"
#include "bindings.hpp"
#include "type.hpp"
#include "expression.hpp"

#include <iostream>


// Declaration definition

Declaration::Declaration(const std::string& _id, Expression* _init)
    : id(_id), init(_init)
{}

void Declaration::print() const
{
    if(next_decl != nullptr)
	next_decl->print();
    
    if(id != "")
	std::cout << id << std::endl;
}

void Declaration::printxml() const
{    
    if(next_decl != nullptr)
	next_decl->printxml();

    if(list_next_decl != nullptr) {
	list_next_decl->printxml();
    }

    if(id != "")
	std::cout << "<Variable id=\""<< id << "\" />" << std::endl;
}

VariableStackBindings Declaration::printasm(VariableStackBindings bindings) const
{
    // if(init == nullptr)
    // 	std::cout << "\t.comm\t" << id << ",4,4" << std::endl;
    // else {
    // 	std::cout << "\t.data\n\t.globl\t" << id << std::endl;
    // 	std::cout << id << ":\n\t.word\t" << std::endl;
    // }

    // return bindings;

    if(next_decl != nullptr)
	bindings = next_decl->printasm(bindings);

    if(list_next_decl != nullptr)
	bindings = list_next_decl->printasm(bindings);
    
    if(id != "") {
	if(init != nullptr)
	    init->printasm(bindings);
	else
	    std::cout << "\tmove\t$2,$0" << std::endl;

	int32_t stack_position = bindings.getCurrentStackPosition();

	std::cout << "\tsw\t$2," << stack_position << "($fp)" << std::endl;

	bindings.insertBinding(id, type, stack_position);

	bindings.increaseStackPosition();
    }
    
    return bindings;
}

void Declaration::addDeclaration(Declaration* _next_decl)
{
    DeclarationPtr decl_ptr(_next_decl);
    next_decl = decl_ptr;
}

void Declaration::addList(Declaration* _next_decl)
{
    DeclarationPtr decl_ptr(_next_decl);
    list_next_decl = decl_ptr;
}

void Declaration::setType(Type* _type)
{
    TypePtr type_ptr(_type);
    type = type_ptr;
}

DeclarationPtr Declaration::getNext() const
{
    return next_decl;
}

DeclarationPtr Declaration::getNextListItem() const
{
    return list_next_decl;
}

std::string Declaration::getId() const
{
    return id;
}

std::string Declaration::getType() const
{
    return type->getType();
}
