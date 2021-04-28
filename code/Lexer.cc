// Filename:   Lexer.cc
// Name:       Amy Burnett and Lauren Deaver 
// Course:     CSCI 435 - Compilers
// Assignment: A10CMinusSymbolTree
// Date:       TBD
//========================================================================
// System includes

#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>

//========================================================================
// Local includes

#include "Lexer.h"

//========================================================================
// Using declarations

using std::string;
using std::stoi;

//========================================================================

/*
  Converts a TokenType enum to it's string representation 
*/
const char* tokenToString (TokenType token)
{
  switch (token)
  {
    case END_OF_FILE: return "END_OF_FILE";
    case ERROR:       return "ERROR";
    case IF:          return "IF";
    case ELSE:        return "ELSE";
    case INT:         return "INT";
    case VOID:        return "VOID";
    case RETURN:      return "RETURN";
    case WHILE:       return "WHILE";
    case PLUS:        return "PLUS";
    case MINUS:       return "MINUS";
    case TIMES:       return "TIMES";
    case DIVIDE:      return "DIVIDE";
    case LT:          return "LT";
    case LTE:         return "LTE";
    case GT:          return "GT";
    case GTE:         return "GTE";
    case EQ:          return "EQ";
    case NEQ:         return "NEQ";
    case ASSIGN:      return "ASSIGN";
    case SEMI:        return "SEMI";
    case COMMA:       return "COMMA";
    case LPAREN:      return "LPAREN";
    case RPAREN:      return "RPAREN";
    case LBRACK:      return "LBRACK";
    case RBRACK:      return "RBRACK";
    case LBRACE:      return "LBRACE";
    case RBRACE:      return "RBRACE";
    case ID:          return "ID";
    case NUM:         return "NUM";
    default:          return "UNKNOWN";
  }
}

//========================================================================

/*
  Constructs a Lexer that will read tokens from the given file 
*/
Lexer::Lexer (FILE* srcFile)
{
  m_srcFile = srcFile;
  m_lineNum = 1;
  m_columnNum = 1; 
}

//========================================================================

/*
  Tokenizes this parser's srcfile and returns a vector of the tokens
*/
std::vector<Token> 
Lexer::tokenize ()
{
    std::vector<Token> tokens; 

    // process each token until the EOF token is reached 
    do
    {
        tokens.push_back (getToken ());
    } while (tokens.back ().type != END_OF_FILE);

    return tokens; 
}

//========================================================================

/*
  Retrieves the next token from the file stream
  - returns a Token object with the token type, lexeme, and number value
*/
Token
Lexer::getToken () 
{   
  while (true)
  {
    char c = getChar ();

    // Keywords/Identifiers 
    if (isalpha (c))
    {
      // unget the character so getId grabs the entire ID
      ungetChar(c);
      return getId ();
    }

    // Numbers 
    if (isdigit (c))
    {
      // unget the character so getNum grabs the entire num
      ungetChar(c);
      return getNum ();
    }

    switch (c)
    {
    // ignore whitespace
    case '\n':
      ++m_lineNum;
      m_columnNum = 1;
      break;
    case ' ':
      break;
    case '\t':
      break;
    
    // End of input 
    case EOF: return Token (END_OF_FILE, "EOF", m_lineNum, m_columnNum);

    // Operators
    case '+': return Token (PLUS, "+", m_lineNum, m_columnNum-1);
    case '-': return Token (MINUS, "-", m_lineNum, m_columnNum-1);
    case '*': return Token (TIMES, "*", m_lineNum, m_columnNum-1); 
    case '/': 
      // check for c-style comment
      c = getChar();
      if (c == '*')
      {
        while(true)
        {
          c = getChar();
          if (c == '\n')
          {
            ++m_lineNum;
            m_columnNum = 1;
          }
          if (c == '*')
          {
            c = getChar();
            if (c == '/')
            {
              // end of block comment
              // ignore comment
              break;
            }
            // for the case of "**/"
            ungetChar(c);
          }
          if (c == EOF) 
            return Token (END_OF_FILE, "EOF", m_lineNum, m_columnNum);
        }
        // ignore comment
        break; 
      }
      // check for single line comment
      if (c == '/')
      {
        // ignore until newline or EOF
        while (true)
        {
          c = getChar();
          if (c == '\n') break;
          if (c == EOF) 
            return Token (END_OF_FILE, "EOF", m_lineNum, m_columnNum);
        }
        // ignore comment - newline seen
        ++m_lineNum;
        m_columnNum = 1;
        break;
      }
      // simple divide
      ungetChar(c);
      return Token (DIVIDE, "/", m_lineNum, m_columnNum-1);   

    // LT/LTE 
    case '<':
      c = getChar ();
      if (c != '=')
      {
        ungetChar (c);
        return Token (LT, "<", m_lineNum, m_columnNum-1);
      }
      return Token (LTE, "<=", m_lineNum, m_columnNum-2);
    // GT/GTE
    case '>':
      c = getChar ();
      if (c != '=')
      {
        ungetChar (c);
        return Token (GT, ">", m_lineNum, m_columnNum-1);
      }
      return Token (GTE, ">=", m_lineNum, m_columnNum-2);
    // EQ/ASSIGN
    case '=':
      c = getChar ();
      if (c != '=')
      {
        ungetChar (c);
        return Token (ASSIGN, "=", m_lineNum, m_columnNum-1);
      }
      return Token (EQ, "==", m_lineNum, m_columnNum-2);
    // NEQ
    case '!':
      c = getChar ();
      if (c != '=')
      {
        ungetChar (c);
        return Token (ERROR, string(1, c), m_lineNum, m_columnNum-1);
      }
      return Token (NEQ, "!=", m_lineNum, m_columnNum-2);

    // Punctuators
    case ';': return Token (SEMI, ";", m_lineNum, m_columnNum-1);
    case ',': return Token (COMMA, ",", m_lineNum, m_columnNum-1);
    case '(': return Token (LPAREN, "(", m_lineNum, m_columnNum-1);  
    case ')': return Token (RPAREN, ")", m_lineNum, m_columnNum-1);
    case '[': return Token (LBRACK, "[", m_lineNum, m_columnNum-1);  
    case ']': return Token (RBRACK, "]", m_lineNum, m_columnNum-1);   
    case '{': return Token (LBRACE, "{", m_lineNum, m_columnNum-1);   
    case '}': return Token (RBRACE, "}", m_lineNum, m_columnNum-1);  
            
    default:
      return Token (ERROR, string(1, c), m_lineNum, m_columnNum-1);
    }
  }
}

