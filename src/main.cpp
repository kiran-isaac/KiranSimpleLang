#include <iostream>
#include <options.h>
#include <parser.h>
#include <lexer.h>

int main(int argc, const char **argv) {
  Options options(argc, argv);
  Lexer lexer(options);
  Parser parser(lexer);
  
  auto ast = parser.parse();
  ast->print(0);
  
  return 0;
}