%code requires{

#include "node.hpp"
#include "translation_unit.hpp"
#include "function.hpp"
#include "declaration.hpp"
#include "statement.hpp"
#include "expression.hpp"
#include "type.hpp"
    
    
extern Node* g_root; // A way of getting the AST out

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
    Statement* statement;
    Declaration* declaration;
    Expression* expression;
    Type* type;
    double number;
    std::string* string;
}
			               
%token			T_IDENTIFIER T_SC T_CMA T_LRB T_LCB T_RCB T_LSB T_RSB T_QU T_COL T_LOG_OR
			T_LOG_AND T_OR T_XOR T_AND T_EQUALITY_OP T_REL_OP T_SHIFT_OP T_MULT T_DIV
			T_REM T_TILDE T_NOT T_DOT T_ARROW T_INCDEC T_ADDSUB_OP T_ASSIGN_OPER T_EQ
			T_SIZEOF T_INT_CONST T_IF T_WHILE T_DO T_FOR T_RETURN
			
			T_VOID T_CHAR T_SCHAR T_UCHAR T_SSINT T_USINT T_LINT T_ULINT T_UINT T_SINT
			
%nonassoc		T_RRB
%nonassoc		T_ELSE
			
			
%type	<node>		ExternalDeclaration
			
%type	<trans_unit>	TranslationUnit
			
%type	<function>	FunctionDefinition

%type	<statement>	StatementList Statement CompoundStatement CompoundStatement_2
			SelectionStatement
			ExpressionStatement JumpStatement IterationStatement

%type	<declaration>	ParameterList Parameter DeclarationList Declaration InitDeclaratorList
			InitDeclarator
			IdentifierList
			Declarator DirectDeclarator

%type	<expression>	Expression AssignmentExpression ConditionalExpression LogicalOrExpression
			LogicalAndExpression InclusiveOrExpression ExclusiveOrExpression
			AndExpression EqualityExpression RelationalExpression ShiftExpression
			AdditiveExpression MultiplicativeExpression CastExpression UnaryExpression
			PostfixExpression PostfixExpression2 ArgumentExpressionList PrimaryExpression
			Constant

%type	<number>        T_INT_CONST
			
%type	<string>	T_IDENTIFIER ASSIGN_OPER T_ASSIGN_OPER T_EQ T_AND T_ADDSUB_OP T_TILDE T_NOT
			T_MULT T_DIV T_REM T_EQUALITY_OP T_REL_OP T_SHIFT_OP MultDivRemOP UnaryOperator
			DeclarationSpec
                        
%start ROOT
                        
%%

ROOT:
		TranslationUnit { g_root = $1; }
		;

// EXTERNAL DEFINITION

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
		DeclarationSpec Declarator CompoundStatement
		{ $$ = new Function($2->getId(), $3, $2->getNext()); }
		;

ParameterList:
		%empty { $$ = new Declaration(); }
	| 	Parameter { $$ = $1; }
	|       ParameterList T_CMA Parameter { $3->linkDeclaration($$); $$ = $3; }
		;

Parameter:	DeclarationSpec T_IDENTIFIER { $$ = new Declaration(*$2); delete $2; }
		;

// Declaration

DeclarationList:
		Declaration { $$ = $1; }
	|	DeclarationList Declaration { $2->linkDeclaration($$); $$ = $2; }
		;

Declaration:	DeclarationSpec InitDeclaratorList T_SC
		{
		    $$ = $2;
		    Declaration* tmp_decl = $2;
    
		    while(tmp_decl != nullptr) {
			if(*$1 == "void") {
			    tmp_decl->setType(new Void);
			} else if(*$1 == "char") {
			    tmp_decl->setType(new Char);
			} else {
			    tmp_decl->setType(new Int);
			}
			tmp_decl = tmp_decl->getNextListItem().get();
		    }

		    delete $1;
		};

