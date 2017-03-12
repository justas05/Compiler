#include "bindings.hpp"

#include <iostream>


// VariableStackBindings definition

VariableStackBindings::VariableStackBindings()
    : stack_counter(4), current_register(2)
{}

void VariableStackBindings::insertBinding(std::string id, TypePtr type, int32_t stack_position)
{
    DeclarationData decl_data;
    decl_data.type = type;
    decl_data.stack_position = stack_position;

    bindings.insert(std::pair<std::string, DeclarationData>(id, decl_data));
}

void VariableStackBindings::increaseStackPosition()
{
    stack_counter += 4;
}

void VariableStackBindings::resetRegister()
{
    current_register = 2;
}

void VariableStackBindings::increaseRegister()
{
    if(current_register == 15)
	current_register = 24;
    else if(current_register == 25)
	std::cerr << "Error : cannot allocate more registers" << std::endl;
    else
	current_register++;
}

int32_t VariableStackBindings::getCurrentStackPosition() const
{
    return stack_counter;
}

int32_t VariableStackBindings::getStackPosition(const std::string &id) const
{
    auto binding = bindings.find(id);

    if(binding != bindings.end())
	return (*binding).second.stack_position;

    else return 0;
}

int8_t VariableStackBindings::getCurrentRegister() const
{
    return current_register;
}

bool VariableStackBindings::bindingExists(const std::string &id) const
{
    auto binding = bindings.find(id);

    if(binding == bindings.end())
	return false;

    else
	return true;
}
