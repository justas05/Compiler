#include "node.hpp"
#include "bindings.hpp"

#include <iostream>

Node* parseAST();

int main(int argc, char *argv[])
{
    (void)argc, (void)argv;
    std::unique_ptr<Node> ast(parseAST());

    VariableStackBindings bindings;
    unsigned label_count = 0;
     
    ast->printAsm(bindings, label_count);

    return 0;
}
