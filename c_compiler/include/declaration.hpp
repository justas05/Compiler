#ifndef AST_DECLARATION_HPP
#define AST_DECLARATION_HPP

#include "node.hpp"
#include "type.hpp"
#include "expression.hpp"

#include <memory>

class Declaration;

typedef std::shared_ptr<Declaration> DeclarationPtr;


class Declaration : public Node {
protected:
    TypePtr type_;
    std::string id_;
    ExpressionPtr initializer_;
    DeclarationPtr next_declaration_;
    DeclarationPtr next_list_declaration_;
    bool extern_declaration_;
public:
    Declaration(const std::string &id = "", Expression *initializer = nullptr);
    Declaration(const std::string &id, ExpressionPtr initializer);

    virtual void print() const;
    virtual void printXml() const;
    virtual Bindings printAsm(Bindings bindings, int &label_count) const;
    virtual Bindings localAsm(Bindings bindings, int &label_count) const;
    virtual void countDeclarations(int &declaration_count) const;    

    void linkDeclaration(Declaration *next_declaration);
    void linkListDeclaration(Declaration *next_list_declaration);

    void setType(TypePtr type);
    void setInitializer(Expression *initializer);
    void setExternDeclaration(bool is_extern);

    DeclarationPtr getNext() const;
    DeclarationPtr getNextListItem() const;
    ExpressionPtr getInitializer() const;
    std::string getId() const;
    TypePtr getType() const;
};

class ArrayDeclaration : public Declaration
{
private:
    int size_;
public:
    ArrayDeclaration(const std::string &id = "", ExpressionPtr initializer = nullptr, const int &size = 0);
    
    virtual Bindings printAsm(Bindings bindings, int &label_count) const;
    virtual Bindings localAsm(Bindings bindings, int &label_count) const;
    virtual void countDeclarations(int &declaration_count) const;    
};

#endif
