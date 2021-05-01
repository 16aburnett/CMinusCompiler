// Filename:   SymbolTableVisitor.h
// Name:       Amy Burnett and Lauren Deaver 
// Course:     CSCI 435 - Compilers
// Assignment: A10CMinusSymbolTree
// Date:       TBD
/********************************************************************/

#ifndef SYMBOL_TABLE_VISITOR_H
#define SYMBOL_TABLE_VISITOR_H

/********************************************************************/
// System Includes

#include <vector>
#include <string>
#include <unordered_map>
// For unique_ptr
#include <memory>

/********************************************************************/
// Local Includes

#include "CMinusAst.h"
#include "SymbolTable.h"

/********************************************************************/
// Using Declarations


/********************************************************************/

struct SymbolTableVisitor : IVisitor 
{
    // for storing and looking up variable type information
    SymbolTable m_symbolTable;
    
    // stores parameters for adding to function scopes 
    std::vector<ParameterNode*> m_parameters; 

    bool m_wasSuccessful = true;

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

};

#endif