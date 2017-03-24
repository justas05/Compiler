%code requires{

#include "node.hpp"
#include "translation_unit.hpp"
#include "function.hpp"
#include "declaration.hpp"
#include "statement.hpp"
#include "expression.hpp"
#include "type.hpp"
    
    
extern Node* g_root; // A way of getting the AST out
void setTypeInformation(Type* type_ptr, std::string type_str);

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
			T_VOID T_CHAR T_SHORT T_INT T_LONG T_FLOAT T_DOUBLE T_SIGNED T_UNSIGNED
			T_TYPEDEF T_EXTERN T_STATIC T_AUTO T_REGISTER
			T_CONST T_VOLATILE T_GOTO T_BREAK T_CONTINUE
			T_CASE T_DEFAULT T_SWITCH
			
%nonassoc		T_RRB
%nonassoc		T_ELSE
			
			
%type	<node>		ExternalDeclaration
			
%type	<trans_unit>	TranslationUnit
			
%type	<function>	FunctionDefinition

%type	<statement>	StatementList Statement CompoundStatement CompoundStatement_2
			SelectionStatement
			ExpressionStatement JumpStatement IterationStatement LabeledStatement

%type	<declaration>	ParameterList Parameter DeclarationList Declaration InitDeclaratorList
			InitDeclarator
			IdentifierList
			Declarator DirectDeclarator

%type	<expression>	Expression AssignmentExpression ConditionalExpression LogicalOrExpression
			LogicalAndExpression InclusiveOrExpression ExclusiveOrExpression
			AndExpression EqualityExpression RelationalExpression ShiftExpression
			AdditiveExpression MultiplicativeExpression CastExpression UnaryExpression
			PostfixExpression PostfixExpression2 ArgumentExpressionList PrimaryExpression
			Constant Initializer InitializerList

%type	<type>		DeclarationSpecifierList Pointer

%type	<number>        T_INT_CONST
			
%type	<string>	T_IDENTIFIER ASSIGN_OPER T_ASSIGN_OPER T_EQ T_AND T_ADDSUB_OP T_TILDE T_NOT
			T_MULT T_DIV T_REM T_EQUALITY_OP T_REL_OP T_SHIFT_OP T_INCDEC MultDivRemOP
			UnaryOperator DeclarationSpecifier TypeQualifier TypeQualifierList
                        
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
		DeclarationSpecifierList Declarator CompoundStatement
		{ $$ = new Function($2->getId(), $3, $2->getNext()); delete $1; }
		;

ParameterList:
		Parameter { $$ = $1; }
	|       ParameterList T_CMA Parameter { $3->linkDeclaration($$); $$ = $3; }
		;

Parameter:	DeclarationSpecifierList Declarator { $$ = new Declaration($2->getId()); delete $1; }
	|	DeclarationSpecifierList { $$ = new Declaration(""); }
	|	DeclarationSpecifierList T_MULT { $$ = new Declaration(""); delete $2; }
		;

// Declaration

DeclarationList:
		Declaration { $$ = $1; }
	|	DeclarationList Declaration { $2->linkDeclaration($$); $$ = $2; }
		;

Declaration:	DeclarationSpecifierList InitDeclaratorList T_SC
		{
		    $$ = $2;
		    Declaration *tmp_decl = $2;
		    std::shared_ptr<Type> tmp_type;
		    if(tmp_decl->getType() == nullptr)
			tmp_type = std::make_shared<TypeContainer>();
		    else
			tmp_type = tmp_decl->getType();
		    
		    while(tmp_decl != nullptr) {
			tmp_type->type($1->type());
			tmp_decl->setType(tmp_type);
			tmp_decl = tmp_decl->getNextListItem().get();
		    }

		    delete $1;
		};

DeclarationSpecifierList:
		DeclarationSpecifier
		{
		    $$ = new TypeContainer();
		    setTypeInformation($$, *$1);
		    delete $1;
		}
	|	DeclarationSpecifierList DeclarationSpecifier
		{ setTypeInformation($$, *$2); delete $2; }
		;

DeclarationSpecifier:
		T_VOID { $$ = new std::string("void"); }
	|	T_CHAR { $$ = new std::string("char"); }
	|	T_SHORT { $$ = new std::string("short"); }
	|	T_INT { $$ = new std::string("int"); }
	|	T_LONG { $$ = new std::string("long"); }
	|	T_FLOAT { $$ = new std::string("float"); }
	|	T_DOUBLE { $$ = new std::string("double"); }
	|	T_SIGNED { $$ = new std::string("signed"); }
	|	T_UNSIGNED { $$ = new std::string("unsigned"); }
	|	T_TYPEDEF { $$ = new std::string("typedef"); }
	|	T_EXTERN { $$ = new std::string("extern"); }
	|	T_STATIC { $$ = new std::string("static"); }
	|	T_AUTO { $$ = new std::string("auto"); }
	|	T_REGISTER { $$ = new std::string("register"); }
	|	T_CONST { $$ = new std::string("const"); }
	|	T_VOLATILE { $$ = new std::string("volatile"); }
		;

InitDeclaratorList:
		InitDeclarator { $$ = $1; }
	|       InitDeclaratorList T_CMA InitDeclarator { $3->linkListDeclaration($$); $$ = $3; }
		;

InitDeclarator:	Declarator { $$ = $1; }
	|	Declarator T_EQ Initializer { $$ = $1; $$->setInitializer($3); delete $2; }
		;

Declarator:	DirectDeclarator { $$ = $1; }
	|	Pointer DirectDeclarator { $$ = $2; std::shared_ptr<Type> tmp($1); $$->setType(tmp); }
		;

Pointer:
		T_MULT { $$ = new Pointer(); delete $1; }
	| 	T_MULT Pointer { $$ = $2; delete $1; }
	| 	T_MULT TypeQualifierList Pointer { $$ = $3; delete $1; delete $2; }
		;

TypeQualifierList:
		TypeQualifier { $$ = $1; }
	| 	TypeQualifierList TypeQualifier { $$ = $2; delete $1; }
		;

TypeQualifier:
		T_CONST { $$ = new std::string("const"); }
	| 	T_VOLATILE { $$ = new std::string("volatile"); }
		;

DirectDeclarator:
		T_IDENTIFIER { $$ = new Declaration(*$1); delete $1; }
	|	T_LRB Declarator T_RRB { $$ = $2; }
|	DirectDeclarator T_LSB ConditionalExpression T_RSB { $$ = new ArrayDeclaration($1->getId(), $1->getInitializer(), $3->constantFold()); }
	|	DirectDeclarator T_LSB T_RSB { $$ = $1; }
	|	DirectDeclarator T_LRB T_RRB { $$ = $1; $$->setExternDeclaration(true); }
	|	DirectDeclarator T_LRB ParameterList T_RRB { $1->linkDeclaration($3); $$ = $1; $$->setExternDeclaration(true); }
	|	DirectDeclarator T_LRB IdentifierList T_RRB { $$ = $1; $$->setExternDeclaration(true); }
		;

IdentifierList:	T_IDENTIFIER { $$ = new Declaration(); }
	|	IdentifierList T_CMA T_IDENTIFIER { $$ = new Declaration(); }

Initializer: 	AssignmentExpression { $$ = $1; }
	|	T_LCB InitializerList T_RCB { $$ = $2; }
	|	T_LCB InitializerList T_CMA T_RCB { $$ = $2; }
		;

InitializerList:
		Initializer { $$ = $1; }
	|	InitializerList T_CMA Initializer { $3->linkExpression($$); $$ = $3; }

// Statement

StatementList:
		Statement { $$ = $1; }
	|	StatementList Statement { $2->linkStatement($$); $$ = $2; }
		;

Statement: 	LabeledStatement { $$ = $1; }
	| 	CompoundStatement { $$ = $1; }
	|	SelectionStatement { $$ = $1; }
	|	ExpressionStatement { $$ = $1; }
	|       JumpStatement { $$ = $1; }
	|	IterationStatement { $$ = $1; }
		;

LabeledStatement:
		T_IDENTIFIER T_COL Statement { $$ = new LabelStatement(*$1, $3); }
	|	T_CASE ConditionalExpression T_COL Statement { $$ = new CaseStatement($4, $2); }
	|	T_DEFAULT T_COL Statement { $$ = new CaseStatement($3, nullptr, true); }
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
		T_IF T_LRB Expression T_RRB Statement { $$ = new IfElseStatement($3, $5); }
	|	T_IF T_LRB Expression T_RRB Statement T_ELSE Statement { $$ = new IfElseStatement($3, $5, $7); }
	|	T_SWITCH T_LRB Expression T_RRB Statement { $$ = new SwitchStatement($3, $5); }
		;

ExpressionStatement:
		T_SC { $$ = new ExpressionStatement(); }
|	Expression T_SC { $$ = new ExpressionStatement($1); }
		;

JumpStatement:	T_RETURN Expression T_SC { $$ = new ReturnStatement($2); }
	|	T_BREAK T_SC { $$ = new BreakStatement(); }
	|	T_CONTINUE T_SC { $$ = new ContinueStatement(); }
	|	T_GOTO T_IDENTIFIER { $$ = new GotoStatement(*$2); }
		;

IterationStatement:
		T_WHILE T_LRB Expression T_RRB Statement { $$ = new WhileLoop($3, $5); }
	|	T_DO Statement T_WHILE T_LRB Expression T_RRB T_SC
		{ $$ = new WhileLoop($5, $2, false); }
	|	T_FOR T_LRB Expression T_SC Expression T_SC Expression T_RRB Statement
		{ $$ = new ForLoop($3, $5, $7, $9); }
		;

// Expressions

Expression:	AssignmentExpression { $$ = $1; }
		;

AssignmentExpression:
		ConditionalExpression { $$ = $1; }
	|	UnaryExpression ASSIGN_OPER AssignmentExpression
		{
		    OperationExpression* tmp;
		    if(*$2 == "=") {
			$$ = new AssignmentExpression($1, $3);
		    } else if(*$2 == "+=") {
			tmp = new AdditiveExpression($1, "+", $3);
			$$ = new AssignmentExpression(tmp->getLhs(), tmp);
		    } else if(*$2 == "-=") {
			tmp = new AdditiveExpression($1, "-", $3);
			$$ = new AssignmentExpression(tmp->getLhs(), tmp);
		    } else if(*$2 == "*=") {
			tmp = new MultiplicativeExpression($1, "*", $3);
			$$ = new AssignmentExpression(tmp->getLhs(), tmp);
		    } else if(*$2 == "/=") {
			tmp = new MultiplicativeExpression($1, "/", $3);
			$$ = new AssignmentExpression(tmp->getLhs(), tmp);
		    } else if(*$2 == "%=") {
			tmp = new MultiplicativeExpression($1, "%", $3);
			$$ = new AssignmentExpression(tmp->getLhs(), tmp);
		    } else if(*$2 == "&=") {
			tmp = new AndExpression($1, $3);
			$$ = new AssignmentExpression(tmp->getLhs(), tmp);
		    } else if(*$2 == "^=") {
			tmp = new ExclusiveOrExpression($1, $3);
			$$ = new AssignmentExpression(tmp->getLhs(), tmp);
		    } else if(*$2 == "|=") {
			tmp = new InclusiveOrExpression($1, $3);
			$$ = new AssignmentExpression(tmp->getLhs(), tmp);
		    } else if(*$2 == "<<=") {
			tmp = new ShiftExpression($1, "<<", $3);
			$$ = new AssignmentExpression(tmp->getLhs(), tmp);
		    } else {
			tmp = new ShiftExpression($1, ">>", $3);
			$$ = new AssignmentExpression(tmp->getLhs(), tmp);
		    }
		    
		    delete $2;
		}
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
	|	T_LRB DeclarationSpecifierList T_RRB CastExpression
		{ new CastExpression($2, $4); delete $2; }
		;

UnaryExpression:
		PostfixExpression { $$ = $1; }
	|	T_INCDEC UnaryExpression { $$ = new UnaryPreIncDecExpression(*$1, $2); delete $1; }
	|	UnaryOperator CastExpression { $$ = new OperatorUnaryExpression(*$1, $2); delete $1; }
	|	T_SIZEOF UnaryExpression { $$ = $2; }
	|	T_SIZEOF T_LRB DeclarationSpecifierList T_RRB { $$ = new Constant(0); delete $3; }
		;

UnaryOperator:	T_AND { $$ = $1; }
	|	T_ADDSUB_OP { $$ = $1; }
	|	T_MULT { $$ = $1; }
	|	T_TILDE { $$ = $1; }
	|	T_NOT { $$ = $1; }
		;

PostfixExpression:
		PrimaryExpression { $$ = $1; }
	|	PostfixExpression T_LSB Expression T_RSB { $$ = new PostfixArrayElement($1, $3); }
	|	PostfixExpression T_LRB PostfixExpression2
		{
		    $$ = $3;
		    PostfixFunctionCall *tmp = dynamic_cast<PostfixFunctionCall *>($$);
		    if(tmp != nullptr)
			tmp->setPostfixExpression($1);
		}
	|	PostfixExpression T_DOT T_IDENTIFIER { $$ = $1; }
	|	PostfixExpression T_ARROW T_IDENTIFIER { $$ = $1; }
	|	PostfixExpression T_INCDEC
		{ $$ = new PostfixPostIncDecExpression(*$2, $1); delete $2; }
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

Node* parseAST()
{
    g_root = 0;
    yyparse();
    return g_root;
}

void setTypeInformation(Type* type_ptr, std::string type_str)
{
    if(type_str == "void") {
	type_ptr->type(new Void());
    } else if(type_str == "char") {
	type_ptr->type(new Char());
	type_ptr->setSize(8);
    } else if(type_str == "short") {
	type_ptr->type(new Int());
	type_ptr->setSize(16);
    } else if(type_str == "int") {
	type_ptr->type(new Int());
    } else if(type_str == "long") {
	type_ptr->type(new Int());
    } else if(type_str == "float") {
	type_ptr->type(new Float());
    } else if(type_str == "double") {
	type_ptr->type(new Float());
    } else if(type_str == "signed") {
	type_ptr->type(new Int());
	type_ptr->setSigned(true);
    } else if(type_str == "unsigned") {
	type_ptr->type(new Int());
	type_ptr->setSigned(false);
    } else if(type_str == "typedef") {
	// TODO do typedef
    } else if(type_str == "extern") {
	type_ptr->setExtern(true);
    } else if(type_str == "static") {
	type_ptr->setStatic(true);
    } else if(type_str == "auto") {
	// TODO do auto
    } else if(type_str == "register") {
	// TODO do register
    } else if(type_str == "const") {
	type_ptr->setConst(true);
    } else if(type_str == "volatile") {
	// TODO do volatile
    } else {
			
    }
}
