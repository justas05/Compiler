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

Bindings Function::printAsm(Bindings bindings, int& label_count) const
{
    Bindings original_bindings = bindings;
    // Counting all the variables being declared in the function
    int variable_count = 0;
    if(statement_ != nullptr)
	statement_->countVariables(variable_count);

    int max_argument_count = 0;
    // Count the maximum number of arguments
    statement_->countArguments(max_argument_count);

    int max_depth = 0;
    statement_->countExpressionDepth(max_depth);

    if(max_argument_count < 4)
	max_argument_count = 4;

    int parameter_count = 0;
    countParameters(parameter_count);
    
    // This adds 2 to store the frame pointer and the return address
    int memory_needed = 4*(variable_count+max_argument_count+max_depth+2);
    
    printf("\t.text\n\t.align\t2\n\t.globl\t%s\n%s:\n\taddiu\t$sp,$sp,-%d\n\tsw\t",
	   id_.c_str(), id_.c_str(), memory_needed);
    printf("$31,%d($sp)\n\tsw\t$fp,%d($sp)\n\tmove\t$fp,$sp\n", memory_needed-4, memory_needed-8);

    // set the stack counter to the right value
    bindings.setStackPosition(max_argument_count*4);
    bindings.setExpressionStackPosition((max_argument_count+variable_count)*4);

    printParameterAsm(bindings, memory_needed);
    
    // Prints the asm for the compound statement in the function
    statement_->printAsm(bindings, label_count);
    
    printf("\tmove\t$2,$0\n0:\n\tmove\t$sp,$fp\n\tlw\t$31,%d($sp)\n\tlw\t$fp,%d", memory_needed-4, memory_needed-8);
    printf("($sp)\n\taddiu\t$sp,$sp,%d\n\tjr\t$31\n\tnop\n", memory_needed);

    auto string_lit_iterator = bindings.getStringLiteralIterator();
    if(string_lit_iterator.first != string_lit_iterator.second)
    {
	printf("\n\t.rdata\n\t.align\t2\n");
	for(auto itr = string_lit_iterator.first; itr != string_lit_iterator.second; ++itr)
	{
	    printf("$%d_string:\n\t.ascii\t\"%s\\000\"", int(itr-string_lit_iterator.first), (*itr).c_str());
	}
    }

    return original_bindings;
}

void Function::printParameterAsm(Bindings& bindings, int& frame_offset) const
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
	int i = itr-parameter_vector.rbegin();
	bindings.insertBinding((*itr)->getId(), (*itr)->getType(), i*4+frame_offset);
	TypePtr parameter_type = (*itr)->getType();
	if(i < 4)
	{    
	    parameter_type->store(4+i, 4*i+frame_offset);
	}
	else
	{
	    if(std::dynamic_pointer_cast<TypeContainer>(parameter_type) != nullptr)
	    {
		if(std::dynamic_pointer_cast<Char>(parameter_type->type()) != nullptr)
		{    
		    printf("\tlw\t$2,%d($fp)\n", i*4+frame_offset);
		    printf("\tsb\t$2,%d($fp)\n", i*4+frame_offset);
		}
		else if(std::dynamic_pointer_cast<Short>(parameter_type->type()) != nullptr)
		{
		    printf("\tlw\t$2,%d($fp)\n", i*4+frame_offset);
		    printf("\tsh\t$2,%d($fp)\n", i*4+frame_offset);
		}
	    }
	}
    }
}

void Function::countParameters(int& parameter_count) const
{
    DeclarationPtr parameter_list = parameter_list_;

    while(parameter_list != nullptr)
    {
	parameter_count++;
	parameter_list = parameter_list->getNext();
    }
}
