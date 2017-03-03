#ifndef AST_BASE_HPP
#define AST_BASE_HPP

#include "ast.hpp"


class Base {
public:
    virtual ~Base() {}

    virtual void print() const = 0;
    virtual void printxml() const = 0;
    virtual void printasm() const = 0;
};


class EmptyNode : public Base {
public:
    EmptyNode() {}
    
    virtual void print() const {}
    virtual void printxml() const {}
    virtual void printasm() const {}
};


class BaseList : public Base {
protected:
    mutable std::vector<const Base*> list;

public:
    BaseList(const Base* _var) {
	push(_var);
    }

    virtual ~BaseList() {
	for(auto& var : list) {
	    delete var;
	}
    }
    
    virtual void print() const {
        for(auto&& declaration : list) {
	    declaration->print();
	}
    }

    virtual void printxml() const {
	for(auto&& declaration : list) {
	    declaration->printxml();
	}
    }

    virtual void printasm() const {
	for(auto&& declaration : list) {
	    declaration->printasm();
	}
    }

    virtual void push(const Base* _var) const {
	list.push_back(_var);
    }
};


class BaseNode : public Base {
protected:
    const Base* leftNode;
    const Base* rightNode;

public:
    BaseNode(const Base* _left = new EmptyNode, const Base* _right = new EmptyNode)
	: leftNode(_left), rightNode(_right) {}

    virtual ~BaseNode() {
	delete leftNode;
	delete rightNode;
    }
    
    virtual void print() const {
	leftNode->print();
	rightNode->print();
    }

    virtual void printxml() const {
	leftNode->printxml();
	rightNode->printxml();
    }

    virtual void printasm() const {
	leftNode->printasm();
	rightNode->printasm();
    }

    virtual const Base* getLeft() const {
	return leftNode;
    }

    virtual const Base* getRight() const {
	return rightNode;
    }
};


class BasePrimitive : public Base {
protected:
    std::string id;
    const Base* type;

public:
    BasePrimitive(const std::string& _id = "", const Base* _type = new EmptyNode)
	: id(_id), type(_type) {}

    virtual ~BasePrimitive() {
	delete type;
    }

    virtual void print() const {}
    virtual void printxml() const {}
    virtual void printasm() const {} 
};


class BaseType : public Base {
public:
    BaseType() {}

    virtual void print() const {}
    virtual void printxml() const {}
    virtual void printasm() const {}
};

    
#endif
