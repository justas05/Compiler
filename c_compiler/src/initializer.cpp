#include "ast.hpp"


// Initializer definition

Initializer::Initializer() {}

void Initializer::print() const {}

void Initializer::printxml() const {}

void Initializer::printasm() const {}


// Integer definition

Integer::Integer() : Initializer() {}


// String Literal definition

StringLiteral::StringLiteral() : Initializer() {}
