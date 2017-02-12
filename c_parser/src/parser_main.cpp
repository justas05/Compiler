#include "ast.hpp"

#include <iostream>

int main(int argc, char *argv[]) {
    
    const Expression *ast = parseAST();
    ast->print();
    
    return 0;
}
