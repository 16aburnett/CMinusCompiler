// Filename:   PrintVisitor.h
// Name:       Amy Burnett and Lauren Deaver 
// Course:     CSCI 435 - Compilers
// Assignment: A10CMinusSymbolTree
// Date:       TBD
//========================================================================

#ifndef PRINT_VISITOR_H
#define PRINT_VISITOR_H

//========================================================================
// Includes

#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>

#include "CMinusAst.h"

//========================================================================

struct PrintVisitor : IVisitor
{
    // class members 
    std::stringstream m_outputString; 
    int m_level = 0; 

    virtual void visit (ProgramNode* node);
    virtual void visit (DeclarationNode* node);
    virtual void visit (VariableDeclarationNode* node);
    virtual void visit (ArrayDeclarationNode* node);
    virtual void visit (FunctionDeclarationNode* node);
    virtual void visit (StatementNode* node);
    virtual void visit (IfStatementNode* node);
    virtual void visit (WhileStatementNode* node);
    virtual void visit (ForStatementNode* node);
    virtual void visit (ExpressionStatementNode* node);
    virtual void visit (ReturnStatementNode* node);
    virtual void visit (CompoundStatementNode* node);
    virtual void visit (ParameterNode* node);
    virtual void visit (ExpressionNode* node);
    virtual void visit (AdditiveExpressionNode* node);
    virtual void visit (MultiplicativeExpressionNode* node);
    virtual void visit (RelationalExpressionNode* node);
    virtual void visit (AssignmentExpressionNode* node);
    virtual void visit (VariableExpressionNode* node);
    virtual void visit (SubscriptExpressionNode* node);
    virtual void visit (CallExpressionNode* node);
    virtual void visit (IntegerLiteralExpressionNode* node);
    virtual void visit (UnaryExpressionNode* node);
    
    void
    printSpaces (int level);
    
};

//========================================================================

#endif