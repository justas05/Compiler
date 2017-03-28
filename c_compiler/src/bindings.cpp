#include "bindings.hpp"

std::vector<std::string> Bindings::string_literals;

// Bindings definition

Bindings::Bindings()
    : break_label_(""), continue_label_(""), stack_counter_(0), expression_stack_(16)
{}

void Bindings::insertBinding(const std::string &id,const TypePtr &type, const int &stack_position)
{
    auto binding = bindings_.find(id);

    if(binding == bindings_.end())
    {
	DeclarationData decl_data;
	decl_data.type = type;
	decl_data.stack_position = stack_position;
	bindings_.insert(std::make_pair(id, decl_data));
    }
    else
    {
	(*binding).second.stack_position = stack_position;
	(*binding).second.type = type;
    }
}

void Bindings::insertBinding(const std::string &id, const TypePtr &type, const int &stack_position, const std::vector<int> array_sizes)
{
    auto binding = bindings_.find(id);

    if(binding == bindings_.end())
    {
	DeclarationData decl_data;
	decl_data.type = type;
	decl_data.stack_position = stack_position;
	decl_data.array_sizes = array_sizes;
	bindings_.insert(std::make_pair(id, decl_data));
    }
    else
    {
	(*binding).second.stack_position = stack_position;
	(*binding).second.type = type;
	(*binding).second.array_sizes = array_sizes;
    }
}

int Bindings::insertStringLiteral(const std::string &string_literal)
{
    string_literals.push_back(string_literal);
    return (int)string_literals.size()-1;
}

void Bindings::increaseStackPosition()
{
    stack_counter_ += 4;
}

void Bindings::increaseStackPosition(const int &position)
{
    stack_counter_ += position;
}

void Bindings::setStackPosition(const int &stack_counter)
{
    stack_counter_ = stack_counter;
}

void Bindings::nextExpressionStackPosition()
{
    expression_stack_ += 4;
}

void Bindings::setExpressionStackPosition(const int &stack_counter)
{
    expression_stack_ = stack_counter;
}

TypePtr Bindings::getType(const std::string &id) const
{
    auto binding = bindings_.find(id);
    return (*binding).second.type;
}

const std::vector<int> &Bindings::getArraySizes(const std::string &id) const
{
    auto binding = bindings_.find(id);
    return (*binding).second.array_sizes;
}

std::string Bindings::breakLabel()
{
    return break_label_;
}

std::string Bindings::breakLabel(const std::string &label)
{
    break_label_ = label;
    return break_label_;
}

std::string Bindings::continueLabel()
{
    return continue_label_;
}

std::string Bindings::continueLabel(const std::string &label)
{
    continue_label_ = label;
    return continue_label_;
}

int Bindings::currentStackPosition() const
{
    return stack_counter_;
}

int Bindings::stackPosition(const std::string &id) const
{
    auto binding = bindings_.find(id);

    if(binding != bindings_.end())
	return (*binding).second.stack_position;

    else return 0;
}

int Bindings::currentExpressionStackPosition() const
{
    return expression_stack_;
}

std::pair<std::vector<std::string>::const_iterator, std::vector<std::string>::const_iterator>
Bindings::getStringLiteralIterator() const
{
    return std::make_pair(string_literals.begin(), string_literals.end());
}

bool Bindings::bindingExists(const std::string &id) const
{
    auto binding = bindings_.find(id);

    if(binding == bindings_.end())
	return false;

    else
	return true;
}
