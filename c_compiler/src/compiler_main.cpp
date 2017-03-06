#include "ast.hpp"

#include <iostream>

int main(int argc, char *argv[])
{
    Node* ast = parseAST();

    ast->printxml();

    ast->printasm();
    
    return 0;
}
