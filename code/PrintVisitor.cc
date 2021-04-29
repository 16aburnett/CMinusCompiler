// Filename:   PrintVisitor.cc
// Name:       Amy Burnett and Lauren Deaver
// Course:     CSCI 435 - Compilers
// Assignment: A10CMinusSymbolTree
// Date:       TBD
//========================================================================
// Includes

#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <string>
#include <vector>

#include "Lexer.h"
#include "CMinusAst.h"
#include "PrintVisitor.h"

//========================================================================

void 
PrintVisitor::visit (ProgramNode* node)
{
    m_outputString << "ProgramNode:\n";
    // visit declarations of the program 
    // indent depth for clean print of children
    ++m_level; 
    for (DeclarationNode* decl : node->m_declarations)
    {
        m_outputString << "\n";
        // visit 
        decl->accept (this);
    }
    --m_level;
}

//========================================================================

void 
PrintVisitor::visit (DeclarationNode* node)
{
    printSpaces (m_level);
    m_outputString << "Declaration: ";
    m_outputString << node->m_id;
    m_outputString << ": ";
    m_outputString << CMinusAST::typeToString(node->m_type);
    m_outputString << " type\n";
}

//========================================================================

void 
PrintVisitor::visit (VariableDeclarationNode* node)
{
    printSpaces (m_level);
    m_outputString << "VariableDeclaration: ";
    m_outputString << node->m_id;
    m_outputString << ": ";
    m_outputString << CMinusAST::typeToString(node->m_type);
    m_outputString << " type\n";
}

//========================================================================

void 
PrintVisitor::visit (ArrayDeclarationNode* node)
{
    printSpaces (m_level);
    // the expected output says variabledeclaration even though its an array
    m_outputString << "VariableDeclaration: ";
    m_outputString << node->m_id;
    m_outputString << "[";
    m_outputString << node->m_size;
    m_outputString << "]: ";
    m_outputString << CMinusAST::typeToString(node->m_type);
    m_outputString << " type\n";
}

//========================================================================

void 
PrintVisitor::visit (FunctionDeclarationNode* node) 
{
    printSpaces (m_level);
    m_outputString << "Function: ";
    m_outputString << node->m_id;
    m_outputString << ": ";
    m_outputString << CMinusAST::typeToString(node->m_type);
    m_outputString << " type\n";
    ++m_level; 

    // print out each of the parameters
    for (DeclarationNode* p : node->m_params)
        p->accept (this);

    // print the compound statement 
    node->m_body->accept (this);
    
    --m_level;
}

//========================================================================

void 
PrintVisitor::visit (ParameterNode* node) 
{
    printSpaces (m_level);
    // Parameter: <id>: <Type> type
    // Parameter: <id>[]: <Type> type
    m_outputString << "Parameter: ";
    m_outputString << node->m_id;

    if (node->m_isArray)
        m_outputString << "[]";
    
    m_outputString << ": ";
    m_outputString << CMinusAST::typeToString(node->m_type);
    m_outputString << " type\n";
}

//========================================================================

void 
PrintVisitor::visit (StatementNode* node) 
{
    // nothing to output or visit
}

//========================================================================

void 
PrintVisitor::visit (IfStatementNode* node) 
{
    printSpaces (m_level);

    m_outputString << "If\n";

    ++m_level;
    node->m_condition->accept (this);
    node->m_then->accept (this);
    // optionally
    if (node->m_else != nullptr)
        node->m_else->accept (this);
    --m_level;
}

//========================================================================

void 
PrintVisitor::visit (WhileStatementNode* node) 
{
    printSpaces (m_level);

    m_outputString << "While\n";

    ++m_level;
    node->m_condition->accept (this);
    node->m_statement->accept (this);
    --m_level;
}

//========================================================================


void 
PrintVisitor::visit (ForStatementNode* node) 
{
    printSpaces (m_level);

    m_outputString << "For\n";

    ++m_level;
    node->m_init->accept (this);
    node->m_condition->accept (this);
    node->m_update->accept (this);
    node->m_statement->accept (this);
    --m_level;   
}


//========================================================================

void 
PrintVisitor::visit (ExpressionStatementNode* node) 
{
    printSpaces (m_level);

    m_outputString << "ExpressionStatement: \n";

    ++m_level;
    if (node->m_expression != nullptr)
        node->m_expression->accept (this);
    --m_level;
}

//========================================================================

