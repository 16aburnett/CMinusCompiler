// Filename:   SymbolTableVisitor.cc
// Name:       Amy Burnett and Lauren Deaver 
// Course:     CSCI 435 - Compilers
// Assignment: A10CMinusSymbolTree
// Date:       TBD
//========================================================================
// System includes

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
// For unique_ptr
#include <memory>

//========================================================================
// Local includes

#include "SymbolTableVisitor.h"
#include "CMinusAst.h"

//========================================================================
// SymbolTableVisitor

void 
SymbolTableVisitor::visit (ProgramNode* node)
{
    // No declarations so nothing to do 
    // just visit the next node(s)
    // this is the same as in the PrintVisitor
    for (DeclarationNode* decl : node->m_declarations)
    {
        // visit 
        decl->accept (this);
    }

}

//========================================================================

void 
SymbolTableVisitor::visit (DeclarationNode* node)
{
    bool wasSuccessful = m_symbolTable.insert (node);

    if (!wasSuccessful)
    {
        std::string varname = node->m_id;
        DeclarationNode* originalDeclaration = m_symbolTable.lookup (varname);
        printf ("Semantic Error: Redeclaration of '%s'\n", varname.c_str ());
        printf ("  Originally on line %d: column %d\n", 
            originalDeclaration->m_lineno, 
            originalDeclaration->m_columnno
        );
        printf ("  Redeclaration on line %d: column %d\n", 
            node->m_lineno,
            node->m_columnno
        );
        printf ("\n");
        m_wasSuccessful = false;
    }

}

//========================================================================

void 
SymbolTableVisitor::visit (VariableDeclarationNode* node)
{
    bool wasSuccessful = m_symbolTable.insert (node);

    if (!wasSuccessful)
    {
        std::string varname = node->m_id;
        DeclarationNode* originalVariableDeclaration = m_symbolTable.lookup (varname);
        printf ("Semantic Error: Redeclaration of '%s'\n", varname.c_str ());
        printf ("  Originally on line %d: column %d\n", 
            originalVariableDeclaration->m_lineno, 
            originalVariableDeclaration->m_columnno
        );
        printf ("  Redeclaration on line %d: column %d\n", 
            node->m_lineno,
            node->m_columnno
        );
        printf ("\n");
        m_wasSuccessful = false;
    }

}

//========================================================================

void 
SymbolTableVisitor::visit (ArrayDeclarationNode* node)
{
    bool wasSuccessful = m_symbolTable.insert (node);

    if (!wasSuccessful)
    {
        std::string varname = node->m_id;
        DeclarationNode* originalArrayDeclaration = m_symbolTable.lookup (varname);
        printf ("Semantic Error: Redeclaration of '%s'\n", varname.c_str ());
        printf ("  Originally on line %d: column %d\n", 
            originalArrayDeclaration->m_lineno, 
            originalArrayDeclaration->m_columnno
        );
        printf ("  Redeclaration on line %d: column %d\n", 
            node->m_lineno,
            node->m_columnno
        );
        printf ("\n");
        m_wasSuccessful = false;
    }

}

//========================================================================

void 
SymbolTableVisitor::visit (FunctionDeclarationNode* node) 
{
    // add declaration to the current scope 
    bool wasSuccessful = m_symbolTable.insert (node);
    // adding to the symbol table is unsuccessful when 
    // the declaration is a duplicate
    if (!wasSuccessful)
    {
        // print error message for the redeclaration
        std::string varname = node->m_id;
        DeclarationNode* originalDeclaration = m_symbolTable.lookup (varname);
        printf ("Semantic Error: Redeclaration of '%s'\n", varname.c_str ());
        printf ("  Originally on line %d: column %d\n", 
            originalDeclaration->m_lineno, 
            originalDeclaration->m_columnno
        );
        printf ("  Redeclaration on line %d: column %d\n", 
            node->m_lineno,
            node->m_columnno
        );
        printf ("\n");
        m_wasSuccessful = false;
    }

    // Grab parameters so that the body can use them
    for (ParameterNode* p : node->m_params)
        m_parameters.push_back (p);

    // Check the body of the function
    // the body should be a compound statement 
    // which will handle the parameters in
    // its own scope 
    node->m_body->accept (this);

}

//========================================================================

void 
SymbolTableVisitor::visit (ParameterNode* node) 
{
    // add declaration to the current scope 
    bool wasSuccessful = m_symbolTable.insert (node);
    // adding to the symbol table is unsuccessful when 
    // the declaration is a duplicate
    if (!wasSuccessful)
    {
        // print error message for the redeclaration
        std::string varname = node->m_id;
        DeclarationNode* originalDeclaration = m_symbolTable.lookup (varname);
        printf ("Semantic Error: Redeclaration of Parameter '%s'\n", varname.c_str ());
        printf ("  Originally on line %d: column %d\n", 
            originalDeclaration->m_lineno, 
            originalDeclaration->m_columnno
        );
        printf ("  Redeclaration on line %d: column %d\n", 
            node->m_lineno,
            node->m_columnno
        );
        printf ("\n");
        m_wasSuccessful = false;
    }
}

