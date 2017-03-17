#ifndef BINDINGS_HPP
#define BINDINGS_HPP

#include <map>
#include <memory>
#include <string>

class Type;

typedef std::shared_ptr<Type> TypePtr;


// struct containing information on the variable declaration
struct DeclarationData
{
    TypePtr type;
    int stack_position;
};


// stores bindings for the current scope and where they are in the stack
class VariableStackBindings
{
private:
    std::map<std::string, DeclarationData> bindings_;
    int stack_counter_;
    int expression_stack_;
	     
public:
    VariableStackBindings();

    void insertBinding(std::string id, TypePtr type, int stack_position);
    void increaseStackPosition();
    void setStackPosition(int stack_counter);
    void nextExpressionStackPosition();

    int currentStackPosition() const;
    int stackPosition(const std::string& id) const;
    int currentExpressionStackPosition() const;

    bool bindingExists(const std::string& id) const;
};


#endif
