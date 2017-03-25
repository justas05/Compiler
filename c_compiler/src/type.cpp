#include "type.hpp"

#include <cstdio>
#include <exception>


// Type definition

void Type::setSigned(bool)
{
    throw std::runtime_error("Error : cannot set sign");
}

void Type::setExtern(bool)
{
    throw std::runtime_error("Error : cannot set extern");
}

void Type::setStatic(bool)
{
    throw std::runtime_error("Error : cannot set static");
}

void Type::setConst(bool)
{
    throw std::runtime_error("Error : cannot set const");
}

void Type::setSize(int)
{
    throw std::runtime_error("Error : cannot set size");
}


// Array definition

Array::Array(const int &size, TypePtr type)
    : size_(size), type_(type)
{}

void Array::print() const
{
    printf("Array\n");
}

void Array::printXml() const
{}

VariableStackBindings Array::printAsm(VariableStackBindings bindings, unsigned &) const
{
    return bindings;
}

TypePtr Array::type()
{
    return type_;
}

TypePtr Array::type(Type *type_ptr)
{
    TypePtr sh_type_ptr(type_ptr);
    type_ = sh_type_ptr;
    return type_;
}

TypePtr Array::type(TypePtr type_ptr)
{
    type_ = type_ptr;
    return type_;
}

void Array::increaseStackPosition(VariableStackBindings &bindings) const
{
    for(int i = 0; i < size_; ++i)
    {
	type_->increaseStackPosition(bindings);
    }
}

void Array::load(const int &reg, const int &position) const
{
    type_->load(reg, position);
}

void Array::store(const int &position) const
{
    type_->store(position);
}


// Pointer definition

Pointer::Pointer()
{}

void Pointer::print() const
{
    printf("Pointer\n");
}

void Pointer::printXml() const
{}

VariableStackBindings Pointer::printAsm(VariableStackBindings bindings, unsigned &) const
{
    return bindings;
}

TypePtr Pointer::type()
{
    return type_;
}

TypePtr Pointer::type(Type *type_ptr)
{
    TypePtr sh_type_ptr(type_ptr);
    type_ = sh_type_ptr;
    return type_;
}

TypePtr Pointer::type(TypePtr type_ptr)
{
    type_ = type_ptr;
    return type_;
}

void Pointer::increaseStackPosition(VariableStackBindings &bindings) const
{
    bindings.increaseStackPosition(4);
}

void Pointer::load(const int &reg, const int &position) const
{
    printf("\tlw\t$%d,%d($fp)\n", reg, position);
}

void Pointer::store(const int &position) const
{
    type_->store(position);
}


// TypeContainer definition

TypeContainer::TypeContainer()
    : type_(nullptr), size_(32), extern_(false), static_(false), const_(false), signed_(true)
{}

void TypeContainer::print() const
{
    printf("TypeContainer : ");
    type_->print();
}

void TypeContainer::printXml() const
{}

VariableStackBindings TypeContainer::printAsm(VariableStackBindings bindings, unsigned &) const
{
    return bindings;
}

TypePtr TypeContainer::type()
{
    return type_;
}

TypePtr TypeContainer::type(Type *type_ptr)
{
    TypePtr new_type_ptr(type_ptr);
    type_ = new_type_ptr;

    return type_;
}

TypePtr TypeContainer::type(TypePtr type_ptr)
{
    type_ = type_ptr;
    
    return type_;
}

void TypeContainer::increaseStackPosition(VariableStackBindings &bindings) const
{
    type_->increaseStackPosition(bindings);
}

void TypeContainer::load(const int &reg, const int &position) const
{
    type_->load(reg, position);
}

void TypeContainer::store(const int &position) const
{
    type_->store(position);
}

void TypeContainer::setSigned(bool _signed)
{
    signed_ = _signed;
}

void TypeContainer::setExtern(bool _extern)
{
    extern_ = _extern;
}

void TypeContainer::setStatic(bool _static)
{
    static_ = _static;
}

void TypeContainer::setConst(bool _const)
{
    const_ = _const;
}

void TypeContainer::setSize(int size)
{
    size_ = size;
}


// Specifier definitions

TypePtr Specifier::type()
{
    throw std::runtime_error("Error : Cannot get type");
}

TypePtr Specifier::type(Type *)
{
    throw std::runtime_error("Error : Cannot get type");
}

TypePtr Specifier::type(TypePtr)
{
    throw std::runtime_error("Error : Cannot get type");
}


// Int definition

Int::Int()
{}

void Int::print() const
{
    printf("Int\n");
}

void Int::printXml() const
{}

VariableStackBindings Int::printAsm(VariableStackBindings bindings, unsigned &) const
{
    return bindings;
}

void Int::increaseStackPosition(VariableStackBindings &bindings) const
{
    bindings.increaseStackPosition(4);
}

void Int::load(const int &reg, const int &position) const
{
    printf("\tlw\t$%d,%d($fp)\n", reg, position);
}

void Int::store(const int &position) const
{
    printf("\tsw\t$2,%d($fp)\n", position);
}


// Void definition

Void::Void()
{}

void Void::print() const
{
    printf("Void\n");
}

void Void::printXml() const
{}

VariableStackBindings Void::printAsm(VariableStackBindings bindings, unsigned &) const
{
    return bindings;
}

void Void::increaseStackPosition(VariableStackBindings &) const
{}

void Void::load(const int &, const int &) const
{}

void Void::store(const int &) const
{}


// Short definition

Short::Short()
{}

void Short::print() const
{
    printf("Short\n");
}

void Short::printXml() const
{}

VariableStackBindings Short::printAsm(VariableStackBindings bindings, unsigned &) const
{
    return bindings;
}

void Short::increaseStackPosition(VariableStackBindings &bindings) const
{
    bindings.increaseStackPosition(2);
}

void Short::load(const int &reg, const int &position) const
{
    printf("\tlh\t$%d,%d($fp)\n", reg, position);
}

void Short::store(const int &position) const
{
    printf("\tsh\t$2,%d($fp)\n", position);
}


// Char definition

Char::Char()
{}

void Char::print() const
{
    printf("Char\n");
}

void Char::printXml() const
{}

VariableStackBindings Char::printAsm(VariableStackBindings bindings, unsigned &) const
{
    return bindings;
}

void Char::increaseStackPosition(VariableStackBindings &bindings) const
{
    bindings.increaseStackPosition(1);
}

void Char::load(const int &reg, const int &position) const
{
    printf("\tlb\t$%d,%d($fp)\n", reg, position);
}

void Char::store(const int &position) const
{
    printf("\tsb\t$2,%d($fp)\n", position);
}


// Float definition

Float::Float()
{}

void Float::print() const
{
    printf("Float\n");
}

void Float::printXml() const
{}

VariableStackBindings Float::printAsm(VariableStackBindings bindings, unsigned &) const
{
    return bindings;
}

void Float::increaseStackPosition(VariableStackBindings &bindings) const
{
    bindings.increaseStackPosition(4);
}

void Float::load(const int &, const int &) const
{
    throw std::runtime_error("Error : Cannot load float yet");    
}

void Float::store(const int &) const
{
    throw std::runtime_error("Error : Cannot store float yet");
}
