#include "node.hpp"
#include "bindings.hpp"

#include <iostream>

Node* parseAST();

int main(int argc, char *argv[])
{
    Node* ast = parseAST();

    VariableStackBindings bindings;
     
    ast->printasm(bindings);
    
    return 0;
}
