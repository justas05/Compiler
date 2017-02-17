%option noyywrap

%{
// Avoid error "error: fileno was not declared in this scope"
extern "C" int fileno(FILE *stream);

#include "c_parser.tab.hpp"

%}

KEYWORD auto|double|int|struct|break|else|long|switch|case|enum|register|typedef|char|extern|return|union|const|float|short|unsigned|continue|for|signed|void|default|goto|sizeof|volatile|do|if|static|while

IDENTIFIER [_a-zA-Z][_a-zA-Z0-9]*

OPERATOR [.][.][.]|[<>][<>][=]|[-][-]|[+][+]|[|][|]|[#][#]|[&][&]|[+\-*\/<>=!%^|&][=]|[<][<]|[->][>]|[<>&=+\/\-*(){}\[\]\.,%~!?:|^;]

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

{IDENTIFIER}	{ yylval.string = new std::string(yytext); return T_IDENTIFIER; }

[;]		{ return T_SC; }
[=]		{ return T_EQ; }
[,]		{ return T_CMA; }
[(]		{ return T_LRB; }
[)]		{ return T_RRB; }
[{]               { return T_LCB; }
[}]               { return T_RCB; }

({HEXCONSTANT}|{OCTALCONSTANT}|{DECIMALCONSTANT}){INTEGERSUFFIX}?	{ return T_INT_CONST; }

{WHITESPACE}		{ ; }

.                   { fprintf(stderr, "Invalid token\n"); exit(1); }

%%

void yyerror(char const *s) {
     fprintf (stderr, "Parse error : %s\n", s);
     exit(1);
}
