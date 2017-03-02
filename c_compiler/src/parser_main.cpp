#include "ast.hpp"

#include <iostream>

int main(int argc, char *argv[]) {
    const Base *ast = parseAST();

    ast->print();
    
    return 0;
}
