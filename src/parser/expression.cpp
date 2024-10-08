//
// Created by kiran on 5/20/24.
//

#include <parser.h>
#include <memory>

using namespace std;

unique_ptr<ASTExpr> Parser::parse_primary_parens() {
  nextToken();
  auto expr = parse_expression();
  if (tk.type != BASK_TT_Punctuation_RParen) {
    parserError("Unmatched, parsing primary expression");
  }
  nextToken();
  return expr;
}

unique_ptr<ASTExprFuncCall> Parser::parse_function_call() {
  string name = tk.value;
  unsigned int line = tk.line;
  unsigned int col = tk.col;
  
  nextToken();
  unique_ptr<vector<unique_ptr<ASTExpr>>> args = make_unique<vector<unique_ptr<ASTExpr>>>();
  while (tk.type != BASK_TT_Punctuation_RParen) {
    nextToken();
    args->push_back(parse_expression());
  }
  nextToken();
  return make_unique<ASTExprFuncCall>(name, std::move(args), line, col);
}

unique_ptr<ASTExpr> Parser::parse_primary() {
  unique_ptr<ASTExpr> return_primary;
  switch (tk.type) {
    case BASK_TT_Literal_Int:
      return_primary = make_unique<ASTExprConstantValue>(BASK_INT_CONST, tk.value, tk.line, tk.col);
      nextToken();
      return return_primary;
    case BASK_TT_Literal_Float:
      return_primary = make_unique<ASTExprConstantValue>(BASK_FLOAT_CONST, tk.value, tk.line, tk.col);
      nextToken();
      return return_primary;
    case BASK_TT_Literal_Bool:
      return_primary = make_unique<ASTExprConstantValue>(BASK_BOOL_CONST, tk.value, tk.line, tk.col);
      nextToken();
      return return_primary;
    case BASK_TT_Literal_Char:
      return_primary = make_unique<ASTExprConstantValue>(BASK_CHAR_CONST, tk.value, tk.line, tk.col);
      nextToken();
      return return_primary;
    case BASK_TT_Literal_String:
      return_primary = make_unique<ASTExprConstantValue>(BASK_STRING_CONST, tk.value, tk.line, tk.col);
      nextToken();
      return return_primary;
      
    case BASK_TT_Identifier:
      if (peek(1).type == BASK_TT_Punctuation_LParen) {
        return parse_function_call();
      } else if (peek(1).type == BASK_TT_Punctuation_LBracket) {
        parserError("Array access not implemented");
        // return parseArrayAccess();
      } else if (peek(1).type == BASK_TT_Operator_Assign) {
        parserError("Assignment not implemented");
        // return parse_assignment();
      }
      return_primary = make_unique<ASTExprIdentifier>(tk.value, tk.line, tk.col);
      nextToken();
      return return_primary;
    case BASK_TT_Punctuation_LParen:
      return parse_primary_parens();
    default:
      parserError("Expected primary expression");
  }
}

#define IS_UNARY_OP(type) (type == BASK_TT_Operator_BitwiseNot || type == BASK_TT_Operator_LogicalNot || type == BASK_TT_Operator_Sub)
#define IS_BINARY_OP(type) (type == BASK_TT_Operator_Add || type == BASK_TT_Operator_Sub || type == BASK_TT_Operator_Mul || type == BASK_TT_Operator_Div || type == BASK_TT_Operator_Mod || type == BASK_TT_Operator_BitwiseAnd || type == BASK_TT_Operator_BitwiseOr || type == BASK_TT_Operator_BitwiseXor || type == BASK_TT_Operator_LogicalAnd || type == BASK_TT_Operator_LogicalOr || type == BASK_TT_Operator_Equal || type == BASK_TT_Operator_NotEqual || type == BASK_TT_Operator_Less || type == BASK_TT_Operator_LessEqual || type == BASK_TT_Operator_Greater || type == BASK_TT_Operator_GreaterEqual || type == BASK_TT_Operator_Shl || type == BASK_TT_Operator_Shr)

unique_ptr<ASTExpr> Parser::parse_unary_expression() {
  BASK_Token op = tk;
  nextToken();
  return make_unique<ASTExprUnary>(op.type, parse_primary(), op.line, op.col);
}

unique_ptr<ASTExpr> Parser::parse_binary_expression(unique_ptr<ASTExpr> LHS, int min_precedence) {
  while (true) {
    int precedence = get_operator_precedence(tk.type);
    if (precedence < min_precedence) {
      return LHS;
    }
    
    BASK_Token op = tk;
    nextToken();
    
    unique_ptr<ASTExpr> RHS;
    
    if (IS_UNARY_OP(tk.type)) {
      RHS = parse_unary_expression();
    } else {
      RHS = parse_primary();
    }
    
    int next_precedence = get_operator_precedence(tk.type);
    if (precedence < next_precedence) {
      RHS = parse_binary_expression(std::move(RHS), precedence + 1);
    }
    
    LHS = make_unique<ASTExprBinary>(std::move(LHS), std::move(RHS), op.type, op.line, op.col);
  }
}

unique_ptr<ASTExpr> Parser::parse_expression() {
  if (IS_UNARY_OP(tk.type)) {
    return parse_binary_expression(std::move(parse_unary_expression()), 0);
  } else {
    return parse_binary_expression(std::move(parse_primary()), 0);
  }
}