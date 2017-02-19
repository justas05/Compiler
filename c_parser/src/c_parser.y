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
%token T_SC T_CMA T_EQ T_LRB T_RRB T_LCB T_RCB T_EQUALITY T_RETURN
%token T_INT_CONST
%token T_IF T_ELSE
                        
%type <stmnt> EXT_DEF EXT_DECLARATION
%type <stmnt> FUNC_DEF PARAMETER_LIST PARAMETER PARAM_DECLARATOR
%type <stmnt> DECLARATION_LIST DECLARATION DECLARATION_SPEC DECLARATION_SPEC_T INIT_DECLARATOR INIT_DECLARATOR_LIST DECLARATOR INITIALIZER
%type <stmnt> STATEMENT_LIST STATEMENT COMPOUND_STATEMENT COMPOUND_STATEMENT_2 SELECTION_STATEMENT SELECTION_STATEMENT_2 EXPRESSION_STATEMENT
%type <stmnt> EXPRESSION EQUALITY_EXPRESSION RETURN_EXPRESSION
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

EXT_DECLARATION : DECLARATION { $$ = $1; }
        |       FUNC_DEF { $$ = $1; }
;

// FUNCTION DEFINITION

FUNC_DEF : DECLARATION_SPEC T_IDENTIFIER T_LRB PARAMETER_LIST T_RRB COMPOUND_STATEMENT { $$ = new ast_Function(*$2, $4, $6); }
;

PARAMETER_LIST : %empty { $$ = new ast_ParamList(); }
	| 	PARAMETER { $$ = new ast_ParamList($1); }
	|	PARAMETER_LIST T_CMA PARAMETER { $$->push($3); }
;

PARAMETER : DECLARATION_SPEC PARAM_DECLARATOR { $$ = $2; }
;

PARAM_DECLARATOR : T_IDENTIFIER { $$ = new ast_Parameter(*$1);}
;

// DECLARATION

DECLARATION_LIST : DECLARATION { $$ = new ast_DeclarationList($1); }
	|	DECLARATION_LIST DECLARATION { $$->push($2); }

DECLARATION : DECLARATION_SPEC INIT_DECLARATOR_LIST T_SC { $$ = $2; }
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

STATEMENT_LIST : STATEMENT { $$ = new ast_StatementList($1); }
	|	STATEMENT_LIST STATEMENT { $$->push($2); }
;

STATEMENT : COMPOUND_STATEMENT { $$ = $1; }
	|	SELECTION_STATEMENT { $$ = $1; }
	|	EXPRESSION_STATEMENT { $$ = $1; }
;

COMPOUND_STATEMENT : T_LCB COMPOUND_STATEMENT_2 { $$ = $2; }
;

COMPOUND_STATEMENT_2 : T_RCB { $$ = new ast_CompoundStatement; }
	|	DECLARATION_LIST T_RCB { $$ = new ast_CompoundStatement($1); }
	|	DECLARATION_LIST STATEMENT_LIST T_RCB { $$ = new ast_CompoundStatement($1, $2); }
	|	STATEMENT_LIST T_RCB { $$ = new ast_CompoundStatement($1); }
;

SELECTION_STATEMENT : T_IF T_LRB EXPRESSION T_RRB STATEMENT SELECTION_STATEMENT_2 { $$ = new ast_SelectionStatement($5, $6); }
;

SELECTION_STATEMENT_2 : %empty { $$ = new ast_SelectionStatement(); }
	|	T_ELSE STATEMENT { $$ = $2; }
;

EXPRESSION_STATEMENT : EXPRESSION T_SC { $$ = $1; }
;

// Expressions

EXPRESSION : EQUALITY_EXPRESSION { $$ = $1; }
	|	RETURN_EXPRESSION { $$ = $1; }
;

EQUALITY_EXPRESSION : T_IDENTIFIER T_EQUALITY T_IDENTIFIER { $$ = new ast_Variable(*$1); }
;

RETURN_EXPRESSION : T_RETURN T_IDENTIFIER { $$ = new ast_ReturnExpression(*$2); }
;

%%

ast_Top *g_root; // Definition of variable (to match declaration earlier)

ast_Top *parseAST() {
    g_root = new ast_Top;
    yyparse();
    return g_root;
}
