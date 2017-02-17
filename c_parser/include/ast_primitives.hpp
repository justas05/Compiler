#ifndef AST_PRIMITIVES_HPP
#define AST_PRIMITIVES_HPP

#include "ast.hpp"

#include <string>

class ast_Variable : public ast_Base {
private:
    std::string id;
public:
    ast_Variable(const std::string& _id) : id(_id) {}

    virtual void print() const {
        std::cout << "<Variable id=\"" << id << "\" />" << std::endl;
    }

    virtual void push(const ast_Base* var) const {
	std::cerr << "Error: Can't call this function on this class" << std::endl;
	(void)var;
    }
};

class ast_Parameter : public ast_Base {
private:
    std::string id;
public:
    ast_Parameter(const std::string& _id) : id(_id) {}

    virtual void print() const {
        std::cout << "<Parameter id=\"" << id << "\" />" << std::endl;
    }

    virtual void push(const ast_Base* var) const {
        std::cerr << "Error: Can't call this function on this class" << std::endl;
	(void)var;
    }
};

#endif
