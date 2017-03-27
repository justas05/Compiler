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

Bindings Array::printAsm(Bindings bindings, int &) const
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

void Array::increaseStackPosition(Bindings &bindings) const
{
    for(int i = 0; i < size_; ++i)
    {
	type_->increaseStackPosition(bindings);
    }
}

void Array::load() const
{
    type_->load();
}

void Array::load(const int &reg, const int &position) const
{
    type_->load(reg, position);
}

void Array::store() const
{
    type_->store();
}

void Array::store(const int &position) const
{
    type_->store(position);
}

void Array::store(const int &reg, const int &position) const
{
    type_->store(reg, position);
}

int Array::getSize() const
{
    return type_->getSize();
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

Bindings Pointer::printAsm(Bindings bindings, int &) const
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

void Pointer::increaseStackPosition(Bindings &bindings) const
{
    bindings.increaseStackPosition(4);
}

void Pointer::load() const
{
    printf("\tlw\t$2,0($t0)\n");
}

void Pointer::load(const int &reg, const int &position) const
{
    printf("\tlw\t$%d,%d($fp)\n", reg, position);
}

void Pointer::store() const
{
    printf("\tsw\t$2,0($t0)\n");
}

void Pointer::store(const int &position) const
{
    printf("\tsw\t$2,%d($fp)\n", position);
}

void Pointer::store(const int &reg, const int &position) const
{
    printf("\tsw\t$%d,%d($fp)\n", reg, position);
}

int Pointer::getSize() const
{
    return type_->getSize();
}

void Pointer::pointerLoad() const
{
    type_->load();
}

void Pointer::pointerLoad(const int &reg, const int &position) const
{
    type_->load(reg, position);
}

void Pointer::pointerStore() const
{

    type_->store();
}

void Pointer::pointerStore(const int &position) const
{
    type_->store(position);
}

void Pointer::pointerStore(const int &reg, const int &position) const
{
    type_->store(reg, position);
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

Bindings TypeContainer::printAsm(Bindings bindings, int &) const
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

void TypeContainer::increaseStackPosition(Bindings &bindings) const
{
    type_->increaseStackPosition(bindings);
}

void TypeContainer::load() const
{
    type_->load();
}

void TypeContainer::load(const int &reg, const int &position) const
{
    type_->load(reg, position);
}

void TypeContainer::store() const
{
    type_->store();
}

void TypeContainer::store(const int &position) const
{
    type_->store(position);
}

void TypeContainer::store(const int &reg, const int &position) const
{
    type_->store(reg, position);
}

int TypeContainer::getSize() const
{
    return type_->getSize();
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

Bindings Int::printAsm(Bindings bindings, int &) const
{
    return bindings;
}

void Int::increaseStackPosition(Bindings &bindings) const
{
    bindings.increaseStackPosition(4);
}

void Int::load() const
{
    printf("\tlw\t$2,0($t0)\n");
}

void Int::load(const int &reg, const int &position) const
{
    printf("\tlw\t$%d,%d($fp)\n", reg, position);
}

void Int::store() const
{
    printf("\tsw\t$2,0($t0)\n");
}

void Int::store(const int &position) const
{
    printf("\tsw\t$2,%d($fp)\n", position);
}

void Int::store(const int &reg, const int &position) const
{
    printf("\tsw\t$%d,%d($fp)\n", reg, position);
}

int Int::getSize() const
{
    return 4;
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

Bindings Void::printAsm(Bindings bindings, int &) const
{
    return bindings;
}

void Void::increaseStackPosition(Bindings &) const
{}

void Void::load() const
{}

void Void::load(const int &, const int &) const
{}

void Void::store() const
{}

void Void::store(const int &) const
{}

void Void::store(const int &, const int &) const
{}

int Void::getSize() const
{
    return 0;
}


// Short definition

Short::Short()
{}

void Short::print() const
{
    printf("Short\n");
}

void Short::printXml() const
{}

Bindings Short::printAsm(Bindings bindings, int &) const
{
    return bindings;
}

void Short::increaseStackPosition(Bindings &bindings) const
{
    bindings.increaseStackPosition(2);
}

void Short::load() const
{
    printf("\tlhu\t$2,0($t0)\n");
}

void Short::load(const int &reg, const int &position) const
{
    printf("\tlhu\t$%d,%d($fp)\n", reg, position);
}

void Short::store() const
{
    printf("\tsh\t$2,0($t0)\n");
}

void Short::store(const int &position) const
{
    printf("\tsh\t$2,%d($fp)\n", position);
}

void Short::store(const int &reg, const int &position) const
{
    printf("\tsh\t$%d,%d($fp)\n", reg, position);
}

int Short::getSize() const
{
    return 2;
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

Bindings Char::printAsm(Bindings bindings, int &) const
{
    return bindings;
}

void Char::increaseStackPosition(Bindings &bindings) const
{
    bindings.increaseStackPosition(1);
}

void Char::load() const
{
    printf("\tlbu\t$2,0($t0)\n");
}

void Char::load(const int &reg, const int &position) const
{
    printf("\tlbu\t$%d,%d($fp)\n", reg, position);
}

void Char::store() const
{
    printf("\tsb\t$2,0($t0)\n");
}

void Char::store(const int &position) const
{
    printf("\tsb\t$2,%d($fp)\n", position);
}

void Char::store(const int &reg, const int &position) const
{
    printf("\tsb\t$%d,%d($fp)\n", reg, position);
}

int Char::getSize() const
{
    return 1;
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

Bindings Float::printAsm(Bindings bindings, int &) const
{
    return bindings;
}

void Float::increaseStackPosition(Bindings &bindings) const
{
    bindings.increaseStackPosition(4);
}

void Float::load() const
{
    throw std::runtime_error("Error : Cannot load float yet");        
}

void Float::load(const int &, const int &) const
{
    throw std::runtime_error("Error : Cannot load float yet");    
}

void Float::store() const
{
    throw std::runtime_error("Error : Cannot store float yet");
}

void Float::store(const int &) const
{
    throw std::runtime_error("Error : Cannot store float yet");
}

void Float::store(const int &, const int &) const
{
    throw std::runtime_error("Error : Cannot store float yet");
}

int Float::getSize() const
{
    return 4;
}
