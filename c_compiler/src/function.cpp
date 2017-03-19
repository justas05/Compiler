#include "function.hpp"

#include <iostream>
#include <vector>


// Function definition

Function::Function(const std::string& id, Statement* statement, DeclarationPtr parameter_list)
    : id_(id), statement_(statement), parameter_list_(parameter_list)
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

VariableStackBindings Function::printAsm(VariableStackBindings bindings, unsigned& label_count) const
{
    // Counting all the variables being declared in the function
    unsigned variable_count = 0;
    if(statement_ != nullptr)
	statement_->countVariables(variable_count);

    unsigned max_argument_count = 0;
    // Count the maximum number of arguments
    statement_->countArguments(max_argument_count);

    unsigned max_depth = 0;
    statement_->countExpressionDepth(max_depth);

    std::cout << "# max depth: " << max_depth << "\n";

    if(max_argument_count < 4)
	max_argument_count = 4;

    unsigned parameter_count = 0;
    countParameters(parameter_count);
    
    // This adds 2 to store the frame pointer and the return address
    unsigned memory_needed = 4*(variable_count+max_argument_count+max_depth+parameter_count+2);
    
    // make frame double word aligned
    if(memory_needed % 8 != 0)
	memory_needed += 4;

    std::cout << "\t.text\n\t.globl\t" << id_ << "\n" << id_ << ":\n\taddiu\t$sp,$sp,-"
	      << memory_needed << "\n\tsw\t$31," << memory_needed-4 << "($sp)\n" << "\tsw\t$fp,"
	      << memory_needed-8 << "($sp)\n\tmove\t$fp,$sp\n";


    // set the stack counter to the right value
    bindings.setStackPosition((max_argument_count+parameter_count)*4);
    bindings.setExpressionStackPosition((max_argument_count+parameter_count+variable_count)*4);

    printParameterAsm(bindings, max_argument_count, memory_needed);
    
    // Prints the asm for the compound statement in the function
    statement_->printAsm(bindings, label_count);

    std::cout << "0:\n\tmove\t$sp,$fp\n\tlw\t$31," << memory_needed-4 << "($sp)\n\tlw\t$fp,"
	      << memory_needed-8 << "($sp)\n\taddiu\t$sp,$sp," << memory_needed
	      << "\n\tjr\t$31\n\tnop\n";

    return bindings;
}

void Function::printParameterAsm(VariableStackBindings& bindings, unsigned& stack_offset,
				 unsigned& frame_offset) const
{
    std::vector<DeclarationPtr> parameter_vector;
    DeclarationPtr parameter_list = parameter_list_;

    while(parameter_list != nullptr) {
	parameter_vector.push_back(parameter_list);
	parameter_list = parameter_list->getNext();
    }

    for(auto itr = parameter_vector.rbegin(); itr != parameter_vector.rend(); ++itr) {
	unsigned i = itr-parameter_vector.rbegin();
	bindings.insertBinding((*itr)->getId(), (*itr)->getType(), (i+stack_offset)*4);
	if(i < 4)
	    std::cout << "\tsw\t$" << 4+i << "," << (i+stack_offset)*4 << "($fp)\n";
	else
	    std::cout << "\tlw\t$2," << frame_offset + 4*i << "($fp)\n\tsw\t$2," << (i+stack_offset)*4
		      << "($fp)\n";
    }
}

void Function::countParameters(unsigned& parameter_count) const
{
    DeclarationPtr parameter_list = parameter_list_;

    while(parameter_list != nullptr) {
	parameter_count++;
	parameter_list = parameter_list->getNext();
    }
}
