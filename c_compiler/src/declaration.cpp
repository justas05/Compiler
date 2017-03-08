#include "ast.hpp"


// Declaration definition

Declaration::Declaration(const std::string& _id)
    : id(_id)
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

void Declaration::printasm() const
{
    if(init == nullptr)
	std::cout << "\t.comm\t" << id << ",4,4" << std::endl;
    else {
	std::cout << "\t.data\n\t.globl\t" << id << std::endl;
	std::cout << id << ":\n\t.word\t" << std::endl;
    }
}

void Declaration::addDeclaration(Declaration* _next_decl)
{
    next_decl = _next_decl;
}

void Declaration::addList(Declaration* _next_decl)
{
    list_next_decl = _next_decl;
}

void Declaration::setType(Type* _type)
{
    type = _type;
}

Declaration* Declaration::getNext() const
{
    return next_decl;
}

Declaration* Declaration::getNextListItem() const
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
