#include "ast.hpp"

#include <iostream>

int main(int argc, char *argv[])
{
    Node* ast = parseAST();

    VariableStackBindings bindings;
    int32_t var_count;
    
    ast->printasm(bindings, var_count);
    
    return 0;
}
