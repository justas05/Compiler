#include "c_lexer.hpp"

std::string toJson(const std::string& classType, const std::string& text,
		   const std::string& strLine, const std::string& srcCol,
		   const std::string& srcLine, const std::string& fName) {
  std::string tmp = "{\"Class\":\"" + classType + "\", \"Text\":\"" + text +
    "\", \"StreamLine\":" + strLine + ", \"SourceFile\":\"" + fName +
    "\", \"SourceLine\":" + srcLine + ", \"SourceCol\":" + srcCol + "}";

  return tmp;
}
