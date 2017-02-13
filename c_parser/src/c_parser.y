%code requires{

#include "ast.hpp"
extern const ast_Base *g_root; // A way of getting the AST out

//! This is to fix problems when generating C++
// We are declaring the functions provided by Flex, so
// that Bison generated code can call them.
int yylex(void);
void yyerror(const char *);

}

// Represents the value associated with any kind of
// AST node.
%union{
    const ast_Base *expr;
    //   double number;
    std::string *string;
}
			
%token	T_KEYWORD T_IDENTIFIER T_SC //T_CONSTANT T_OPERATOR T_LCBRACKET T_RCBRACKET
			
%type <expr> STMNT_LIST STMNT DCLRTN // COMP_STMNT EXPR_STMNT SLCT_STMNT ITR_STMNT JMP_STMNT
//			%type <number> //	T_CONSTANT
%type <string> T_KEYWORD T_IDENTIFIER //T_OPERATOR
			
%start ROOT
			
%%

ROOT : STMNT_LIST { g_root = $1; }

STMNT_LIST : STMNT { $$ = $1; }
	| 	STMNT_LIST STMNT { $$ = $2; }

STMNT : DCLRTN { $$ = $1; }

DCLRTN : T_KEYWORD T_IDENTIFIER T_SC { $$ = new ast_Declaration(*$2); }

%%

const ast_Base *g_root; // Definition of variable (to match declaration earlier)

const ast_Base *parseAST() {
    g_root = 0;
    yyparse();
    return g_root;
}
