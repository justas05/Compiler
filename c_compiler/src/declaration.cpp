#include "declaration.hpp"
#include "bindings.hpp"
#include "type.hpp"
#include "expression.hpp"

#include <cstdio>
#include <vector>


// Declaration definition

Declaration::Declaration(Expression *initializer)
    : initializer_(initializer), extern_declaration_(false)
{}

Declaration::Declaration(ExpressionPtr initializer)
    : initializer_(initializer), extern_declaration_(false)
{}

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

TypePtr Declaration::getType() const
{
    return type_;
}


// IdentifierDeclaration definition

IdentifierDeclaration::IdentifierDeclaration(const std::string &id, Expression *initializer)
    : Declaration(initializer), id_(id)
{}

IdentifierDeclaration::IdentifierDeclaration(const std::string &id, ExpressionPtr initializer)
    : Declaration(initializer), id_(id)
{}

void IdentifierDeclaration::print() const
{
    if(next_declaration_ != nullptr)
	next_declaration_->print();
    
    if(id_ != "")
	printf("%s\n", id_.c_str());
}

void IdentifierDeclaration::printXml() const
{    
    if(next_declaration_ != nullptr)
	next_declaration_->printXml();

    if(next_list_declaration_ != nullptr)
	next_list_declaration_->printXml();

    if(id_ != "")
	printf("<Variable id=\"%s\" />", id_.c_str());
}

Bindings IdentifierDeclaration::printAsm(Bindings bindings, int& label_count) const
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

Bindings IdentifierDeclaration::localAsm(Bindings bindings, int& label_count) const
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

void IdentifierDeclaration::countDeclarations(int &declaration_count) const
{
    if(next_declaration_ != nullptr)
	next_declaration_->countDeclarations(declaration_count);

    if(next_list_declaration_ != nullptr)
	next_list_declaration_->countDeclarations(declaration_count);

    ++declaration_count;
}

std::string IdentifierDeclaration::getId() const
{
    return id_;
}


// Array declaration class

ArrayDeclaration::ArrayDeclaration(Declaration *declarator, ExpressionPtr initializer, const int &size)
    : Declaration(initializer), size_(size), declarator_(declarator)
{}

void ArrayDeclaration::print() const
{
    if(next_declaration_ != nullptr)
	next_declaration_->print();
    if(next_list_declaration_ != nullptr)
	next_list_declaration_->print();
    
    printf("Array Declaration\n");
    declarator_->print();
}

void ArrayDeclaration::printXml() const
{
    if(next_declaration_ != nullptr)
	next_declaration_->printXml();
    if(next_list_declaration_ != nullptr)
	next_list_declaration_->printXml();
    declarator_->printXml();
}

Bindings ArrayDeclaration::printAsm(Bindings bindings, int &label_count) const
{
    if(next_declaration_ != nullptr)
	bindings = next_declaration_->printAsm(bindings, label_count);
    if(next_list_declaration_ != nullptr)
	bindings = next_list_declaration_->printAsm(bindings, label_count);
    bindings = declarator_->printAsm(bindings, label_count);
    
    return bindings;
}

Bindings ArrayDeclaration::localAsm(Bindings bindings, int &label_count) const
{
    if(next_declaration_ != nullptr)
	bindings = next_declaration_->localAsm(bindings, label_count);
    if(next_list_declaration_ != nullptr)
	bindings = next_list_declaration_->localAsm(bindings, label_count);

    if(getId() != "")
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
	
	bindings.insertBinding(getId(), type_, stack_position);
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

    std::shared_ptr<ArrayDeclaration> next_array;
    next_array = std::dynamic_pointer_cast<ArrayDeclaration>(declarator_);
    if(next_array != nullptr)
	next_array->countDeclarations(declaration_count);

    declaration_count += size_;
}

std::string ArrayDeclaration::getId() const
{
    return declarator_->getId();
}
