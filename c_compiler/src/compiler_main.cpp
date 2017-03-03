#include "ast.hpp"

#include <iostream>

int main(int argc, char *argv[]) {
    const BaseList* ast = parseAST();

    ast->printasm();
    
    return 0;
}
