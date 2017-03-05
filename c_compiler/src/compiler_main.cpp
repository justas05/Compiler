#include "ast.hpp"

#include <iostream>

int main(int argc, char *argv[])
{
    TranslationUnit* ast = parseAST();

    ast->printxml();
    
    return 0;
}
