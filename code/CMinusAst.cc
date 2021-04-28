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

DeclarationNode::DeclarationNode (CMinusAST::Type _type, std::string _id)
: m_type(_type), m_id(_id)
{}

//--------------------------------------------------------------------

DeclarationNode::~DeclarationNode () {}

//========================================================================
// VariableDeclarationNode

VariableDeclarationNode::VariableDeclarationNode (
    CMinusAST::Type _type, 
    std::string _id
): DeclarationNode(_type, _id)
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
    size_t _size
): VariableDeclarationNode(_type, _id),
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
    std::vector<ParameterNode*> params, 
    CompoundStatementNode* body
)
: DeclarationNode(_type, _id),
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
    bool isArray
): m_type(type), m_id(id), m_isArray(isArray)
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

VariableExpressionNode::VariableExpressionNode (std::string id) 
: m_id(id)
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
    ExpressionNode* subscript
) : VariableExpressionNode(id),
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
    std::vector<ExpressionNode*> args
): m_id(id), m_args(args)
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
// PrintVisitor

// should return a string!!!
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

//------------------------------------------------------------------------

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

//------------------------------------------------------------------------

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

//------------------------------------------------------------------------

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

//------------------------------------------------------------------------

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
    for (ParameterNode* p : node->m_params)
        p->accept (this);

    // print the compound statement 
    node->m_body->accept (this);
    
    --m_level;
}

//------------------------------------------------------------------------

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

//------------------------------------------------------------------------

void 
PrintVisitor::visit (StatementNode* node) 
{
    // nothing to output or visit
}

//------------------------------------------------------------------------

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

//------------------------------------------------------------------------

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

//------------------------------------------------------------------------


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


//------------------------------------------------------------------------

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

//------------------------------------------------------------------------

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

//------------------------------------------------------------------------

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

//------------------------------------------------------------------------

void 
PrintVisitor::visit (ExpressionNode* node) 
{
    // nothing to output or visit
}

//------------------------------------------------------------------------

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

//------------------------------------------------------------------------

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

//------------------------------------------------------------------------

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

//------------------------------------------------------------------------

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

//------------------------------------------------------------------------

void 
PrintVisitor::visit (VariableExpressionNode* node) 
{
    printSpaces (m_level);
    m_outputString << "Variable: ";
    m_outputString << node->m_id;
    m_outputString << "\n";
}

//------------------------------------------------------------------------

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

//------------------------------------------------------------------------

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

//------------------------------------------------------------------------

void 
PrintVisitor::visit (IntegerLiteralExpressionNode* node) 
{
    printSpaces (m_level);
    m_outputString << "Integer: ";
    m_outputString << node->m_value;
    m_outputString << "\n";
}

//------------------------------------------------------------------------

void 
PrintVisitor::visit (UnaryExpressionNode* node) 
{

}

//------------------------------------------------------------------------

void
PrintVisitor::printSpaces (int level)
{
    while (level-- > 0)
        m_outputString << "  ";
}

//========================================================================