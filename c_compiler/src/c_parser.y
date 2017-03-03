%code requires{

#include "ast.hpp"
    
extern const BaseList* g_root; // A way of getting the AST out

//! This is to fix problems when generating C++
// We are declaring the functions provided by Flex, so
// that Bison generated code can call them.
int yylex(void);
void yyerror(const char *);

}

// Represents the value associated with any kind of
// AST node.
%union{
    const BaseNode* base_node;
    const BaseList* base_list;
    const BasePrimitive* base_prim;
    const BaseType* base_type;
    double number;
    std::string *string;
}
                        
%token			T_TYPE_SPEC T_TYPE_QUAL T_STRG_SPEC T_IDENTIFIER T_SC T_CMA T_LRB T_LCB T_RCB
			T_LSB T_RSB T_QU T_COL T_LOG_OR T_LOG_AND T_OR T_XOR T_AND T_EQUALITY_OP
			T_REL_OP T_SHIFT_OP T_MULT T_DIV T_REM T_TILDE T_NOT T_DOT T_ARROW T_INCDEC
			T_ADDSUB_OP T_ASSIGN_OPER T_EQ T_SIZEOF T_INT_CONST T_IF T_WHILE T_DO T_FOR
			T_RETURN
			
%nonassoc		T_RRB
%nonassoc		T_ELSE
			
                        
%type	<base_list>	ExtDef ParameterList DeclarationList InitDeclaratorList IdentifierList
			StatementList ArgumentExpressionList

%type	<base_node>	ExtDeclaration FuncDef Declaration DeclarationSpec DeclarationSpec_T
			Statement CompoundStatement CompoundStatement_2 PrimaryExpression
			SelectionStatement ExpressionStatement JumpStatement IterationStatement
			Expression AssignmentExpression ConditionalExpression LogicalOrExpression
			LogicalAndExpression InclusiveOrExpression ExclusiveOrExpression
			AndExpression EqualityExpression RelationalExpression ShiftExpression
			AdditiveExpression MultiplicativeExpression CastExpression UnaryExpression
			PostfixExpression PostfixExpression2
			
%type	<base_prim>	Parameter ParamDeclarator Declarator DirectDeclarator InitDeclarator Constant
			
%type	<number>        T_INT_CONST

			
%type	<string>	T_IDENTIFIER ASSIGN_OPER T_ASSIGN_OPER T_EQ T_AND T_ADDSUB_OP T_TILDE T_NOT
			T_MULT T_DIV T_REM MultDivRemOP UnaryOperator
                        
%start ROOT
                        
%%

ROOT:
	        ExtDef { g_root = $1; }
		;

// EXTERNAL DEFINITION

ExtDef:
		ExtDeclaration { $$ = new ExternalDefinition($1); }
|       ExtDef ExtDeclaration { $$->push($2); }
		;

ExtDeclaration:
		Declaration { $$ = $1; }
        |       FuncDef { $$ = $1; }
		;

// FUNCTION DEFINITION

FuncDef:
	    DeclarationSpec T_IDENTIFIER T_LRB ParameterList T_RRB CompoundStatement { $$ = new Function(*$2, $4, $6); }
		;

ParameterList:
		%empty { $$ = new ParamList(); }
	| 	Parameter { $$ = new ParamList($1); }
	|       ParameterList T_CMA Parameter { $$->push($3); }
		;

Parameter:
		DeclarationSpec ParamDeclarator { $$ = $2; }
		;

ParamDeclarator:
		T_IDENTIFIER { $$ = new Parameter(*$1);}
		;

// Declaration

DeclarationList:
		Declaration { $$ = new DeclarationList($1); }
	|	DeclarationList Declaration { $$->push($2); }
		;

Declaration:
		DeclarationSpec InitDeclaratorList T_SC { $$ = new Declaration($2); }
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
		InitDeclarator { $$ = new InitDeclaratorList($1); }
	|       InitDeclaratorList T_CMA InitDeclarator { $$->push($3); }
		;

InitDeclarator:
		Declarator { $$ = $1; }
	|	Declarator T_EQ AssignmentExpression { $$ = $1; }
		;

Declarator:
		DirectDeclarator { $$ = $1; }
	|	T_MULT DirectDeclarator { $$ = $2; }
		;

DirectDeclarator:
		T_IDENTIFIER { $$ = new Declarator(*$1); }
	|	T_LRB Declarator T_RRB { $$ = $2; }
	|	DirectDeclarator T_LSB ConditionalExpression T_RSB { $$ = $1; }
	|	DirectDeclarator T_LSB T_RSB { $$ = $1; }
	|	DirectDeclarator T_LRB ParameterList T_RRB { $$ = $1; }
	|	DirectDeclarator T_LRB IdentifierList T_RRB { $$ = $1; }
		;

IdentifierList:
		T_IDENTIFIER { $$ = new BaseList(); }
	|	IdentifierList T_CMA T_IDENTIFIER { $$ = new BaseList(); }
		;

// Statement

StatementList:
		Statement { $$ = new StatementList($1); }
	|	StatementList Statement { $$->push($2); }
		;

Statement:
		CompoundStatement { $$ = $1; }
	|	SelectionStatement { $$ = $1; }
	|	ExpressionStatement { $$ = $1; }
	|       JumpStatement { $$ = $1; }
	|	IterationStatement { $$ = $1; }
		;

CompoundStatement:
		T_LCB CompoundStatement_2 { $$ = $2; }
		;

CompoundStatement_2:
		T_RCB { $$ = new CompoundStatement; }
	|	DeclarationList T_RCB { $$ = new CompoundStatement($1); }
	|	DeclarationList StatementList T_RCB { $$ = new CompoundStatement($1, $2); }
	|	StatementList T_RCB { $$ = new CompoundStatement($1); }
		;

