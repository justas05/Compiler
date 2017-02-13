#ifndef AST_DECLARATION_HPP
#define AST_DECLARATION_HPP

#include "ast.hpp"

class ast_Declaration : public ast_Base {
private:
    const std::string id;
public:
    ast_Declaration(const std::string& _id) : id(_id) {}

    virtual void print() const override {
	std::cout << id;
    }
};

#endif
