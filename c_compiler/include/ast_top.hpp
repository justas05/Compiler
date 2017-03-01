#ifndef TOP_AST_HPP
#define TOP_AST_HPP

#include "ast.hpp"

#include <vector>

class ast_Top {
public:
    void print() {
	for(size_t i = 0; i < vec.size(); ++i) {
	    vec[i]->print();
	}
    }
 
    void push(const Base *stmnt) {
        vec.push_back(stmnt);
    }
   
private:
    std::vector<const Base *> vec;
};

#endif
