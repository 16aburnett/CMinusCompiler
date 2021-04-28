// Filename:   Lexer.h
// Name:       Amy Burnett and Lauren Deaver 
// Course:     CSCI 435 - Compilers
// Assignment: A10CMinusSymbolTree
// Date:       TBD
//========================================================================

#ifndef LEXER_H
#define LEXER_H

//========================================================================

#include <string>

//========================================================================

enum TokenType
{
  // Special tokens
  END_OF_FILE, ERROR,

  // Keywords
  IF, ELSE, INT, VOID, RETURN, WHILE,

  // Operators
  PLUS, MINUS, TIMES, DIVIDE, LT, LTE, GT, GTE, EQ, NEQ, ASSIGN,

  // Punctuators
  SEMI, COMMA, LPAREN, RPAREN, LBRACK, RBRACK, LBRACE, RBRACE,

  // Identifier and integer literals
  ID, NUM
};

//========================================================================

const char* tokenToString (TokenType token);

//========================================================================

struct Token
{
  Token (TokenType pType = END_OF_FILE,
         std::string pLexeme = "",
         int pLineno = 1,
         int pColumnno = 1,
         int pNumber = 0)
    : type (pType), 
      lexeme (pLexeme), 
      lineno (pLineno), 
      columnno (pColumnno),
      number (pNumber)
  {  }
  
  TokenType   type;
  std::string lexeme;
  int         lineno;
  int         columnno; 
  int         number;
};

//========================================================================

class Lexer
{
public:
  Lexer (FILE* srcFile);

  Token
  getToken ();

  int 
  getLineNum ();

  int 
  getColumnNum ();

private:
  int
  getChar ();

  void
  ungetChar (int c);

  Token
  getId ();

  Token
  getNum ();

private:
  FILE* m_srcFile;
  int   m_lineNum;
  int   m_columnNum;
};

//========================================================================

#endif
