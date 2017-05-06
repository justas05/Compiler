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
		Bindings bindings;
		int label_count = 0;
		ast->printAsm(bindings, label_count);	
	}
	catch(const std::exception& e)
	{
		fprintf(stderr, "%s\n", e.what());
	}
	catch(...)
	{
		fprintf(stderr, "Error : Exception thrown\n");
	}

	return 0;
}
