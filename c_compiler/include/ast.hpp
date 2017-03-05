#ifndef AST_HPP
#define AST_HPP

#include <vector>
#include <string>
#include <iostream>

#include "node.hpp"
#include "expression.hpp"
#include "type.hpp"
#include "initializer.hpp"
#include "declaration.hpp"
#include "statement.hpp"
#include "function.hpp"
#include "translation_unit.hpp"

TranslationUnit* parseAST();

#endif
