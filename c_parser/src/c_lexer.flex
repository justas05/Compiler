%option noyywrap

%{

#include "c_parser.tab.hpp"

%}

KEYWORD auto|double|int|struct|break|else|long|switch|case|enum|register|typedef|char|extern|return|union|const|float|short|unsigned|continue|for|signed|void|default|goto|sizeof|volatile|do|if|static|while

IDENTIFIER [_a-zA-Z][_a-zA-Z0-9]*

OPERATOR [.][.][.]|[<>][<>][=]|[-][-]|[+][+]|[|][|]|[#][#]|[&][&]|[+\-*\/<>=!%^|&][=]|[<][<]|[->][>]|[<>&=+\/\-*(){}\[\]\.,%~!?:|^;]

ASSIGNMENT_OPERATOR (([<>][<>]|[*\/%+\-&^|])[=]|[=])

FRACTIONALCONSTANT (([0-9]*\.[0-9]+)|([0-9]+\.))
EXPONENTPART ([eE][+-]?[0-9]+)

FLOATINGSUFFI X ([flFL])
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

typedef|extern|static|auto|register	{ return T_STRG_SPEC; }
void|char|short|int|long|float|double|signed|unsigned	      { return T_TYPE_SPEC; }
const|volatile						      { return T_TYPE_QUAL; }

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
[*] 		{ return T_MULT; }
[\/]		{ return T_DIV; }
[%]		{ return T_REM; }
[~]		{ return T_TILDE; }
[!]		{ return T_NOT; }
[.]		{ return T_DOT; }
[-][>]		{ return T_ARROW; }
[+-][+-]	{ return T_INCDEC; }
[+-]		{ return T_ADDSUB_OP; }
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
