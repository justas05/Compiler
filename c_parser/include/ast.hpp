#ifndef AST_HPP
#define AST_HPP

#include "ast_base.hpp"
#include "ast_statement.hpp"
#include "ast_function.hpp"
#include "ast_declaration.hpp"
#include "ast_expression.hpp"
#include "ast_primitives.hpp"
#include "ast_top.hpp"

ast_Top *parseAST();

#endif
