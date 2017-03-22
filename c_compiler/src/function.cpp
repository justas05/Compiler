#include "function.hpp"

#include <cstdio>
#include <vector>


// Function definition

Function::Function(const std::string& id, Statement* statement, DeclarationPtr parameter_list)
    : id_(id), statement_(statement), parameter_list_(parameter_list)
{}

void Function::print() const
{
    printf("%s\n", id_.c_str());
    
    if(parameter_list_ != nullptr)
	parameter_list_->print();
    
    if(statement_ != nullptr)
	statement_->print();
}

void Function::printXml() const
{
    printf("<Function id=\"%s\">\n", id_.c_str());

    DeclarationPtr parameter = parameter_list_;
    std::vector<std::string> parameter_vec;
    
    while(parameter != nullptr)
    {
	parameter_vec.push_back(parameter->getId());
	parameter = parameter->getNext();
    }

    for(std::vector<std::string>::reverse_iterator itr = parameter_vec.rbegin();
	itr != parameter_vec.rend(); ++itr)
    {
	printf("<Parameter id=\"%s\" />", (*itr).c_str());	
    }

    
    if(statement_ != nullptr)
	statement_->printXml();

    printf("</Function>\n");
}

VariableStackBindings Function::printAsm(VariableStackBindings bindings, unsigned& label_count) const
{
    VariableStackBindings original_bindings = bindings;
    // Counting all the variables being declared in the function
    unsigned variable_count = 0;
    if(statement_ != nullptr)
	statement_->countVariables(variable_count);

    unsigned max_argument_count = 0;
    // Count the maximum number of arguments
    statement_->countArguments(max_argument_count);

    unsigned max_depth = 0;
    statement_->countExpressionDepth(max_depth);

    if(max_argument_count < 4)
	max_argument_count = 4;

    unsigned parameter_count = 0;
    countParameters(parameter_count);
    
    // This adds 2 to store the frame pointer and the return address
    unsigned memory_needed = 4*(variable_count+max_argument_count+max_depth+parameter_count+2);
    
    // make frame double word aligned
    if(memory_needed % 8 != 0)
	memory_needed += 4;
    
    printf("\t.text\n\t.globl\t%s\n%s:\n\taddiu\t$sp,$sp,-%d\n\tsw\t",
	   id_.c_str(), id_.c_str(), memory_needed);
    printf("$31,%d($sp)\n\tsw\t$fp,%d($sp)\n\tmove\t$fp,$sp\n", memory_needed-4, memory_needed-8);

    // set the stack counter to the right value
    bindings.setStackPosition((max_argument_count+parameter_count)*4);
    bindings.setExpressionStackPosition((max_argument_count+parameter_count+variable_count)*4);

    printParameterAsm(bindings, max_argument_count, memory_needed);
    
    // Prints the asm for the compound statement in the function
    statement_->printAsm(bindings, label_count);
    
    printf("\tmove\t$2,$0\n0:\n\tmove\t$sp,$fp\n\tlw\t$31,%d($sp)\n\tlw\t$fp,%d", memory_needed-4, memory_needed-8);
    printf("($sp)\n\taddiu\t$sp,$sp,%d\n\tjr\t$31\n\tnop\n", memory_needed);

    return original_bindings;
}

void Function::printParameterAsm(VariableStackBindings& bindings, unsigned& stack_offset,
				 unsigned& frame_offset) const
{
    std::vector<DeclarationPtr> parameter_vector;
    DeclarationPtr parameter_list = parameter_list_;

    while(parameter_list != nullptr)
    {
	parameter_vector.push_back(parameter_list);
	parameter_list = parameter_list->getNext();
    }

    for(auto itr = parameter_vector.rbegin(); itr != parameter_vector.rend(); ++itr)
    {
	unsigned i = itr-parameter_vector.rbegin();
	bindings.insertBinding((*itr)->getId(), (*itr)->getType(), (i+stack_offset)*4);
	if(i < 4)
	    printf("\tsw\t$%d,%d($fp)\n", 4+i, (i+stack_offset)*4);
	else
	    printf("\tlw\t$2,%d($fp)\n\tsw\t$2,%d($fp)\n", frame_offset+4*i, (i+stack_offset)*4);
    }
}

void Function::countParameters(unsigned& parameter_count) const
{
    DeclarationPtr parameter_list = parameter_list_;

    while(parameter_list != nullptr)
    {
	parameter_count++;
	parameter_list = parameter_list->getNext();
    }
}
