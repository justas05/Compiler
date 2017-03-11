#ifndef AST_FUNCTION_HPP
#define AST_FUNCTION_HPP

#include "node.hpp"

#include <memory>

class Declaration;
class Statement;
class Type;
class Function;

typedef std::shared_ptr<Declaration> DeclarationPtr;
typedef std::shared_ptr<Statement> StatementPtr;
typedef std::shared_ptr<Type> TypePtr;
typedef std::shared_ptr<Function> FunctionPtr;


class Function : public Node {
protected:
    TypePtr type;
    std::string id;
    DeclarationPtr parameter_list;
    StatementPtr statement;
    
public:
    Function(const std::string& _id, Declaration* _parameter_list, Statement* _statement);

    virtual void print() const;
    virtual void printxml() const;
    virtual VariableStackBindings printasm(VariableStackBindings bindings) const;
};


#endif
