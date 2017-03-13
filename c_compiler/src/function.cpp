#include "function.hpp"

#include <iostream>
#include <vector>


// Function definition

Function::Function(const std::string& id, Declaration* parameter_list, Statement* statement)
    : id_(id), parameter_list_(parameter_list), statement_(statement)
{}

void Function::print() const
{
    std::cout << id_ << std::endl;
    
    if(parameter_list_ != nullptr)
	parameter_list_->print();
    
    if(statement_ != nullptr)
	statement_->print();
}

void Function::printXml() const
{
    std::cout << "<Function id=\"" << id_ << "\">" << std::endl;

    DeclarationPtr parameter = parameter_list_;
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
    
    if(statement_ != nullptr)
	statement_->printXml();

    std::cout << "</Function>" << std::endl;
}

VariableStackBindings Function::printAsm(VariableStackBindings bindings) const
{
    // Counting all the variables being declared in the function
    unsigned count = 0;
    if(statement_ != nullptr)
	statement_->countVariables(count);
    
    // This includes the space for the old frame counter and (return address)?
    unsigned memory_needed = 4*count+8;

    std::cout << "\t.text\n\t.globl\t" << id_ << std::endl << id_ << ":\n\taddiu\t$sp,$sp,-"
	      << memory_needed << "\n\tsw\t$fp," << memory_needed-4 << "($sp)\n\tmove\t$fp,$sp"
	      << std::endl;

    // TODO print asm for parameters

    // Prints the asm for the compound statement in the function
    statement_->printAsm(bindings);

    std::cout << "\tmove\t$sp,$fp\n\tlw\t$fp," << memory_needed-4 << "($sp)\n\taddiu\t$sp,$sp,"
	      << memory_needed << "\n\tjr\t$31\n\tnop" << std::endl;

    return bindings;
}
