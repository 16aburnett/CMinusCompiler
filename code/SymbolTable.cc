// Filename:   SymbolTable.cc
// Name:       Amy Burnett and Lauren Deaver 
// Course:     CSCI 435 - Compilers
// Assignment: A10CMinusSymbolTree
// Date:       TBD
//========================================================================
// System Includes

#include <vector>
#include <string>
#include <unordered_map>
// For unique_ptr
#include <memory>

//========================================================================
// Local Includes

#include "SymbolTable.h"
#include "CMinusAst.h"

//========================================================================
// Using Declarations

using ScopeTable = std::unordered_map<std::string, DeclarationNode*>;

//========================================================================

SymbolTable::SymbolTable ()
: m_nestLevel (0)
{
    // make initial scope table
    std::unique_ptr<ScopeTable> newScope (new ScopeTable ());
    // add new scope table to the symbol table
    m_table.push_back (std::move (newScope));
}

//========================================================================

SymbolTable::~SymbolTable ()
{
    while (!m_table.empty ())
        m_table.pop_back ();
}

//========================================================================

// Adjust the nest level; add a new scope table
void
SymbolTable::enterScope ()
{
    // make a new scope table
    std::unique_ptr<ScopeTable> newScope (new ScopeTable ());
    // add new scope table to the symbol table
    m_table.push_back (std::move (newScope));
    // increase scope level 
    ++m_nestLevel;
}

//========================================================================

// Adjust the nest level; remove most recent scope table
void
SymbolTable::exitScope ()
{
    // pop from symbol table
    m_table.pop_back ();
    // decrement scope level
    --m_nestLevel;
}

//========================================================================

// Add a (name, declarationPtr) entry to table at the current scope level
// If successful set nest level in *declarationPtr
// Return true if successful, false o/w
bool
SymbolTable::insert (DeclarationNode* declarationPtr)
{
    // Get the varname for the declaration
    std::string name = declarationPtr->m_id;
    // Ensure variable wasn't already delcared at current scope
    if (m_table.back ().get ()->find (name) != m_table.back ().get ()->end ())
        return false;
    // Add declaration to symbol table
    m_table[m_nestLevel].get ()->insert ({name, declarationPtr});
    // set nest level for declarationPtr
    declarationPtr->m_nestLevel = m_nestLevel;
    return true;
}

//========================================================================

// Lookup a name corresponding to a Use node
// Return corresponding declaration pointer on success,
//   nullptr o/w
DeclarationNode*
SymbolTable::lookup (const std::string& name)
{
    // Look through each scope 
    // innermost -> outermost 
    for (auto iter = m_table.rbegin (); iter != m_table.rend (); ++iter)
    {
        // check scope for variable
        auto entry = iter->get()->find (name);
        // if variable exists
        if (entry != iter->get()->end ())
        {
            return entry->second; 
        }
    }
    // reaches here if no matching variable declaration was found
    return nullptr; 
}

//========================================================================

int
SymbolTable::getNestLevel () const
{
    return m_nestLevel;
}

//========================================================================