SelectionStatement:
		T_IF T_LRB Expression T_RRB Statement { $$ = new SelectionStatement($5); }
	|	T_IF T_LRB Expression T_RRB Statement T_ELSE Statement { $$ = new SelectionStatement($5, $7); }
		;

ExpressionStatement:
		T_SC { $$ = new ExpressionStatement(); }
	|	Expression T_SC { $$ = $1; }
		;

JumpStatement:
		  T_RETURN ExpressionStatement { $$ = new JumpStatement($2); }
		;

IterationStatement:
		T_WHILE T_LRB Expression T_RRB Statement { $$ = $5; }
	|	T_DO Statement T_WHILE T_LRB Expression T_RRB T_SC { $$ = $2; }
	|	T_FOR T_LRB Expression T_SC Expression T_SC Expression T_RRB Statement { $$ = $9; }
		;

// Expressions

Expression:
		AssignmentExpression { $$ = $1; }
		;

AssignmentExpression:
		ConditionalExpression { $$ = $1; }
	|	UnaryExpression ASSIGN_OPER AssignmentExpression { $$ = $1; }
		;

ASSIGN_OPER:
		T_ASSIGN_OPER { ; }
	|	T_EQ { ; }
	;

ConditionalExpression:
		LogicalOrExpression { $$ = $1; }
	|	LogicalOrExpression T_QU Expression T_COL ConditionalExpression { $$ = $1; }
		;

LogicalOrExpression:
		LogicalAndExpression { $$ = $1; }
	|	LogicalOrExpression T_LOG_OR LogicalAndExpression { $$ = $3; }
		;

LogicalAndExpression:
		InclusiveOrExpression { $$ = $1; }
	|	LogicalAndExpression T_LOG_AND InclusiveOrExpression { $$ = $3; }
		;

InclusiveOrExpression:
		ExclusiveOrExpression { $$ = $1; }
	|	InclusiveOrExpression T_OR ExclusiveOrExpression { $$ = $3; }
		;

ExclusiveOrExpression:
		AndExpression { $$ = $1; }
	|	ExclusiveOrExpression T_XOR AndExpression { $$ = $3; }
		;

AndExpression:
		EqualityExpression { $$ = $1; }
	|	AndExpression T_AND EqualityExpression { $$ = $3; }
		;

EqualityExpression:
	        RelationalExpression { $$ = $1; }
	|	EqualityExpression T_EQUALITY_OP RelationalExpression { $$ = $3; }
		;

RelationalExpression:
		ShiftExpression { $$ = $1; }
	|       RelationalExpression T_REL_OP ShiftExpression { $$ = $3; }
		;

ShiftExpression:
		AdditiveExpression { $$ = $1; }
	|	ShiftExpression T_SHIFT_OP AdditiveExpression { $$ = $3; }
		;

AdditiveExpression:
		MultiplicativeExpression { $$ = $1; }
	|	AdditiveExpression T_ADDSUB_OP MultiplicativeExpression { $$ = $3; }
		;

MultiplicativeExpression:
		CastExpression { $$ = $1; }
	|	MultiplicativeExpression MultDivRemOP CastExpression { $$ = $3; }
		;

MultDivRemOP:
		T_MULT { $$ = $1; }
	|	T_DIV { $$ = $1; }
	|	T_REM { $$ = $1; }
		;

CastExpression:
		UnaryExpression { $$ = $1; }
	|	T_LRB T_TYPE_SPEC T_RRB CastExpression { $$ = $4; }
		;

UnaryExpression:
		PostfixExpression { $$ = $1; }
	|	T_INCDEC UnaryExpression { $$ = $2; }
	|	UnaryOperator CastExpression { $$ = $2; }
	|	T_SIZEOF UnaryExpression { $$ = $2; }
	|	T_SIZEOF T_LRB T_TYPE_SPEC T_RRB { $$ = new Expression(); }
		;

UnaryOperator:
		T_AND { $$ = $1; }
	|	T_ADDSUB_OP { $$ = $1; }
	|	T_MULT { $$ = $1; }
	|	T_TILDE { $$ = $1; }
	|	T_NOT { $$ = $1; }
		;

PostfixExpression:
		PrimaryExpression { $$ = $1; }
	|	PostfixExpression T_LSB Expression T_RSB { $$ = $3; }
	|	PostfixExpression T_LRB PostfixExpression2 { $$ = $3; }
	|	PostfixExpression T_DOT T_IDENTIFIER { $$ = new Expression(); }
	|	PostfixExpression T_ARROW T_IDENTIFIER { $$ = new Expression(); }
	|	PostfixExpression T_INCDEC { $$ = new Expression(); }
		;

PostfixExpression2:
		T_RRB { $$ = new Expression(); }
	|	ArgumentExpressionList T_RRB { $$ = new BaseNode; }
		;

ArgumentExpressionList:
		AssignmentExpression { $$ = new BaseList; }
	|	ArgumentExpressionList T_CMA AssignmentExpression { $$ = new BaseList; }
		;

PrimaryExpression:
		T_IDENTIFIER { $$ = new Expression(); }
	|       Constant { $$ = new Expression($1); }
	|	T_LRB Expression T_RRB { $$ = $2; }
		;

Constant:
		T_INT_CONST { $$ = new Immediate($1); }
		;

%%

const BaseList* g_root; // Definition of variable (to match declaration earlier)

const BaseList* parseAST() {
    g_root = 0;
    yyparse();
    return g_root;
}
