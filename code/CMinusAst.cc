// Filename:   CMinusAst.cc
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

//========================================================================

std::string
CMinusAST::typeToString (CMinusAST::Type t)
{
    if (t == INT)
        return "Int";
    else
        return "void";
}

//========================================================================

std::string
CMinusAST::AddOpToString (CMinusAST::AddOperator a)
{
    if (a == ADD)
        return "+";
    else if (a == MINUS)
        return "-";
    return "";
}

//========================================================================

std::string
CMinusAST::MultiplyOpToString (CMinusAST::MultiplyOperator a)
{
    if (a == TIMES)
        return "*";
    else if (a == DIVIDE)
        return "/";
    else if (a == MOD)
        return "%";
    return "";
}

//========================================================================

std::string
CMinusAST::RelationalOpToString (CMinusAST::RelationalOperator a)
{
    if (a == LTE)
        return "<=";
    else if (a == LT)
        return "<";
    else if (a == GT)
        return ">";
    else if (a == GTE)
        return ">=";
    else if (a == EQ)
        return "==";
    else if (a == NEQ)
        return "!=";
    return "";
}

//========================================================================
// ProgramNode

ProgramNode::ProgramNode (std::vector<DeclarationNode*> declarations) 
: m_declarations(declarations)
{}

//------------------------------------------------------------------------

ProgramNode::~ProgramNode ()
{
    for (DeclarationNode* n : m_declarations)
        delete n; 
}

//------------------------------------------------------------------------

void 
ProgramNode::accept (IVisitor* visitor)
{
    visitor->visit (this);
}

//========================================================================
// DeclarationNode 

DeclarationNode::DeclarationNode (
    CMinusAST::Type _type, 
    std::string _id, 
    int lineno, 
    int columnno
)
: m_type(_type), m_id(_id), m_lineno(lineno), m_columnno(columnno)
{}

//--------------------------------------------------------------------

DeclarationNode::~DeclarationNode () {}

//========================================================================
// VariableDeclarationNode

VariableDeclarationNode::VariableDeclarationNode (
    CMinusAST::Type _type, 
    std::string _id,
    int lineno,
    int columnno
): DeclarationNode(_type, _id, lineno, columnno)
{}

//--------------------------------------------------------------------

VariableDeclarationNode::~VariableDeclarationNode () {}

//------------------------------------------------------------------------

void 
VariableDeclarationNode::accept (IVisitor* visitor)
{
    visitor->visit (this);
}

//========================================================================
// ArrayDeclarationNode

ArrayDeclarationNode::ArrayDeclarationNode (
    CMinusAST::Type _type, 
    std::string _id,
    int lineno,
    int columnno, 
    size_t _size
): VariableDeclarationNode(_type, _id, lineno, columnno),
    m_size(_size)
{}

//------------------------------------------------------------------------

void 
ArrayDeclarationNode::accept (IVisitor* visitor)
{
    visitor->visit (this);
}

//========================================================================
// FunctionDeclarationNode

FunctionDeclarationNode::FunctionDeclarationNode (
    CMinusAST::Type _type, 
    std::string _id, 
    int lineno,
    int columnno,
    std::vector<ParameterNode*> params, 
    CompoundStatementNode* body
)
: DeclarationNode(_type, _id, lineno, columnno),
    m_params(params),
    m_body(body)
{}

//--------------------------------------------------------------------

FunctionDeclarationNode::~FunctionDeclarationNode () 
{
    for (ParameterNode* p : m_params)
        delete p; 
    delete m_body; 
}

//------------------------------------------------------------------------

void 
FunctionDeclarationNode::accept (IVisitor* visitor)
{
    visitor->visit (this);
}

//========================================================================
// ParameterNode

ParameterNode::ParameterNode (
    CMinusAST::Type type, 
    std::string id, 
    int lineno,
    int columnno,
    bool isArray
): DeclarationNode(type, id, lineno, columnno), m_isArray(isArray)
{}

//------------------------------------------------------------------------

void 
ParameterNode::accept (IVisitor* visitor)
{
    visitor->visit (this);
}

//========================================================================
// StatementNode

