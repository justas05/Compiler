#include "ast.hpp"

#include <iostream>

int main(int argc, char *argv[]) {
    ast_Top *ast = parseAST();

    ast->print();

    std::cout << std::endl;
    
    return 0;
}
