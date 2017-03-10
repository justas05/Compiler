#include "function.hpp"
#include "statement.hpp"
#include "declaration.hpp"
#include "bindings.hpp"

#include <iostream>
#include <vector>


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

VariableStackBindings Function::printasm(VariableStackBindings bindings) const
{
    // Counting all the variables being declared in the function
    int32_t count = 0;
    if(statement != nullptr)
	statement->count_variables(count);

    // This includes the space for the old frame counter and (return address)?
    int32_t memory_needed = 4*count+8;

    std::cout << "\t.text\n\t.globl\t" << id << std::endl << id << ":\n\taddiu\t$sp,$sp,-"
	      << memory_needed << "\n\tsw\t$fp," << memory_needed-4 << "($sp)\n\tmove\t$fp,$sp"
	      << std::endl;

    // TODO print asm for parameters

    // Prints the asm for the compound statement in the function
    statement->printasm(bindings);

    std::cout << "\tmove\t$sp,$fp\n\tlw\t$fp," << memory_needed-4 << "($sp)\n\taddiu\t$sp,$sp,"
	      << memory_needed << "\n\tjr\t$31\n\tnop" << std::endl;

    return bindings;
}
