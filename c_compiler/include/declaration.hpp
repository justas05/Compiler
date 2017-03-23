#ifndef AST_DECLARATION_HPP
#define AST_DECLARATION_HPP

#include "node.hpp"
#include "type.hpp"
#include "expression.hpp"

#include <memory>

class Declaration;

typedef std::shared_ptr<Declaration> DeclarationPtr;


class Declaration : public Node {
private:
    TypePtr type_;
    std::string id_;
    ExpressionPtr initializer_;
    DeclarationPtr next_declaration_;
    DeclarationPtr next_list_declaration_;
    bool extern_declaration_;
    
public:
    Declaration(const std::string& id = "", Expression* initializer = nullptr);

    virtual void print() const;
    virtual void printXml() const;
    virtual VariableStackBindings printAsm(VariableStackBindings bindings, unsigned& label_count) const;

    VariableStackBindings localAsm(VariableStackBindings bindings, unsigned& label_count) const;

    void linkDeclaration(Declaration* next_declaration);
    void linkListDeclaration(Declaration* next_list_declaration);

    void setType(TypePtr type);
    void setInitializer(Expression* initializer);
    void setExternDeclaration(bool is_extern);

    DeclarationPtr getNext() const;
    DeclarationPtr getNextListItem() const;
    std::string getId() const;
    TypePtr getType() const;
};

#endif
