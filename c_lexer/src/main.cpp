#include "c_lexer.hpp"

#include <cassert>
#include <sstream>
#include <vector>

std::string *yylval;

int main() {
	printf("[\n");
	std::string sourceLine, streamLine, sourceCol, classType, text;
	while(1) {
		// get the token type and run the lexer
		std::stringstream str_line, src_col, src_line;

		TokenType type = (TokenType)yylex();

		str_line << lineCount;
		streamLine = str_line.str();

		src_col << spaceCount;
		sourceCol = src_col.str();

		src_line << sourceLineCount;
		sourceLine = src_line.str();

		if(type == None) {
			// returns None when the file ends and we want to break then
			break;
		} else if(type == Invalid) {
			// type is Invalid
			text = *yylval;
			classType = "Invalid";
			delete yylval;
		} else if(type == Keyword) {
			// found a keyword
			text = *yylval;
			classType = "Keyword";
			delete yylval;
		} else if(type == Identifier) {
			// found an identifier
			text = *yylval;
			classType = "Identifier";
			delete yylval;
		} else if(type == Operator) {
			// found an operator
			text = *yylval;
			classType = "Operator";
			delete yylval;
		} else if(type == Constant) {
			// found a constant
			text = *yylval;
			classType = "Constant";
			delete yylval;
		} else if(type == StringLiteral) {
			// found a string literal
			text = *yylval;
			classType = "StringLiteral";
			delete yylval;
		} else {
			// if any other type comes we assert to 0
			assert(0);
			return 1;
		}

		printf("%s,\n", toJson(classType, text, streamLine, sourceCol, sourceLine, fileName).c_str());

		spaceCount += yyleng;
	}

	printf("{}\n]\n");

	return 0;
}
