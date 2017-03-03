#include "ast.hpp"


// Parameter list definition (used in functions)

ParamList::ParamList() : BaseList() {}

ParamList::ParamList(const Base* _param) : BaseList(_param) {}


// Declarator defintion, used when declaring a variable

Declarator::Declarator(const std::string& _id) : BasePrimitive(_id) {}

void Declarator::printxml() const {
    std::cout << "<Variable id=\"" << id << "\" />" << std::endl;
}


// Parameter class defition

Parameter::Parameter(const std::string& _id) : BasePrimitive(_id) {}

void Parameter::printxml() const {
    std::cout << "<Parameter id=\"" << id << "\" />" << std::endl;
}


// Immediate class definition, used when loading an immediate value

Immediate::Immediate(const int32_t& _imm) : BasePrimitive(), imm(_imm) {}

void Immediate::printasm() const {
    std::cout << "\tli\t$2," << imm << "\n\tsw\t$2,8($fp)" << std::endl;
}
