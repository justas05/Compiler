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
    StatementPtr statement_;
    DeclarationPtr parameter_list_;
    
public:
    Function(const std::string& id, Statement* statement, DeclarationPtr parameter_list = nullptr);

    virtual void print() const;
    virtual void printXml() const;
    virtual VariableStackBindings printAsm(VariableStackBindings bindings, int& label_count) const;

    void printParameterAsm(VariableStackBindings& bindings, int& frame_offset) const;
    void countParameters(int& parameter_count) const;
};


#endif
