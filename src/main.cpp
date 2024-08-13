#include "codegen.h"
#include <iostream>
#include <fstream>
#include <options.h>
#include <parser.h>
#include <lexer.h>

#include <llvm/Support/raw_os_ostream.h>

int main(int argc, const char **argv) {
  CommandLineArguments options(argc, argv);
  Lexer lexer(options);
  Parser parser(lexer);
  
  auto ast = parser.parse();
  ast->fold_expressions();
  ast->check_semantics();

  ostream &out = options.out.empty() ? std::cout : *(new ofstream(options.out));

  if (options.mode == CommandLineArguments::Mode::AST) {
    ast->print(0, out);
    return 0;
  }

  KLCodeGenVisitor visitor("Main");
  ast->accept(&visitor);

  if (options.mode == CommandLineArguments::Mode::IR) {
    if (options.out.empty()) {
      visitor.printModule();
    } else {
      llvm::raw_os_ostream llvm_stream(out);

      visitor.printModule(llvm_stream);
    }
  }
}
