#include "node.hpp"
#include "bindings.hpp"

#include <cstdio>
#include <string>

Node* parseAST();

int main(int, char**)
{
    try
    {
	std::unique_ptr<Node> ast(parseAST());
	VariableStackBindings bindings;
	unsigned label_count = 0;
	ast->printAsm(bindings, label_count);	
    }
    catch(std::string error_msg)
    {
	fprintf(stderr, "%s\n", error_msg.c_str());
    }
    catch(...)
    {
	fprintf(stderr, "Error : Exception thrown\n");
    }

    return 0;
}
