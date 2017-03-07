#ifndef AST_HPP
#define AST_HPP

#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <cstdint>

struct VarLocation;

typedef std::map<std::string, VarLocation> VariableStack;

#include "node.hpp"
#include "type.hpp"
#include "expression.hpp"
#include "declaration.hpp"
#include "statement.hpp"
#include "function.hpp"
#include "translation_unit.hpp"

TranslationUnit* parseAST();

#endif
