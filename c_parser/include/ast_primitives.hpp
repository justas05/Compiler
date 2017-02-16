#ifndef AST_PRIMITIVES_HPP
#define AST_PRIMITIVES_HPP

#include "ast.hpp"

#include <string>

class ast_Variable : public ast_Base {
private:
    std::string id;
public:
    ast_Variable(const std::string& _id) : id(_id) {}

    virtual void print() const override {
        std::cout << "<Variable id=\"" << id << "\" />" << std::endl;
    }
};

#endif
