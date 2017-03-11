#ifndef AST_DECLARATION_HPP
#define AST_DECLARATION_HPP

#include "node.hpp"

#include <memory>

class Expression;
class Type;
class Declaration;

typedef std::shared_ptr<Expression> ExpressionPtr;
typedef std::shared_ptr<Type> TypePtr;
typedef std::shared_ptr<Declaration> DeclarationPtr;


class Declaration : public Node {
private:
    TypePtr type;
    std::string id;
    ExpressionPtr init;
    DeclarationPtr next_decl;
    DeclarationPtr list_next_decl;
    
public:
    Declaration(const std::string& _id = "", Expression* _init = nullptr);

    virtual void print() const;
    virtual void printxml() const;
    virtual VariableStackBindings printasm(VariableStackBindings bindings) const;

    void addDeclaration(Declaration* _next_decl);
    void addList(Declaration* _next_decl);

    void setType(Type* _type);

    DeclarationPtr getNext() const;
    DeclarationPtr getNextListItem() const;
    std::string getId() const;
    std::string getType() const;
};

#endif
