#include "ast.hpp"

#include <iostream>

int main(int argc, char *argv[]) {
    
    const ast_Base *ast = parseAST();
    ast->print();

    std::cout << std::endl;
    
    return 0;
}
