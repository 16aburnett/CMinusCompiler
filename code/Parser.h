// Name:       Amy Burnett and Lauren Deaver 
// Course:     CSCI 435 - Compilers
// Assignment: A10CMinusSymbolTree
// Date:       TBD
//========================================================================

#ifndef PARSER_H
#define PARSER_H

//========================================================================
// Includes

#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <string>
#include <vector>
#include <initializer_list>

#include "Lexer.h"
#include "CMinusAst.h"

//========================================================================

class Parser
{
public:
    Parser(std::vector<Token> tokens, bool doDebug);

    Node*
    parse();

private:

    void
    match (std::string function, 
        std::initializer_list<TokenType> expectedTokenTypes, 
        std::string additional
    );

    void
    error (std::string function, 
        std::initializer_list<TokenType> expectedTokenTypes,
        std::string additional);

    void 
    enter (std::string name);

    void 
    leave (std::string name);

    void
    printSpaces (int level);

    // Grammar Productions 

    ProgramNode* 
    program ();

    std::vector<DeclarationNode*> 
    declarationList ();

    DeclarationNode*  
    declaration ();

    VariableDeclarationNode*  
    varDeclaration (CMinusAST::Type type, std::string var);

    CMinusAST::Type  
    typeSpecifier ();

    FunctionDeclarationNode* 
    funDeclaration (CMinusAST::Type type, std::string var);

    std::vector<ParameterNode*>   
    params ();

    std::vector<ParameterNode*>   
    paramList ();

    ParameterNode*  
    param ();

    CompoundStatementNode*  
    compoundStatement ();

    std::vector<VariableDeclarationNode*> 
    localDeclarations ();

    std::vector<StatementNode*> 
    statementList ();

    StatementNode* 
    statement ();

    ExpressionStatementNode*  
    expressionStatement ();

    IfStatementNode* 
    selectionStatement ();

    WhileStatementNode*  
    iterationStatement ();

    ReturnStatementNode*   
    returnStatement ();

    ExpressionNode*   
    expression ();

    VariableExpressionNode*  
    var ();

    ExpressionNode* 
    simpleExpression ();

    CMinusAST::RelationalOperator 
    relop ();

    ExpressionNode* 
    additiveExpression ();

    CMinusAST::AddOperator  
    addop ();

    ExpressionNode*  
    term ();

    CMinusAST::MultiplyOperator 
    mulop ();

    ExpressionNode* 
    factor ();

    CallExpressionNode* 
    call ();

    std::vector<ExpressionNode*> 
    args ();

    std::vector<ExpressionNode*> 
    argList ();

    // end productions 

    std::vector<Token> m_tokens;
    size_t m_currentToken; 
    // For indenting trace output
    int m_level;
    // for outputting debug info
    bool m_doDebug; 
};


//========================================================================

#endif