void 
StatementNode::accept (IVisitor* visitor)
{
    visitor->visit (this);
}

//========================================================================
// IfStatementNode

IfStatementNode::IfStatementNode (
    ExpressionNode* condition, 
    StatementNode* then, 
    StatementNode* _else
) : m_condition(condition), m_then(then), m_else(_else)
{}

//--------------------------------------------------------------------

IfStatementNode::~IfStatementNode () 
{
    delete m_condition;
    delete m_then;
    // else component can be optional 
    if (m_else != nullptr)
        delete m_else;
}

//------------------------------------------------------------------------

void 
IfStatementNode::accept (IVisitor* visitor)
{
    visitor->visit (this);
}

//========================================================================
// WhileStatementNode

WhileStatementNode::WhileStatementNode (
    ExpressionNode* condition, 
    StatementNode* statement
): m_condition(condition), m_statement(statement)
{}

//--------------------------------------------------------------------

WhileStatementNode::~WhileStatementNode () 
{
    delete m_condition;
    delete m_statement;
}

//------------------------------------------------------------------------

void 
WhileStatementNode::accept (IVisitor* visitor)
{
    visitor->visit (this);
}

//========================================================================
// ForStatementNode

ForStatementNode::ForStatementNode (
    ExpressionNode* init, 
    ExpressionNode* condition, 
    ExpressionNode* update, 
    StatementNode* statement
) 
: m_init(init),
    m_condition(condition),
    m_update(update),
    m_statement(statement)
{}

//--------------------------------------------------------------------

ForStatementNode::~ForStatementNode () 
{
    delete m_init;
    delete m_condition;
    delete m_update;
    delete m_statement;
}

//------------------------------------------------------------------------

void 
ForStatementNode::accept (IVisitor* visitor)
{
    visitor->visit (this);
}

//========================================================================
// ExpressionStatementNode

ExpressionStatementNode::ExpressionStatementNode (ExpressionNode* expression)
: m_expression(expression)
{}

//--------------------------------------------------------------------

ExpressionStatementNode::~ExpressionStatementNode () 
{
    // component is optional
    if (m_expression != nullptr)
        delete m_expression;
}

//------------------------------------------------------------------------

void 
ExpressionStatementNode::accept (IVisitor* visitor)
{
    visitor->visit (this);
}

//========================================================================
// ReturnStatementNode

ReturnStatementNode::ReturnStatementNode (ExpressionNode* expression) 
: m_expression(expression)
{}

//--------------------------------------------------------------------

ReturnStatementNode::~ReturnStatementNode () 
{
    // expression is optional
    if (m_expression != nullptr)
        delete m_expression;
}

//------------------------------------------------------------------------

void 
ReturnStatementNode::accept (IVisitor* visitor)
{
    visitor->visit (this);
}

//========================================================================
// CompoundStatementNode

CompoundStatementNode::CompoundStatementNode (
    std::vector<VariableDeclarationNode*> vars, 
    std::vector<StatementNode*> statements
) 
: m_vars(vars),
    m_statements(statements)
{}

//--------------------------------------------------------------------

CompoundStatementNode::~CompoundStatementNode () 
{
    for (VariableDeclarationNode* d : m_vars)
        delete d;
    for (StatementNode* s : m_statements)
        delete s; 
}

//------------------------------------------------------------------------

void 
CompoundStatementNode::accept (IVisitor* visitor)
{
    visitor->visit (this);
}

//========================================================================
//ExpressionNode

ExpressionNode::ExpressionNode () {}
    
//--------------------------------------------------------------------

ExpressionNode::~ExpressionNode () {}

//------------------------------------------------------------------------

void 
ExpressionNode::accept (IVisitor* visitor)
{
    visitor->visit (this);
}

//========================================================================
// AdditiveExpressionNode

AdditiveExpressionNode::AdditiveExpressionNode (
    ExpressionNode* lhs, 
    CMinusAST::AddOperator op, 
    ExpressionNode* rhs
) : m_lhs(lhs), m_addOp(op), m_rhs(rhs)
{}

//--------------------------------------------------------------------

AdditiveExpressionNode::~AdditiveExpressionNode () 
{
    delete m_lhs;
    delete m_rhs;
}

