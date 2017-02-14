#ifndef TOP_AST_HPP
#define TOP_AST_HPP

#include "ast.hpp"

#include <vector>

class ast_Top {
public:
    void print_vec() {
	for(size_t i = 0; i < ast_vec.size(); ++i) {
	    ast_vec[i]->print();
	}
    }

    void push_back(const ast_Base *stmnt) {
	ast_vec.push_back(stmnt);
    }
private:
    std::vector<const ast_Base *> ast_vec;
};

#endif