DeclarationSpec:
		T_VOID { $$ = new std::string("void"); }
	|	T_CHAR { $$ = new std::string("char"); }
	|	T_SCHAR { $$ = new std::string("char"); }
	|	T_UCHAR { $$ = new std::string("char"); }
	|	T_SSINT { $$ = new std::string("int"); }
	|	T_USINT { $$ = new std::string("int"); }
	|	T_LINT { $$ = new std::string("int"); }
	|	T_ULINT { $$ = new std::string("int"); }
	|	T_UINT { $$ = new std::string("int"); }
	|	T_SINT { $$ = new std::string("int"); }
		;

InitDeclaratorList:
		InitDeclarator { $$ = $1; }
	|       InitDeclaratorList T_CMA InitDeclarator { $3->linkListDeclaration($$); $$ = $3; }
		;

InitDeclarator:	Declarator { $$ = $1; }
		    |	Declarator T_EQ AssignmentExpression { $$->setInitializer($3); }
		;

Declarator:	DirectDeclarator { $$ = $1; }
	|	T_MULT DirectDeclarator { $$ = $2; }
		;

DirectDeclarator:
		T_IDENTIFIER { $$ = new Declaration(*$1); delete $1; }
	|	T_LRB Declarator T_RRB { $$ = $2; }
	|	DirectDeclarator T_LSB ConditionalExpression T_RSB { $$ = $1; }
	|	DirectDeclarator T_LSB T_RSB { $$ = $1; }
	|	DirectDeclarator T_LRB T_RRB { $$ = $1; }
	|	DirectDeclarator T_LRB ParameterList T_RRB { $1->linkDeclaration($3); $$ = $1; }
	|	DirectDeclarator T_LRB IdentifierList T_RRB { $$ = $1; }
		;

IdentifierList:	T_IDENTIFIER { $$ = new Declaration(); }
	|	IdentifierList T_CMA T_IDENTIFIER { $$ = new Declaration(); }

// Statement

StatementList:
		Statement { $$ = $1; }
	|	StatementList Statement { $2->linkStatement($$); $$ = $2; }
		;

Statement:	CompoundStatement { $$ = $1; }
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
		T_IF T_LRB Expression T_RRB Statement { $$ = new SelectionStatement($3, $5); }
	|	T_IF T_LRB Expression T_RRB Statement T_ELSE Statement { $$ = new SelectionStatement($3, $5, $7); }
		;

ExpressionStatement:
		T_SC { $$ = new ExpressionStatement(); }
|	Expression T_SC { $$ = new ExpressionStatement($1); }
		;

JumpStatement:	T_RETURN Expression T_SC { $$ = new JumpStatement($2); }
		;

IterationStatement:
		T_WHILE T_LRB Expression T_RRB Statement { $$ = new WhileLoop($3, $5); }
	|	T_DO Statement T_WHILE T_LRB Expression T_RRB T_SC { $$ = $2; }
	|	T_FOR T_LRB Expression T_SC Expression T_SC Expression T_RRB Statement
		{ $$ = new ForLoop($3, $5, $7, $9); }
		;

// Expressions

Expression:	AssignmentExpression { $$ = $1; }
		;

AssignmentExpression:
		ConditionalExpression { $$ = $1; }
	|	UnaryExpression ASSIGN_OPER AssignmentExpression { $$ = new AssignmentExpression($1, $3); }
		;

ASSIGN_OPER:	T_ASSIGN_OPER { ; }
	|	T_EQ { ; }
	;

ConditionalExpression:
		LogicalOrExpression { $$ = $1; }
	|	LogicalOrExpression T_QU Expression T_COL ConditionalExpression
		{ $$ = new ConditionalExpression($1, $3, $5); }
		;

LogicalOrExpression:
		LogicalAndExpression { $$ = $1; }
	|	LogicalOrExpression T_LOG_OR LogicalAndExpression { $$ = new LogicalOrExpression($1, $3); }
		;

LogicalAndExpression:
		InclusiveOrExpression { $$ = $1; }
	|	LogicalAndExpression T_LOG_AND InclusiveOrExpression { $$ = new LogicalAndExpression($1, $3); }
		;

InclusiveOrExpression:
		ExclusiveOrExpression { $$ = $1; }
	|	InclusiveOrExpression T_OR ExclusiveOrExpression { $$ = new InclusiveOrExpression($1, $3); }
		;

