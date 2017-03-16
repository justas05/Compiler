#ifndef AST_FUNCTION_HPP
#define AST_FUNCTION_HPP

#include "bindings.hpp"
#include "declaration.hpp"
#include "node.hpp"
#include "statement.hpp"
#include "type.hpp"

#include <memory>
#include <string>

class Function;

typedef std::shared_ptr<Function> FunctionPtr;


class Function : public Node {
protected:
    TypePtr type_;
    std::string id_;
    DeclarationPtr parameter_list_;
    StatementPtr statement_;
    
public:
    Function(const std::string& id, Declaration* parameter_list, Statement* statement);

    virtual void print() const;
    virtual void printXml() const;
    virtual VariableStackBindings printAsm(VariableStackBindings bindings) const;
};


#endif
