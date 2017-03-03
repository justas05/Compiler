#include "ast.hpp"


// Expression definition

Expression::Expression(const Base* _expr)
    : BaseNode(_expr) {}

void Expression::printasm() const {
    leftNode->printasm();
}
