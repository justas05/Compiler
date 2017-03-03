#ifndef PRIMITIVES_HPP
#define PRIMITIVES_HPP

#include "ast.hpp"

#include <cstdint>


class ParamList : public BaseList {
public:
    ParamList();
    ParamList(const Base* _param);
};


class Declarator : public BasePrimitive {
public:
    Declarator(const std::string& _id);

    virtual void printxml() const;
};


class Parameter : public BasePrimitive {
public:
    Parameter(const std::string& _id);

    virtual void printxml() const;
};

class Immediate : public BasePrimitive {
protected:
    int32_t imm;
public:
    Immediate(const int32_t& _imm);

    virtual void printasm() const override;
};


#endif
