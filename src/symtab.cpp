//
// Created by kiran on 5/21/24.
//

#include "symtab.h"
#include "types.h"

using namespace std;

bool SymTab::name_is_in_scope(std::string name) {
  for (auto scope = scope_stack.rbegin(); scope != scope_stack.rend();
       scope++) {
    std::unordered_map<std::string, BASK_Type>::iterator value =
        scope->find(name);

    if (value != scope->end()) {
      return true;
    }
  }

  return false;
}

optional<BASK_Type> SymTab::get_name_type(std::string name) {
  for (auto scope = scope_stack.rbegin(); scope != scope_stack.rend(); scope++) {
    std::unordered_map<std::string, BASK_Type>::iterator value = scope->find(name);

    if (value != scope->end()) {
      return value->second;
    }
  }

  return nullopt;
}

void SymTab::add_name(std::string name, BASK_Type type) {
  scope_stack.back().insert({name, type});
}

void SymTab::enter_block() {
  scope_stack.emplace_back();
}

void SymTab::exit_block() {
  scope_stack.pop_back();
}