//========================================================================

void 
SymbolTableVisitor::visit (StatementNode* node) 
{
    // nothing to visit
}

//========================================================================

void 
SymbolTableVisitor::visit (IfStatementNode* node) 
{
    // No variables at this level
    // just check children nodes 

    node->m_condition->accept (this);
    node->m_then->accept (this);
    // optionally
    if (node->m_else != nullptr)
        node->m_else->accept (this);
}

//========================================================================

void 
SymbolTableVisitor::visit (WhileStatementNode* node) 
{
    node->m_condition->accept (this);
    node->m_statement->accept (this);
}

//========================================================================

void 
SymbolTableVisitor::visit (ForStatementNode* node) 
{
    node->m_init->accept (this);
    node->m_condition->accept (this);
    node->m_update->accept (this);
    node->m_statement->accept (this);
}

//========================================================================

void 
SymbolTableVisitor::visit (ExpressionStatementNode* node) 
{
    node->m_expression->accept (this);
}

//========================================================================

void 
SymbolTableVisitor::visit (ReturnStatementNode* node) 
{
    if (node->m_expression != nullptr)
        node->m_expression->accept (this);   
}

//========================================================================

void 
SymbolTableVisitor::visit (CompoundStatementNode* node) 
{
    // compound statements have their own scope 
    m_symbolTable.enterScope ();

    // if this is a function body,
    // then add the parameters to this scope
    for (ParameterNode* p : m_parameters)
        p->accept (this);
    // get rid of params since we added them
    m_parameters.clear ();

    // visit the declaration statements 
    // any declaration will be added to this scope 
    for (VariableDeclarationNode* v : node->m_vars)
        v->accept (this);

    // check that the statements reference declared variables 
    for (StatementNode* s : node->m_statements)
        s->accept (this);

    // close scope 
    m_symbolTable.exitScope ();
}

//========================================================================

void 
SymbolTableVisitor::visit (ExpressionNode* node) 
{
    // nothing to visit
}

//========================================================================

void 
SymbolTableVisitor::visit (AdditiveExpressionNode* node) 
{
    node->m_lhs->accept (this);
    node->m_rhs->accept (this);
}

//========================================================================

void 
SymbolTableVisitor::visit (MultiplicativeExpressionNode* node) 
{
    node->m_lhs->accept (this);
    node->m_rhs->accept (this);
}

//========================================================================

void 
SymbolTableVisitor::visit (RelationalExpressionNode* node) 
{
    node->m_lhs->accept (this);
    node->m_rhs->accept (this);
}

//========================================================================

void 
SymbolTableVisitor::visit (AssignmentExpressionNode* node) 
{
    node->m_lhs->accept (this);
    node->m_rhs->accept (this);
}

//========================================================================

void 
SymbolTableVisitor::visit (VariableExpressionNode* node) 
{
    // Ensure that variable was declared previously
    DeclarationNode* declaration = m_symbolTable.lookup (node->m_id);
    if (declaration == nullptr)
    {
        // Print error message for undeclared variable 
        printf ("Semantic Error: '%s' was not declared in this scope\n", 
            node->m_id.c_str ()
        );
        printf ("  Located on line %d: column %d\n",
            node->m_lineno,
            node->m_columnno
        );
        printf ("\n");
        m_wasSuccessful = false;
    }
    // variable has a declaration
    else
    {
        // Save type information with variable 
        node->m_type = declaration->m_type;
    }
}

//========================================================================

void 
SymbolTableVisitor::visit (SubscriptExpressionNode* node) 
{
    DeclarationNode* declaration = m_symbolTable.lookup (node->m_id);

    if(declaration == nullptr)
    {
        printf ("Semantic Error: '%s' was not declared in this scope\n", 
            node->m_id.c_str ()
        );
        printf ("  Located on line %d: column %d\n",
            node->m_lineno,
            node->m_columnno
        );
        printf ("\n");
        m_wasSuccessful = false;
    }
    else
    {
        node->m_type = declaration->m_type;
    }

    // visit subscript
    node->m_subscript->accept (this);
}

//========================================================================

void 
SymbolTableVisitor::visit (CallExpressionNode* node) 
{
    DeclarationNode* declaration = m_symbolTable.lookup (node->m_id);
    
    if(declaration == nullptr)
    {
        printf ("Semantic Error: '%s' was not declared in this scope\n", 
            node->m_id.c_str ()
        );
        printf ("  Located on line %d: column %d\n",
            node->m_lineno,
            node->m_columnno
        );
        printf ("\n");
        m_wasSuccessful = false;
    }
    else
    {
        node->m_type = declaration->m_type;
    }

    // check the arguments 
    for (ExpressionNode* a : node->m_args)
        a->accept (this);
}

//========================================================================

void 
SymbolTableVisitor::visit (IntegerLiteralExpressionNode* node) 
{
    // nothing to analyze 
}

//========================================================================

void 
SymbolTableVisitor::visit (UnaryExpressionNode* node) 
{
    // nothing to analyze
    // not implemented in language
}

//========================================================================

