#ifndef EXTERNAL_HPP
#define EXTERNAL_HPP

#include "ast.hpp"


class ExternalDefinition : public BaseList {
public:
    ExternalDefinition(const Base* _var) : BaseList(_var) {}
};


#endif
