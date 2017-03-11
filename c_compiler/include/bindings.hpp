#ifndef BINDINGS_HPP
#define BINDINGS_HPP

#include <cstdint>
#include <map>
#include <memory>
#include <string>

class Type;

typedef std::shared_ptr<Type> TypePtr;


// struct containing information on the variable declaration
struct DeclarationData
{
    TypePtr type;
    int32_t stack_position;
};


// stores bindings for the current scope and where they are in the stack
class VariableStackBindings
{
private:
    std::map<std::string, DeclarationData> bindings;
    int32_t stack_counter;
	     
public:
    VariableStackBindings();

    void insertBinding(std::string id, TypePtr type, int32_t stack_position);
    void increaseStackPosition();

    int32_t getCurrentStackPosition() const;
    int32_t getStackPosition(const std::string& id) const;

    bool bindingExists(const std::string& id) const;
};


#endif
