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
                        
%token T_TYPE_SPEC T_TYPE_QUAL T_STRG_SPEC T_IDENTIFIER
%token T_SC T_CMA T_EQ T_LRB T_RRB T_LCB T_RCB
%token T_INT_CONST
                        
%type <stmnt> EXT_DEF EXT_DECLARATION EXT_DECLARATION_2
%type <stmnt> FUNC_DEF PARAMETER_LIST PARAMETER PARAM_DECLARATOR
%type <stmnt> DECLARATION DECLARATION_SPEC DECLARATION_SPEC_T INIT_DECLARATOR INIT_DECLARATOR_LIST DECLARATOR INITIALIZER
%type <stmnt> COMPOUND_STATEMENT
//                      %type <number> //       T_CONSTANT
%type <string> T_IDENTIFIER //T_OPERATOR
                        
%start ROOT
                        
%%

ROOT : EXT_DEF { ; }
;

// EXTERNAL DEFINITION

EXT_DEF : EXT_DECLARATION { g_root->push($1); }
        |       EXT_DEF EXT_DECLARATION { g_root->push($2); }
;

EXT_DECLARATION : DECLARATION_SPEC EXT_DECLARATION_2 { $$ = $2; }
;

EXT_DECLARATION_2 : DECLARATION { $$ = $1; }
        |       FUNC_DEF { $$ = $1; }
;

// FUNCTION DEFINITION

FUNC_DEF : T_IDENTIFIER T_LRB PARAMETER_LIST T_RRB COMPOUND_STATEMENT { $$ = new ast_Function(*$1, $3); }
;

PARAMETER_LIST: PARAMETER { $$ = new ast_ParamList($1); }
	|	PARAMETER_LIST T_CMA PARAMETER { $$->push($3); }
;

PARAMETER: DECLARATION_SPEC PARAM_DECLARATOR { $$ = $2; }
;

PARAM_DECLARATOR: T_IDENTIFIER { $$ = new ast_Parameter(*$1);}

// DECLARATION

DECLARATION : INIT_DECLARATOR_LIST T_SC { $$ = $1; }
;

DECLARATION_SPEC : DECLARATION_SPEC_T { ; }
	|	DECLARATION_SPEC_T DECLARATION_SPEC { ; }
;

DECLARATION_SPEC_T : T_TYPE_SPEC { ; }
	|	T_TYPE_QUAL { ; }
	|	T_STRG_SPEC { ; }
;

INIT_DECLARATOR_LIST : INIT_DECLARATOR { $$ = new ast_VariableDeclaration($1); }
	|       INIT_DECLARATOR_LIST T_CMA INIT_DECLARATOR { $$->push($3); }
;

INIT_DECLARATOR : DECLARATOR { ; }
	|	DECLARATOR T_EQ INITIALIZER { ; }
;

DECLARATOR : T_IDENTIFIER {$$ = new ast_Variable(*$1); }
;

INITIALIZER : T_INT_CONST { ; }
;

// STATEMENT

COMPOUND_STATEMENT: T_SC { ; }
;

%%

ast_Top *g_root; // Definition of variable (to match declaration earlier)

ast_Top *parseAST() {
    g_root = new ast_Top;
    yyparse();
    return g_root;
}
