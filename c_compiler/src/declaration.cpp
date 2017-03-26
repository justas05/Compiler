#include "declaration.hpp"
#include "bindings.hpp"
#include "type.hpp"
#include "expression.hpp"

#include <cstdio>
#include <vector>


// Declaration definition

Declaration::Declaration(const std::string &id, Expression *initializer)
    : id_(id), initializer_(initializer), extern_declaration_(false)
{}

Declaration::Declaration(const std::string &id, ExpressionPtr initializer)
    :id_(id), initializer_(initializer), extern_declaration_(false)
{}

void Declaration::print() const
{
    if(next_declaration_ != nullptr)
	next_declaration_->print();
    
    if(id_ != "")
	printf("%s\n", id_.c_str());
}

void Declaration::printXml() const
{    
    if(next_declaration_ != nullptr)
	next_declaration_->printXml();

    if(next_list_declaration_ != nullptr)
	next_list_declaration_->printXml();

    if(id_ != "")
	printf("<Variable id=\"%s\" />", id_.c_str());
}

VariableStackBindings Declaration::printAsm(VariableStackBindings bindings, int& label_count) const
{
    (void)label_count;
    if(!extern_declaration_)
    {
	if(initializer_ == nullptr)
	    printf("\t.comm\t%s,4,4\n", id_.c_str());
	else
	    printf("\t.data\n\t.globl\t%s\n%s:\n\t.word\t%d\n",
		   id_.c_str(), id_.c_str(), initializer_->constantFold());
    }
    
    bindings.insertBinding(id_, type_, -1);
    return bindings;
}

VariableStackBindings Declaration::localAsm(VariableStackBindings bindings, int& label_count) const
{
    if(next_declaration_ != nullptr)
	bindings = next_declaration_->localAsm(bindings, label_count);

    if(next_list_declaration_ != nullptr)
	bindings = next_list_declaration_->localAsm(bindings, label_count);
    
    if(id_ != "")
    {
	int stack_position = bindings.currentStackPosition();
	if(initializer_ != nullptr)
	{
	    initializer_->printAsm(bindings, label_count);
	    type_->store(stack_position);
	}
	bindings.insertBinding(id_, type_, stack_position);
	type_->increaseStackPosition(bindings);
    }

    return bindings;
}

void Declaration::countDeclarations(int &declaration_count) const
{
    if(next_declaration_ != nullptr)
	next_declaration_->countDeclarations(declaration_count);

    if(next_list_declaration_ != nullptr)
	next_list_declaration_->countDeclarations(declaration_count);

    ++declaration_count;
}

void Declaration::linkDeclaration(Declaration* next_declaration)
{
    DeclarationPtr decl_ptr(next_declaration);
    next_declaration_ = decl_ptr;
}

void Declaration::linkListDeclaration(Declaration* next_declaration)
{
    DeclarationPtr decl_ptr(next_declaration);
    next_list_declaration_ = decl_ptr;
}

void Declaration::setType(TypePtr type)
{
    type_ = type;
}

void Declaration::setInitializer(Expression* initializer)
{
    ExpressionPtr expression_ptr(initializer);
    initializer_ = expression_ptr;
}

void Declaration::setExternDeclaration(bool is_extern)
{
    extern_declaration_ = is_extern;
}

DeclarationPtr Declaration::getNext() const
{
    return next_declaration_;
}

DeclarationPtr Declaration::getNextListItem() const
{
    return next_list_declaration_;
}

ExpressionPtr Declaration::getInitializer() const
{
    return initializer_;
}

std::string Declaration::getId() const
{
    return id_;
}

TypePtr Declaration::getType() const
{
    return type_;
}


// Array declaration class

ArrayDeclaration::ArrayDeclaration(const std::string &id, ExpressionPtr initializer, const int &size)
    : Declaration(id, initializer), size_(size)
{}

VariableStackBindings ArrayDeclaration::printAsm(VariableStackBindings bindings, int &label_count) const
{
    return bindings;
}

VariableStackBindings ArrayDeclaration::localAsm(VariableStackBindings bindings, int &label_count) const
{
    if(next_declaration_ != nullptr)
	bindings = next_declaration_->localAsm(bindings, label_count);

    if(next_list_declaration_ != nullptr)
	bindings = next_list_declaration_->localAsm(bindings, label_count);

    if(id_ != "")
    {
	int stack_position = bindings.currentStackPosition();
	if(initializer_ != nullptr)
	{
	    ExpressionPtr initializer = initializer_;
	    std::vector<ExpressionPtr> initializer_vector;
	    
	    while(initializer != nullptr)
	    {
		initializer_vector.push_back(initializer);
		initializer = initializer->nextExpression();
	    }

	    for(auto itr = initializer_vector.rbegin(); itr != initializer_vector.rend(); ++itr)
	    {
		int initializer_count = itr-initializer_vector.rbegin();
		(*itr)->printAsm(bindings, label_count);
		type_->store(stack_position+type_->getSize()*initializer_count);
	    }
	}
	
	bindings.insertBinding(id_, type_, stack_position);
	type_->increaseStackPosition(bindings);
    }
    
    return bindings;
}

void ArrayDeclaration::countDeclarations(int &declaration_count) const
{
    if(next_declaration_ != nullptr)
	next_declaration_->countDeclarations(declaration_count);

    if(next_list_declaration_ != nullptr)
	next_list_declaration_->countDeclarations(declaration_count);

    declaration_count += size_;
}
