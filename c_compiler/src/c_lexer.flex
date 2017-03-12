%option noyywrap

%{

#include "c_parser.tab.hpp"

%}

IDENTIFIER [_a-zA-Z][_a-zA-Z0-9]*

ASSIGNMENT_OPERATOR (([<>][<>]|[*\/%+\-&^|])[=]|[=])

INTEGERSUFFIX ([uU][lL]|[lL][uU]|[uUlL])

DECIMALCONSTANT ([1-9][0-9]*)
OCTALCONSTANT ([0][0-7]*)
HEXCONSTANT ([0][xX][0-9A-Fa-f]+)

CHARCONSTANT ('(([\\]['])|([^']))+')

STRINGLITERAL ["](([\\]["])|([^"]))*["]

WHITESPACE [ \t\r\n]+

PREPROC [#][ ][0-9]+[ ]{STRINGLITERAL}[ 0-9]*

ALL .

%%

(void)								{ return T_VOID; }
(char) 							      	{ return T_CHAR; }
(signed[ ]char) 						{ return T_SCHAR; }
(unsigned[ ]char) 					      	{ return T_UCHAR; }
((short[ ]int)|(signed[ ]short[ ]int)|short|(signed[ ]short))	{ return T_SSINT; }
((unsigned[ ]short[ ]int)|(unsigned[ ]short))       		{ return T_USINT; }
((signed[ ]long[ ]int)|(signed[ ]long)|(long[ ]int)|long)	{ return T_LINT; }
((unsigned[ ]long[ ]int)|(unsigned[ ]long))  			{ return T_ULINT; }
((unsigned[ ]int)|unsigned)				      	{ return T_UINT; }
((signed[ ]int)|int|signed) 				      	{ return T_SINT; }


[;]		{ return T_SC; }
[,]		{ return T_CMA; }
[(]		{ return T_LRB; }
[)]		{ return T_RRB; }
[{]             { return T_LCB; }
[}]             { return T_RCB; }
[[]		{ return T_LSB; }
[]]		{ return T_RSB; }
[?]		{ return T_QU; }
[:]		{ return T_COL; }
[|][|]		{ return T_LOG_OR; }
[&][&]		{ return T_LOG_AND; }
[|]		{ return T_OR; }
[\^]		{ return T_XOR; }
[&]		{ return T_AND; }
[=][=]		{ return T_EQUALITY_OP; }
[!][=]		{ return T_EQUALITY_OP; }
([<>][=])|[<>]	{ return T_REL_OP; }
[<>][<>]	{ return T_SHIFT_OP; }
[*] 		{ yylval.string = new std::string(yytext); return T_MULT; }
[\/]		{ yylval.string = new std::string(yytext); return T_DIV; }
[%]		{ yylval.string = new std::string(yytext); return T_REM; }
[~]		{ return T_TILDE; }
[!]		{ return T_NOT; }
[.]		{ return T_DOT; }
[-][>]		{ return T_ARROW; }
[+-][+-]	{ return T_INCDEC; }
[+-]		{ yylval.string = new std::string(yytext); return T_ADDSUB_OP; }
[=]		{ yylval.string = new std::string(yytext); return T_EQ; }

{ASSIGNMENT_OPERATOR} { yylval.string = new std::string(yytext); return T_ASSIGN_OPER; }

if		{ return T_IF; }
else		{ return T_ELSE; }
return		{ return T_RETURN; }
while		{ return T_WHILE; }
do		{ return T_DO; }
for		{ return T_FOR; }
sizeof		{ return T_SIZEOF; }

{IDENTIFIER}	{ yylval.string = new std::string(yytext); return T_IDENTIFIER; }

({HEXCONSTANT}|{OCTALCONSTANT}|{DECIMALCONSTANT}){INTEGERSUFFIX}?	{ yylval.number = strtol(yytext, NULL, 0); return T_INT_CONST; }

{WHITESPACE}		{ ; }

.                   { fprintf(stderr, "Invalid token\n"); exit(1); }

%%

void yyerror(char const *s) {
     fprintf (stderr, "Parse error : %s\n", s);
     exit(1);
}
