#ifndef AST_DECLARATION_HPP
#define AST_DECLARATION_HPP

#include "ast.hpp"

class Declaration : public Expression {
private:
    const std::string id;
public:
    Declaration(const std::string& _id) : id(_id) {}

    virtual void print() const override {
	std::cout << id;
    }
};

#endif
