#ifndef AST_BASE_HPP
#define AST_BASE_HPP

#include "ast.hpp"


class Node {
public:
    virtual ~Node() {}

    virtual void print() const = 0;
    virtual void printxml() const = 0;
    virtual void printasm() const = 0;
};


class EmptyNode : public Node {
public:
    EmptyNode() {}
    
    virtual void print() const {}
    virtual void printxml() const {}
    virtual void printasm() const {}
};


class NodeList : public Node {
protected:
    mutable std::vector<const Node*> list;

public:
    NodeList(const Node* _var) {
	push(_var);
    }

    virtual ~NodeList() {
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

    virtual void push(const Node* _var) const {
	list.push_back(_var);
    }
};


class NodeNode : public Node {
protected:
    const Node* leftNode;
    const Node* rightNode;

public:
    NodeNode(const Node* _left = new EmptyNode, const Node* _right = new EmptyNode)
	: leftNode(_left), rightNode(_right) {}

    virtual ~NodeNode() {
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

    virtual const Node* getLeft() const {
	return leftNode;
    }

    virtual const Node* getRight() const {
	return rightNode;
    }
};


class NodePrimitive : public Node {
protected:
    std::string id;
    const Node* type;

public:
    NodePrimitive(const std::string& _id = "", const Node* _type = new EmptyNode)
	: id(_id), type(_type) {}

    virtual ~NodePrimitive() {
	delete type;
    }

    virtual void print() const {}
    virtual void printxml() const {}
    virtual void printasm() const {} 
};


class NodeType : public Node {
public:
    NodeType() {}

    virtual void print() const {}
    virtual void printxml() const {}
    virtual void printasm() const {}
};

    
#endif
