#include "ast.hpp"

Function::Function(const std::string& _id, const BaseList* _param_list,
		   const BaseNode* _comp_statement)
    : BaseNode(_param_list, _comp_statement), id(_id) {}

void Function::printxml() const {
    std::cout << "<Function id=\"" << id << "\">" << std::endl;
    leftNode->printxml();
    rightNode->printxml();
    std::cout << "</Function>" << std::endl;
}

void Function::printasm() const {
    std::cout << id << ":\n\taddiu\t$sp,$sp,-24\n\tsw\t$fp,20($sp)\n\tmove\t$fp,$sp" << std::endl;
    rightNode->printasm();
    std::cout << "\tmove\t$sp,$fp\n\tlw\t$fp,20($sp)\n\taddiu\t$sp,$sp,24\n\tjr\t$31\n\tnop" << std::endl;
}