ExclusiveOrExpression:
		AndExpression { $$ = $1; }
	|	ExclusiveOrExpression T_XOR AndExpression { $$ = new ExclusiveOrExpression($1, $3); }
		;

AndExpression:	EqualityExpression { $$ = $1; }
	|	AndExpression T_AND EqualityExpression { $$ = new AndExpression($1, $3); }
		;

EqualityExpression:
	        RelationalExpression { $$ = $1; }
	|	EqualityExpression T_EQUALITY_OP RelationalExpression
		{ $$ = new EqualityExpression($1, *$2, $3); delete $2; }
		;

RelationalExpression:
		ShiftExpression { $$ = $1; }
	|       RelationalExpression T_REL_OP ShiftExpression
		{ $$ = new RelationalExpression($1, *$2, $3); delete $2; }
		;

ShiftExpression:
		AdditiveExpression { $$ = $1; }
	|	ShiftExpression T_SHIFT_OP AdditiveExpression
		{ $$ = new ShiftExpression($1, *$2, $3); }
		;

AdditiveExpression:
		MultiplicativeExpression { $$ = $1; }
	|	AdditiveExpression T_ADDSUB_OP MultiplicativeExpression
		{ $$ = new AdditiveExpression($1, *$2, $3); delete $2; }
		;

MultiplicativeExpression:
		CastExpression { $$ = $1; }
	|	MultiplicativeExpression MultDivRemOP CastExpression
		{ $$ = new MultiplicativeExpression($1, *$2, $3); delete $2; }
		;

MultDivRemOP:	T_MULT { $$ = $1; }
	|	T_DIV { $$ = $1; }
	|	T_REM { $$ = $1; }
		;

CastExpression:	UnaryExpression { $$ = $1; }
	|	T_LRB DeclarationSpec T_RRB CastExpression
		{
		    if(*$2 == "int") {
			$$ = new CastExpression(new Int, $4);
		    } else if(*$2 == "char") {
			$$ = new CastExpression(new Char, $4);
		    } else {
			$$ = new CastExpression(new Void, $4);
		    }

		    delete $2;
		}
		;

UnaryExpression:
		PostfixExpression { $$ = $1; }
	|	T_INCDEC UnaryExpression { $$ = $2; }
	|	UnaryOperator CastExpression { $$ = $2; }
	|	T_SIZEOF UnaryExpression { $$ = $2; }
	|	T_SIZEOF T_LRB DeclarationSpec T_RRB { $$ = new Constant(0); }
		;

UnaryOperator:	T_AND { $$ = $1; }
	|	T_ADDSUB_OP { $$ = $1; }
	|	T_MULT { $$ = $1; }
	|	T_TILDE { $$ = $1; }
	|	T_NOT { $$ = $1; }
		;

PostfixExpression:
		PrimaryExpression { $$ = $1; }
	|	PostfixExpression T_LSB Expression T_RSB { $$ = new PostfixArrayElement(); }
	|	PostfixExpression T_LRB PostfixExpression2 { $$ = $3; $$->setPostfixExpression($1); }
	|	PostfixExpression T_DOT T_IDENTIFIER { $$ = $1; }
	|	PostfixExpression T_ARROW T_IDENTIFIER { $$ = $1; }
	|	PostfixExpression T_INCDEC { $$ = $1; }
		;

PostfixExpression2:
		T_RRB { $$ = new PostfixFunctionCall(); }
	|	ArgumentExpressionList T_RRB { $$ = new PostfixFunctionCall($1); }
		;

ArgumentExpressionList:
		AssignmentExpression { $$ = $1; }
	|	ArgumentExpressionList T_CMA AssignmentExpression { $3->linkExpression($$);$$ = $3; }
		;

PrimaryExpression:
		T_IDENTIFIER { $$ = new Identifier(*$1); delete $1; }
	|       Constant { $$ = $1; }
	|	T_LRB Expression T_RRB { $$ = $2; }
		;

Constant:	T_INT_CONST { $$ = new Constant($1); }
		;

%%

Node* g_root; // Definition of variable (to match declaration earlier)

Node* parseAST() {
    g_root = 0;
    yyparse();
    return g_root;
}
