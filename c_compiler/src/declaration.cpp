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
{}

void Declaration::addDeclaration(Declaration* _next_decl)
{
    next_decl = _next_decl;
}

void Declaration::addList(Declaration* _next_decl)
{
    list_next_decl = _next_decl;
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
