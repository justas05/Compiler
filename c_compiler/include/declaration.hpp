#ifndef AST_DECLARATION_HPP
#define AST_DECLARATION_HPP

#include "ast.hpp"

// Declaration that holds a list of declarations

class Declaration : public BaseNode {
public:
    Declaration(const Base* _var) : BaseNode(_var) {}
};

class DeclarationList : public BaseList {
public:
    DeclarationList(const Base* _var) : BaseList(_var) {}
};

class InitDeclaratorList : public BaseList {
public:
    InitDeclaratorList(const Base* _var) : BaseList(_var) {}
};

#endif
