#ifndef AST_HPP
#define AST_HPP

#include <vector>
#include <string>
#include <iostream>

#include "base.hpp"
#include "statement.hpp"
#include "function.hpp"
#include "declaration.hpp"
#include "expression.hpp"
#include "primitives.hpp"
#include "external.hpp"
#include "ast_top.hpp"

const BaseList* parseAST();

#endif