//------------------------------------------------------------------------

void 
AdditiveExpressionNode::accept (IVisitor* visitor)
{
    visitor->visit (this);
}

//========================================================================
// MultiplicativeExpressionNode

MultiplicativeExpressionNode::MultiplicativeExpressionNode (
    ExpressionNode* lhs, 
    CMinusAST::MultiplyOperator op, 
    ExpressionNode* rhs
): m_lhs(lhs), m_multOp(op), m_rhs(rhs)
{}

//--------------------------------------------------------------------

MultiplicativeExpressionNode::~MultiplicativeExpressionNode () 
{
    delete m_lhs;
    delete m_rhs;
}

//------------------------------------------------------------------------

void 
MultiplicativeExpressionNode::accept (IVisitor* visitor)
{
    visitor->visit (this);
}

//========================================================================
// RelationalExpressionNode

RelationalExpressionNode::RelationalExpressionNode (
    ExpressionNode* lhs, 
    CMinusAST::RelationalOperator op, 
    ExpressionNode* rhs
): m_lhs(lhs), m_relOp(op), m_rhs(rhs)
{}

//--------------------------------------------------------------------

RelationalExpressionNode::~RelationalExpressionNode () 
{
    delete m_lhs;
    delete m_rhs;
}

//------------------------------------------------------------------------

void 
RelationalExpressionNode::accept (IVisitor* visitor)
{
    visitor->visit (this);
}

//========================================================================
// AssignmentExpressionNode

AssignmentExpressionNode::AssignmentExpressionNode (
    VariableExpressionNode* lhs, 
    ExpressionNode* rhs
): m_lhs(lhs), m_rhs(rhs)
{}

//--------------------------------------------------------------------

AssignmentExpressionNode::~AssignmentExpressionNode () 
{
    delete m_lhs;
    delete m_rhs;
}

//------------------------------------------------------------------------

void 
AssignmentExpressionNode::accept (IVisitor* visitor)
{
    visitor->visit (this);
}

//========================================================================
// VariableExpressionNode

VariableExpressionNode::VariableExpressionNode (
    std::string id,
    int lineno,
    int columnno
) 
: m_id(id), m_lineno(lineno), m_columnno(columnno)
{}

//--------------------------------------------------------------------

VariableExpressionNode::~VariableExpressionNode () {}

//------------------------------------------------------------------------

void 
VariableExpressionNode::accept (IVisitor* visitor)
{
    visitor->visit (this);
}

//========================================================================
// SubscriptExpressionNode

SubscriptExpressionNode::SubscriptExpressionNode (
    std::string id, 
    int lineno,
    int columnno,
    ExpressionNode* subscript
) : VariableExpressionNode(id, lineno, columnno),
    m_subscript(subscript)
{}

//--------------------------------------------------------------------

SubscriptExpressionNode::~SubscriptExpressionNode () 
{
    delete m_subscript;
}

//------------------------------------------------------------------------

void 
SubscriptExpressionNode::accept (IVisitor* visitor)
{
    visitor->visit (this);
}

//========================================================================
// CallExpressionNode

CallExpressionNode::CallExpressionNode (
    std::string id, 
    int lineno,
    int columnno,
    std::vector<ExpressionNode*> args
): m_id(id), m_lineno(lineno), m_columnno(columnno), m_args(args)
{}

//--------------------------------------------------------------------

CallExpressionNode::~CallExpressionNode () 
{
    for (ExpressionNode* a : m_args)
        delete a;
}

//------------------------------------------------------------------------

void 
CallExpressionNode::accept (IVisitor* visitor)
{
    visitor->visit (this);
}

//========================================================================
// IntegerLiteralExpressionNode

IntegerLiteralExpressionNode::IntegerLiteralExpressionNode (int value) 
: m_value(value)
{}

//--------------------------------------------------------------------

IntegerLiteralExpressionNode::~IntegerLiteralExpressionNode () {}

//------------------------------------------------------------------------

void 
IntegerLiteralExpressionNode::accept (IVisitor* visitor)
{
    visitor->visit (this);
}

//========================================================================