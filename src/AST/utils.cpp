//
// Created by kiran on 5/21/24.
//

#include "AST.h"
#include <iostream>

void printIndent(int indent, ostream &out = std::cout) {
  for (int i = 0; i < indent; i++) {
    out << "  ";
  }
}