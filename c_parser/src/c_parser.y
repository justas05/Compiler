%code requires{

#include "ast.hpp"
extern ast_Top *g_root; // A way of getting the AST out

//! This is to fix problems when generating C++
// We are declaring the functions provided by Flex, so
// that Bison generated code can call them.
int yylex(void);
void yyerror(const char *);

}

// Represents the value associated with any kind of
// AST node.
%union{
    const ast_Base *stmnt;
    //   double number;
    std::string *string;
}
                        
%token T_TYPE_SPEC T_TYPE_QUAL T_STRG_SPEC T_IDENTIFIER T_SC T_CMA T_EQ T_LRB T_RRB T_LCB T_RCB T_INT_CONST
                        
%type <stmnt> EXT_DEF EXT_DECLARATION FUNC_DEF DECLARATION DECLARATION_SPEC DECLARATION_SPEC_T INIT_DECLARATOR INIT_DECLARATOR_LIST DECLARATOR INITIALIZER
//                      %type <number> //       T_CONSTANT
%type <string> T_IDENTIFIER //T_OPERATOR
                        
%start ROOT
                        
%%

ROOT : EXT_DEF { ; }

// EXTERNAL DEFINITION

EXT_DEF : EXT_DECLARATION { ; }
	|       EXT_DEF EXT_DECLARATION { $$ = $2; }

EXT_DECLARATION : DECLARATION_SPEC EXT_DECLARATION_2 { ; }

EXT_DECLARATION_2 : DECLARATION { ; }
	|	FUNC_DEF { ; }

// FUNCTION DEFINITION

FUNC_DEF : T_IDENTIFIER T_LRB  T_RRB T_LCB T_RCB { ; }

// DECLARATION

DECLARATION : INIT_DECLARATOR_LIST T_SC { ; }

DECLARATION_SPEC :   DECLARATION_SPEC_T { ; }
	|	DECLARATION_SPEC_T DECLARATION_SPEC { ; }

DECLARATION_SPEC_T : T_TYPE_SPEC { ; }
	|	T_TYPE_QUAL { ; }
	|	T_STRG_SPEC { ; }

INIT_DECLARATOR_LIST : INIT_DECLARATOR { g_root->push($1); }
	|       INIT_DECLARATOR_LIST T_CMA INIT_DECLARATOR { g_root->push($3); }

INIT_DECLARATOR :   DECLARATOR { ; }
	|	DECLARATOR T_EQ INITIALIZER { ; }

DECLARATOR :        T_IDENTIFIER {$$ = new ast_Declaration(*$1); }

INITIALIZER :   T_INT_CONST { ; }

// STATEMENTS

%%

ast_Top *g_root; // Definition of variable (to match declaration earlier)

ast_Top *parseAST() {
    g_root = new ast_Top;
    yyparse();
    return g_root;
}
