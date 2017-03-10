#include "bindings.hpp"


// VariableStackBindings definition

VariableStackBindings::VariableStackBindings()
    : stack_counter(4)
{}

void VariableStackBindings::insertBinding(std::string id, Type* type, int32_t stack_position)
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

bool VariableStackBindings::bindingExists(const std::string &id) const
{
    auto binding = bindings.find(id);

    if(binding == bindings.end())
	return false;

    else
	return true;
}
