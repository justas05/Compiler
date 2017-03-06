#include "ast.hpp"


// Function definition

Function::Function(const std::string& _id, Declaration* _parameter_list, Statement* _statement)
    : id(_id), parameter_list(_parameter_list), statement(_statement)
{}

void Function::print() const
{
    std::cout << id << std::endl;
    
    if(parameter_list != nullptr)
	parameter_list->print();
    
    if(statement != nullptr)
	statement->print();
}

void Function::printxml() const
{
    std::cout << "<Function id=\"" << id << "\">" << std::endl;

    Declaration* parameter = parameter_list;
    std::vector<std::string> parameter_vec;
    
    while(parameter != nullptr) {
	if(parameter->getId() != "")
	    parameter_vec.push_back(parameter->getId());
	parameter = parameter->getNext();
    }

    for(std::vector<std::string>::reverse_iterator itr = parameter_vec.rbegin();
	itr != parameter_vec.rend(); ++itr) {
	
	std::cout << "<Parameter id=\"" << *itr << "\" />" << std::endl;	
    }
    
    if(statement != nullptr)
	statement->printxml();

    std::cout << "</Function>" << std::endl;
}

void Function::printasm() const
{
    int32_t count = 0;
    if(statement != nullptr)
	statement->count_variables(count);

    std::cout << id << ": " << count << " variables defined" << std::endl;
}
