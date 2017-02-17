#ifndef AST_STATEMENT_HPP
#define AST_STATEMENT_HPP

class ast_Statement : public ast_Base {
protected:
    mutable std::vector<const ast_Base*> ast_list;
    
public:
    ast_Statement() {}
    ast_Statement(const ast_Base* _el) {
	ast_list.push_back(_el);
    }

    virtual void print() const {
	for(size_t i = 0; i < ast_list.size(); ++i) {
	    ast_list[i]->print();
	}
    }

    virtual void push(const ast_Base* _var) const {
	ast_list.push_back(_var);
    }
};

class ast_CompoundStatement : public ast_Statement {
public:
    ast_CompoundStatement() : ast_Statement() {}
    ast_CompoundStatement(const ast_Base* _el) : ast_Statement(_el) {}

    virtual void print() const override {
	std::cout << "<Scope>" << std::endl;
	for(size_t i = 0; i < ast_list.size(); ++i) {
	    ast_list[i]->print();
	}
	std::cout << "</Scope>" << std::endl;
    }
};

#endif
