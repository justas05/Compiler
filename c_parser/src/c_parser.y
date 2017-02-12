%code requires{

#include "ast.hpp"

extern const Expression *g_root; // A way of getting the AST out

//! This is to fix problems when generating C++
// We are declaring the functions provided by Flex, so
// that Bison generated code can call them.
int yylex(void);
void yyerror(const char *);

}

// Represents the value associated with any kind of
// AST node.
%union{
    const Expression *expr;
    double number;
    std::string *string;
}

%token T_KEYWORD T_IDENTIFIER T_CONSTANT T_OPERATOR T_LCBRACKET T_RCBRACKET

%type <expr> STMNT_LIST STMNT COMP_STMNT EXPR_STMNT SLCT_STMNT ITR_STMNT JMP_STMNT
%type <number> T_CONSTANT
%type <string> T_KEYWORD T_IDENTIFIER T_OPERATOR

%start ROOT

%%

ROOT : STMNT_LIST { g_root = $1; }

STMNT_LIST : STMNT
           | STMNT_LIST STMNT

STMNT : COMP_STMNT
      | EXPR_STMNT
      | SLCT_STMNT
      | ITR_STMNT
      | JMP_STMNT

COMP_STMNT : STMNT_LIST

%%

const Expression *g_root; // Definition of variable (to match declaration earlier)

const Expression *parseAST() {
    g_root = 0;
    yyparse();
    return g_root;
}