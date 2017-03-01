#include "ast.hpp"

#include <iostream>

int main(int argc, char *argv[]) {
    ast_Top *ast = parseAST();

    std::cout << "<?xml version=\"1.0\"?>" << std::endl << "<Program>" << std::endl;

    ast->print();

    std::cout << "</Program>" << std::endl;
    
    return 0;
}
