#ifndef BINDINGS_HPP
#define BINDINGS_HPP

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class Type;

typedef std::shared_ptr<Type> TypePtr;

// struct containing information on the variable declaration
struct DeclarationData
{
	TypePtr type;
	int stack_position;
	std::vector<int> array_sizes;
};

// stores bindings for the current scope and where they are in the stack
class Bindings
{
private:
	static std::vector<std::string> string_literals;
    
	std::unordered_map<std::string, DeclarationData> bindings_;
	std::string break_label_;
	std::string continue_label_;
	int stack_counter_;
	int expression_stack_;
public:
	Bindings();

	void insertBinding(const std::string &id, const TypePtr &type, const int &stack_position);
	void insertBinding(const std::string &id, const TypePtr &type, const int &stack_position, const std::vector<int> array_sizes);
	int insertStringLiteral(const std::string &string_literal);
	void increaseStackPosition();
	void increaseStackPosition(const int &position);
	void setStackPosition(const int &stack_counter);
	void nextExpressionStackPosition();
	void setExpressionStackPosition(const int &stack_counter);

	TypePtr getType(const std::string &id) const;
	const std::vector<int> &getArraySizes(const std::string &id) const;

	std::string breakLabel();
	std::string breakLabel(const std::string &label);
	std::string continueLabel();
	std::string continueLabel(const std::string &label);

	int currentStackPosition() const;
	int stackPosition(const std::string &id) const;
	int currentExpressionStackPosition() const;

	std::pair<std::vector<std::string>::const_iterator, std::vector<std::string>::const_iterator>
	getStringLiteralIterator() const;

	bool bindingExists(const std::string &id) const;
};


#endif
