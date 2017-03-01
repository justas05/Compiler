#include "ast.hpp"

#include <iostream>

int main(int argc, char *argv[]) {
    ast_Top *ast = parseAST();

    ast->print();
    
    return 0;
}
