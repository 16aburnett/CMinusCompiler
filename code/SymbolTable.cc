// Filename:   SymbolTable.cc
// Name:       Amy Burnett and Lauren Deaver 
// Course:     CSCI 435 - Compilers
// Assignment: A10CMinusSymbolTree
// Date:       TBD
/********************************************************************/
// System Includes

#include <vector>
#include <string>
#include <unordered_map>
// For unique_ptr
#include <memory>

/********************************************************************/
// Local Includes

#include "SymbolTable.h"
#include "CMinusAst.h"

/********************************************************************/
// Using Declarations

using ScopeTable = std::unordered_map<std::string, DeclarationNode*>;

/********************************************************************/

SymbolTable::SymbolTable ()
{

}

//========================================================================

SymbolTable::~SymbolTable ()
{

}

//========================================================================

// Adjust the nest level; add a new scope table
void
SymbolTable::enterScope ()
{

}

//========================================================================

// Adjust the nest level; remove most recent scope table
void
SymbolTable::exitScope ()
{

}

//========================================================================

// Add a (name, declarationPtr) entry to table
// If successful set nest level in *declarationPtr
// Return true if successful, false o/w
bool
SymbolTable::insert (DeclarationNode* declarationPtr)
{

}

//========================================================================

// Lookup a name corresponding to a Use node
// Return corresponding declaration pointer on success,
//   nullptr o/w
DeclarationNode*
SymbolTable::lookup (const std::string& name)
{

}

//========================================================================

int
SymbolTable::getNestLevel () const
{

}

//========================================================================