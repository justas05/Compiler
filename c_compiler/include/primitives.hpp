#ifndef AST_PRIMITIVES_HPP
#define AST_PRIMITIVES_HPP

#include "ast.hpp"


class Declarator : public BasePrimitive {
public:
    Declarator(const std::string& _id) : BasePrimitive(_id) {}

    virtual void printxml() const {
	std::cout << "<Variable id=\"" << id << "\" />" << std::endl;
    }
};


class Parameter : public BasePrimitive {
public:
    Parameter(const std::string& _id) : BasePrimitive(_id) {}

    virtual void printxml() const {
        std::cout << "<Parameter id=\"" << id << "\" />" << std::endl;
    }
};


#endif
