#include "declaration.hpp"
#include "bindings.hpp"
#include "type.hpp"
#include "expression.hpp"

#include <cstdio>


// Declaration definition

Declaration::Declaration(const std::string& id, Expression* initializer)
    : id_(id), initializer_(initializer), extern_declaration_(false)
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

VariableStackBindings Declaration::printAsm(VariableStackBindings bindings, unsigned& label_count) const
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

VariableStackBindings Declaration::localAsm(VariableStackBindings bindings, unsigned& label_count) const
{
    if(next_declaration_ != nullptr)
	bindings = next_declaration_->localAsm(bindings, label_count);

    if(next_list_declaration_ != nullptr)
	bindings = next_list_declaration_->localAsm(bindings, label_count);
    
    if(id_ != "")
    {
	if(initializer_ != nullptr)
	    initializer_->printAsm(bindings, label_count);
	else
	    printf("\tmove\t$2,$0\n");

	int stack_position = bindings.currentStackPosition();
	printf("\tsw\t$2,%d($fp)\n", stack_position);
	bindings.insertBinding(id_, type_, stack_position);
	bindings.increaseStackPosition();
    }

    return bindings;
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

std::string Declaration::getId() const
{
    return id_;
}

TypePtr Declaration::getType() const
{
    return type_;
}