//========================================================================

/*
  Retrieves a character from the file stream
*/
int 
Lexer::getChar ()
{
  ++m_columnNum;
  return getc(m_srcFile);
}

//========================================================================

/*
  Places a character back into the file stream
*/
void 
Lexer::ungetChar (int c)
{
  --m_columnNum; 
  ungetc(c, m_srcFile);
}

//========================================================================

/*
  Grabs and returns a keyword or identifier token from the file at 
  the current position 
  - File stream must be currently at an alpha
*/
Token
Lexer::getId ()
{
  // read in chars until non id char is found
  char c = getChar();
  std::stringstream lexeme; 

  // first char must be alpha
  if (isalpha(c)) 
  {
    lexeme << c; 
  
    // add rest of characters
    // can be alpha or num 
    c = getChar();
    while (isalnum(c))
    {
      lexeme << c;
      c = getChar();
    }

    // Put non-identifier char back
    ungetChar(c);

    // determine if token is a keyword
    if (lexeme.str() == "if")          
      return Token (
        IF, 
        lexeme.str(), 
        m_lineNum, 
        m_columnNum-lexeme.str().size()
      );
    else if (lexeme.str() == "else")   
      return Token (
        ELSE, 
        lexeme.str(), 
        m_lineNum, 
        m_columnNum-lexeme.str().size()
      );
    else if (lexeme.str() == "int")    
      return Token (
        INT, 
        lexeme.str(), 
        m_lineNum, 
        m_columnNum-lexeme.str().size()
      );
    else if (lexeme.str() == "void")   
      return Token (
        VOID, 
        lexeme.str(), 
        m_lineNum, 
        m_columnNum-lexeme.str().size()
      );
    else if (lexeme.str() == "return") 
      return Token (
        RETURN, 
        lexeme.str(), 
        m_lineNum, 
        m_columnNum-lexeme.str().size()
      );
    else if (lexeme.str() == "while")  
      return Token (
        WHILE, 
        lexeme.str(), 
        m_lineNum, 
        m_columnNum-lexeme.str().size()
      );

    // token is not keyword 
    // therefore it is an identifier
    return Token (
      ID, 
      lexeme.str(), 
      m_lineNum,
      m_columnNum-lexeme.str().size()
    );
  
  }

  // doesn't start with alpha
  // Is not an identifier 
  ungetChar(c);
  return Token (ERROR, string(1, c), m_lineNum, m_columnNum-1);
}

//========================================================================

/*
  Grabs and returns an integer token from the file at the current 
  position 
  - File stream must be currently at a digit
*/
Token
Lexer::getNum ()
{
  // read in chars until non-digit char is found
  char c = getChar();
  std::stringstream lexeme; 

  while (isdigit(c)) 
  {
    lexeme << c; 
    c = getChar();
  }

  // put the non digit char back
  ungetChar(c);

  return Token (
    NUM, 
    lexeme.str(), 
    m_lineNum, 
    m_columnNum-lexeme.str().size(), 
    stoi(lexeme.str())
  );
}

//========================================================================

int
Lexer::getLineNum()
{
  return m_lineNum;
}

//========================================================================

int 
Lexer::getColumnNum()
{
  return m_columnNum;
}

//========================================================================