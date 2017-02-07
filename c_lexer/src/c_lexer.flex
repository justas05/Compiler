%option noyywrap

%{

#include "c_lexer.hpp"

#include <sstream>
#include <stdlib.h>

int lineCount = 1;
int spaceCount = 1;
int sourceLineCount = 1;

int charLength = 0;

std::string fileName;

%}

KEYWORD auto|double|int|struct|break|else|long|switch|case|enum|register|typedef|char|extern|return|union|const|float|short|unsigned|continue|for|signed|void|default|goto|sizeof|volatile|do|if|static|while

IDENTIFIER [_a-zA-Z][_a-zA-Z0-9]*

OPERATOR [.][.][.]|[<>][<>][=]|[-][-]|[+][+]|[|][|]|[#][#]|[&][&]|[+\-*\/<>=!%^|&][=]|[<][<]|[->][>]|[<>&=+\/\-*(){}\[\]\.,%~!?:|^;]

FRACTIONALCONSTANT (([0-9]*\.[0-9]+)|([0-9]+\.))
EXPONENTPART ([eE][+-]?[0-9]+)

FLOATINGSUFFIX ([flFL])
INTEGERSUFFIX ([uU][lL]|[lL][uU]|[uUlL])

DECIMALCONSTANT ([1-9][0-9]*)
OCTALCONSTANT ([0][0-7]*)
HEXCONSTANT ([0][xX][0-9A-Fa-f]+)

CHARCONSTANT ('(([\\]['])|([^']))+')

STRINGLITERAL ["](([\\]["])|([^"]))*["]

NEWLINE (\r\n?|\n)

WHITESPACE [ ]

TAB \t

PREPROC [#][ ][0-9]+[ ]{STRINGLITERAL}[ 0-9]*

ALL .

%%

{KEYWORD} {
    yylval = new std::string(yytext);
    charLength = (int)yyleng;
    return Keyword;
}

{IDENTIFIER} {
    yylval = new std::string(yytext);
    charLength = (int)yyleng;
    return Identifier;
}

{OPERATOR} {
    yylval = new std::string(yytext);
    charLength = (int)yyleng;
    return Operator;
}

{FRACTIONALCONSTANT}{EXPONENTPART}?{FLOATINGSUFFIX}? {
    yylval = new std::string(yytext);
    charLength = (int)yyleng;
    return Constant;
}

([0-9]+){EXPONENTPART}{FLOATINGSUFFIX}? {
    yylval = new std::string(yytext);
    charLength = (int)yyleng;
    return Constant;
}

{HEXCONSTANT}{INTEGERSUFFIX}? {
    yylval = new std::string(yytext);
    charLength = (int)yyleng;
    return Constant;
}

{DECIMALCONSTANT}{INTEGERSUFFIX}? {
    yylval = new std::string(yytext);
    charLength = (int)yyleng;
    return Constant;
}

{OCTALCONSTANT}{INTEGERSUFFIX}? {
    yylval = new std::string(yytext);
    charLength = (int)yyleng;
    return Constant;
}

{CHARCONSTANT} {
    std::string tmp(yytext);
    yylval = new std::string(tmp.substr(1, tmp.length()-2));
    charLength = (int)yyleng;
    return Constant;
}

{STRINGLITERAL} {
    std::string tmp(yytext);
    yylval = new std::string(tmp.substr(1, tmp.length()-2));
    charLength = (int)yyleng;
    return StringLiteral;
}

{NEWLINE} {
    spaceCount = 1;
    lineCount++;
    sourceLineCount++;
}

{WHITESPACE} {
    spaceCount++;
}

{PREPROC} {
    int srcLineInt;

    yylval = new std::string(yytext);
    std::stringstream preProcLine((*yylval).substr(1, (*yylval).length()));
    preProcLine >> srcLineInt >> fileName;
    sourceLineCount = srcLineInt - 1;
    fileName = fileName.substr(1, fileName.length() - 2);
}

{TAB} {
    spaceCount += 8;
}

{ALL} {
    yylval = new std::string(yytext);
    charLength = (int)yyleng;
    return Invalid;
}

%%