void 
PrintVisitor::visit (ReturnStatementNode* node) 
{
    printSpaces (m_level);

    m_outputString << "Return\n";

    ++m_level;
    if (node->m_expression != nullptr)
        node->m_expression->accept (this);
    --m_level;
}

//========================================================================

void 
PrintVisitor::visit (CompoundStatementNode* node) 
{
    printSpaces (m_level);

    m_outputString << "CompoundStatement:\n";

    ++m_level;
    for (VariableDeclarationNode* v : node->m_vars)
        v->accept (this);
    for (StatementNode* s : node->m_statements)
        s->accept (this);
    --m_level;
    
}

//========================================================================

void 
PrintVisitor::visit (ExpressionNode* node) 
{
    // nothing to output or visit
}

//========================================================================

void 
PrintVisitor::visit (AdditiveExpressionNode* node) 
{
    printSpaces (m_level);

    m_outputString << "AdditiveExpression: ";
    m_outputString << CMinusAST::AddOpToString(node->m_addOp);
    m_outputString << "\n";

    ++m_level;

    // output left node
    printSpaces (m_level);
    m_outputString << "Left:\n";
    ++m_level;
    node->m_lhs->accept (this);
    --m_level;

    // output right node
    printSpaces (m_level);
    m_outputString << "Right:\n";
    ++m_level;
    node->m_rhs->accept (this);
    --m_level;

    --m_level;
}

//========================================================================

void 
PrintVisitor::visit (MultiplicativeExpressionNode* node) 
{
    printSpaces (m_level);

    m_outputString << "MultiplicativeExpression: ";
    m_outputString << CMinusAST::MultiplyOpToString(node->m_multOp);
    m_outputString << "\n";

    ++m_level;

    // output left node
    printSpaces (m_level);
    m_outputString << "Left:\n";
    ++m_level;
    node->m_lhs->accept (this);
    --m_level;

    // output right node
    printSpaces (m_level);
    m_outputString << "Right:\n";
    ++m_level;
    node->m_rhs->accept (this);
    --m_level;

    --m_level;
}

//========================================================================

void 
PrintVisitor::visit (RelationalExpressionNode* node) 
{
    printSpaces (m_level);
    m_outputString << "RelationalExpression: ";
    m_outputString << CMinusAST::RelationalOpToString(node->m_relOp);
    m_outputString << "\n";

    ++m_level;

    // output left node
    printSpaces (m_level);
    m_outputString << "Left:\n";
    ++m_level;
    node->m_lhs->accept (this);
    --m_level;

    // output right node
    printSpaces (m_level);
    m_outputString << "Right:\n";
    ++m_level;
    node->m_rhs->accept (this);
    --m_level;

    --m_level;
}

//========================================================================

void 
PrintVisitor::visit (AssignmentExpressionNode* node) 
{
    printSpaces (m_level);
    m_outputString << "Assignment:\n";

    ++m_level;
    node->m_lhs->accept (this);
    node->m_rhs->accept (this);
    --m_level;
}

//========================================================================

void 
PrintVisitor::visit (VariableExpressionNode* node) 
{
    printSpaces (m_level);
    m_outputString << "Variable: ";
    m_outputString << node->m_id;
    m_outputString << "\n";
}

//========================================================================

void 
PrintVisitor::visit (SubscriptExpressionNode* node) 
{
    printSpaces (m_level);
    m_outputString << "Subscript: ";
    m_outputString << node->m_id;
    m_outputString << "\n";

    ++m_level;
    printSpaces (m_level);
    m_outputString << "Index:\n";

    ++m_level;
    node->m_subscript->accept (this);
    --m_level;

    --m_level;

}

//========================================================================

void 
PrintVisitor::visit (CallExpressionNode* node) 
{
    printSpaces (m_level);
    m_outputString << "FunctionCall: ";
    m_outputString << node->m_id;
    m_outputString << "\n";

    ++m_level;

    printSpaces (m_level);
    m_outputString << "Arguments:\n";
    ++m_level;
    for (ExpressionNode* a : node->m_args)
    {
        a->accept (this);
    }
    --m_level;

    --m_level;
    
}

//========================================================================

void 
PrintVisitor::visit (IntegerLiteralExpressionNode* node) 
{
    printSpaces (m_level);
    m_outputString << "Integer: ";
    m_outputString << node->m_value;
    m_outputString << "\n";
}

//========================================================================

void 
PrintVisitor::visit (UnaryExpressionNode* node) 
{

}

//========================================================================

void
PrintVisitor::printSpaces (int level)
{
    while (level-- > 0)
        m_outputString << "  ";
}

//========================================================================