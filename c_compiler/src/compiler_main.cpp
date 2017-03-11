#include "node.hpp"
#include "bindings.hpp"

#include <iostream>

Node* parseAST();

int main(int argc, char *argv[])
{
    std::unique_ptr<Node> ast(parseAST());

    VariableStackBindings bindings;
     
    ast->printasm(bindings);
    
    return 0;
}
