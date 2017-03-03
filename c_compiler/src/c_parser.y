%code requires{

#include "ast.hpp"
    
extern TranslationUnit* g_root; // A way of getting the AST out

//! This is to fix problems when generating C++
// We are declaring the functions provided by Flex, so
// that Bison generated code can call them.
int yylex(void);
void yyerror(const char *);

}

// Represents the value associated with any kind of
// AST node.
%union{
    Node* node;
    TranslationUnit* trans_unit;
    Function* function;
    Type* type;
    Initializer* initializer;
    Declaration* declaration;
    double number;
    std::string* string;
}
                        
%token			T_TYPE_SPEC T_TYPE_QUAL T_STRG_SPEC T_IDENTIFIER T_SC T_CMA T_LRB T_LCB T_RCB
			T_LSB T_RSB T_QU T_COL T_LOG_OR T_LOG_AND T_OR T_XOR T_AND T_EQUALITY_OP
			T_REL_OP T_SHIFT_OP T_MULT T_DIV T_REM T_TILDE T_NOT T_DOT T_ARROW T_INCDEC
			T_ADDSUB_OP T_ASSIGN_OPER T_EQ T_SIZEOF T_INT_CONST T_IF T_WHILE T_DO T_FOR
			T_RETURN
			
%nonassoc		T_RRB
%nonassoc		T_ELSE
			
%type	<node>		ExternalDeclaration
			
%type	<trans_unit>	TranslationUnit
			
%type	<function>	FunctionDefinition

%type	<declaration>	Parameter Declaration InitDeclaratorList InitDeclarator ParameterList

%type	<type>		DeclarationSpec

%type	<string>	Declarator DirectDeclarator
			
%type	<number>        T_INT_CONST

%type	<initializer>	Initializer
			
%type	<string>	T_IDENTIFIER T_ASSIGN_OPER T_EQ T_AND T_ADDSUB_OP T_TILDE T_NOT
			T_MULT T_DIV T_REM
                        
%start ROOT
                        
%%

ROOT:
	        TranslationUnit { g_root = $1; }
		;

// TRANSLATION UNIT

TranslationUnit:
		ExternalDeclaration { $$ = new TranslationUnit($1); }
	|       TranslationUnit ExternalDeclaration { $$->push($2); }
		;

ExternalDeclaration:
		Declaration { $$ = $1; }
        |       FunctionDefinition { $$ = $1; }
		;

// FUNCTION DEFINITION

FunctionDefinition:
		DeclarationSpec T_IDENTIFIER T_LRB ParameterList T_RRB T_SC { $$ = new Function(*$2, $4); }
		;

ParameterList:
		%empty { $$ = new Declaration(); }
	| 	Parameter { $$ = $1; }
	|       ParameterList T_CMA Parameter { $3->addDeclaration($$); $$ = $3; }
		;

Parameter:
		DeclarationSpec T_IDENTIFIER { $$ = new Declaration(*$2); }
		;

// Declaration

Declaration:
		DeclarationSpec InitDeclaratorList T_SC { $$ = $2; }
		;

DeclarationSpec:
		DeclarationSpec_T { ; }
	|	DeclarationSpec_T DeclarationSpec { ; }
		;

DeclarationSpec_T:
		T_TYPE_SPEC { ; }
	|	T_TYPE_QUAL { ; }
	|	T_STRG_SPEC { ; }
		;

InitDeclaratorList:
		InitDeclarator { $$ = $1; }
	|       InitDeclaratorList T_CMA InitDeclarator { $3->addDeclaration($$); $$ = $3; }
		;

InitDeclarator:
		Declarator { $$ = new Declaration(*$1); }
	|	Declarator T_EQ Initializer { $$ = new Declaration(*$1); }
		;

Initializer:
		T_INT_CONST { $$ = new Initializer(); }
	;

Declarator:
		DirectDeclarator { $$ = $1; }
	|	T_MULT DirectDeclarator { $$ = $2; }
		;

DirectDeclarator:
		T_IDENTIFIER { $$ = $1; }
		;

%%

TranslationUnit* g_root; // Definition of variable (to match declaration earlier)

TranslationUnit* parseAST() {
    g_root = 0;
    yyparse();
    return g_root;
}
