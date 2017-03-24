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
    std::string break_label_;
    std::string continue_label_;
    int stack_counter_;
    int expression_stack_;
	     
public:
    VariableStackBindings();

    void insertBinding(const std::string &id, TypePtr type, const int &stack_position);
    void increaseStackPosition();
    void increaseStackPosition(const int &position);
    void setStackPosition(const int &stack_counter);
    void nextExpressionStackPosition();
    void setExpressionStackPosition(const int &stack_counter);

    TypePtr getType(const std::string &id) const;

    std::string breakLabel();
    std::string breakLabel(const std::string &label);
    std::string continueLabel();
    std::string continueLabel(const std::string &label);

    int currentStackPosition() const;
    int stackPosition(const std::string &id) const;
    int currentExpressionStackPosition() const;

    bool bindingExists(const std::string &id) const;
};


#endif
