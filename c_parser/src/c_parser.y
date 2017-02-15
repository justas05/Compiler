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
			
%token T_TYPE_SPEC T_TYPE_QUAL T_STRG_SPEC T_IDENTIFIER T_SC T_CMA T_EQ T_INT_CONST
			
%type <stmnt> STMNT DCLRTN DCLRTN_SPEC DCLRTN_SPEC_T INIT_DCLRTR INIT_DCLRTR_LIST DCLRTR INITIALIZER STMNT_LIST  // COMP_STMNT STMNT_LIST_STMNT SLCT_STMNT ITR_STMNT JMP_STMNT
//			%type <number> //	T_CONSTANT
%type <string> T_IDENTIFIER //T_OPERATOR
			
%start ROOT
			
%%

ROOT : STMNT_LIST { ; }

STMNT_LIST :    STMNT { ; }
	| 	STMNT_LIST STMNT { ; }
	|	DCLRTN { ; }

STMNT :         DCLRTN { ; }

DCLRTN :        DCLRTN_SPEC INIT_DCLRTR_LIST T_SC { $$ = $2; }
DCLRTN_SPEC :   DCLRTN_SPEC_T { ; }
	|	DCLRTN_SPEC_T DCLRTN_SPEC { ; }
DCLRTN_SPEC_T : T_TYPE_SPEC { ; }
	|	T_TYPE_QUAL { ; }
	|	T_STRG_SPEC { ; }
INIT_DCLRTR_LIST : INIT_DCLRTR { g_root->push($1); }
	|       INIT_DCLRTR_LIST T_CMA INIT_DCLRTR { g_root->push($3); }
INIT_DCLRTR :   DCLRTR { ; }
	|	DCLRTR T_EQ INITIALIZER { ; }
DCLRTR :        T_IDENTIFIER {$$ = new ast_Declaration(*$1); }
INITIALIZER :   T_INT_CONST { ; }

%%

ast_Top *g_root; // Definition of variable (to match declaration earlier)

ast_Top *parseAST() {
    g_root = new ast_Top;
    yyparse();
    return g_root;
}
