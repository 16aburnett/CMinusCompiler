// Filename:   CMinusAst.h
// Name:       Amy Burnett and Lauren Deaver 
// Course:     CSCI 435 - Compilers
// Assignment: A10CMinusSymbolTree
// Date:       TBD
//========================================================================

#ifndef CMinusAST_H
#define CMinusAST_H

//========================================================================
// Includes

#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>

//========================================================================
// Types 

namespace CMinusAST 
{
    enum Type {INT, VOID, ARRAY}; 

    std::string 
    typeToString (Type t);

    // Operators 
    enum AddOperator {ADD, MINUS};

    std::string
    AddOpToString (AddOperator a);

    enum MultiplyOperator {TIMES, DIVIDE, MOD};

    std::string
    MultiplyOpToString (MultiplyOperator a);

    enum RelationalOperator {LTE, LT, GT, GTE, EQ, NEQ};

    std::string
    RelationalOpToString (RelationalOperator a);
}

//========================================================================
// Forward declarations for referencing 

struct IVisitor;
struct Node; 
struct ProgramNode;
// declarations 
struct DeclarationNode; 
struct VariableDeclarationNode; 
struct ArrayDeclarationNode;
struct FunctionDeclarationNode; 
// Params 
struct ParameterNode;
// statments
struct StatementNode; 
struct IfStatementNode;
struct WhileStatementNode;
struct ForStatementNode; 
struct ExpressionStatementNode; 
struct ReturnStatementNode;
struct CompoundStatementNode;
// expressions
struct ExpressionNode;
struct AdditiveExpressionNode;
struct MultiplicativeExpressionNode;
struct RelationalExpressionNode;
struct AssignmentExpressionNode;
struct VariableExpressionNode;
struct SubscriptExpressionNode;
struct CallExpressionNode;
struct IntegerLiteralExpressionNode;
struct UnaryExpressionNode;

//========================================================================

struct IVisitor
{
    virtual void visit (ProgramNode* node) = 0;

    virtual void visit (DeclarationNode* node) = 0;
    virtual void visit (FunctionDeclarationNode* node) = 0;
    virtual void visit (VariableDeclarationNode* node) = 0;
    virtual void visit (ArrayDeclarationNode* node) = 0;
    virtual void visit (ParameterNode* node) = 0;

    virtual void visit (StatementNode* node) = 0;
    virtual void visit (CompoundStatementNode* node) = 0;
    virtual void visit (IfStatementNode* node) = 0;
    virtual void visit (WhileStatementNode* node) = 0;
    virtual void visit (ForStatementNode* node) = 0;
    virtual void visit (ReturnStatementNode* node) = 0;
    virtual void visit (ExpressionStatementNode* node) = 0;

    virtual void visit (ExpressionNode* node) = 0;
    virtual void visit (AssignmentExpressionNode* node) = 0;
    virtual void visit (VariableExpressionNode* node) = 0;
    virtual void visit (SubscriptExpressionNode* node) = 0;
    virtual void visit (CallExpressionNode* node) = 0;
    virtual void visit (AdditiveExpressionNode* node) = 0;
    virtual void visit (MultiplicativeExpressionNode* node) = 0;
    virtual void visit (RelationalExpressionNode* node) = 0;
    virtual void visit (UnaryExpressionNode* node) = 0;
    virtual void visit (IntegerLiteralExpressionNode* node) = 0;
};

//========================================================================

struct Node 
{
    virtual ~Node() {}

    virtual void
    accept (IVisitor* visitor) = 0;
};

//========================================================================

struct ProgramNode : Node 
{
    //--------------------------------------------------------------------
    // Class Members 
    std::vector<DeclarationNode*> m_declarations; 

    //--------------------------------------------------------------------

    ProgramNode (std::vector<DeclarationNode*> declarations);

    //--------------------------------------------------------------------

    virtual ~ProgramNode ();
    
    //--------------------------------------------------------------------
    
    virtual void 
    accept (IVisitor* visitor);

    //--------------------------------------------------------------------
};

//========================================================================

struct DeclarationNode : Node
{
    //--------------------------------------------------------------------
    // Class Members 
    CMinusAST::Type m_type;
    std::string m_id; 
    int m_nestLevel; 
    int m_lineno;
    int m_columnno;
    
    //--------------------------------------------------------------------

    DeclarationNode (
        CMinusAST::Type _type, 
        std::string _id, 
        int lineno, int columnno
    );

    //--------------------------------------------------------------------

    virtual ~DeclarationNode () = 0;

    //--------------------------------------------------------------------

    virtual void 
    accept (IVisitor* visitor) = 0;

    //--------------------------------------------------------------------

};

//========================================================================

struct VariableDeclarationNode : DeclarationNode
{
    //--------------------------------------------------------------------
    // Class members 
 
    //--------------------------------------------------------------------

    VariableDeclarationNode (
        CMinusAST::Type _type, 
        std::string _id,
        int lineno,
        int columnno
    );
    
    //--------------------------------------------------------------------

    virtual ~VariableDeclarationNode ();

    //--------------------------------------------------------------------

    virtual void 
    accept (IVisitor* visitor);

    //--------------------------------------------------------------------

};

//========================================================================

struct ArrayDeclarationNode : VariableDeclarationNode
{
    //--------------------------------------------------------------------
    // Class members 
    size_t m_size; 
 
    //--------------------------------------------------------------------

    ArrayDeclarationNode (
        CMinusAST::Type _type, 
        std::string _id, 
        int lineno,
        int columnno,
        size_t _size
    );
    
    //--------------------------------------------------------------------

    virtual ~ArrayDeclarationNode () {}

    //--------------------------------------------------------------------

    virtual void 
    accept (IVisitor* visitor);

    //--------------------------------------------------------------------

};

//========================================================================

struct FunctionDeclarationNode : DeclarationNode
{
    //--------------------------------------------------------------------
    // Class members 
    std::vector<ParameterNode*> m_params; 
    CompoundStatementNode* m_body;
 
    //--------------------------------------------------------------------

    FunctionDeclarationNode (
        CMinusAST::Type _type, 
        std::string _id, 
        int lineno,
        int columnno,
        std::vector<ParameterNode*> params, 
        CompoundStatementNode* body
    );
    
    //--------------------------------------------------------------------

    virtual ~FunctionDeclarationNode ();

    //--------------------------------------------------------------------

    virtual void 
    accept (IVisitor* visitor);

    //--------------------------------------------------------------------

};

//========================================================================

struct ParameterNode : DeclarationNode
{
    //--------------------------------------------------------------------
    // Class members 
    bool m_isArray;
 
    //--------------------------------------------------------------------

    ParameterNode (
        CMinusAST::Type type, 
        std::string id, 
        int lineno,
        int columnno,
        bool isArray
    );
    
    //--------------------------------------------------------------------

    virtual ~ParameterNode () {}

    //--------------------------------------------------------------------

    virtual void 
    accept (IVisitor* visitor);

    //--------------------------------------------------------------------

};

//========================================================================

struct StatementNode : Node
{
    //--------------------------------------------------------------------
    // Class members 
 
    //--------------------------------------------------------------------

    StatementNode () {}
    
    //--------------------------------------------------------------------

    virtual ~StatementNode () {}

    //--------------------------------------------------------------------

    virtual void 
    accept (IVisitor* visitor);

    //--------------------------------------------------------------------

};

//========================================================================

struct IfStatementNode : StatementNode
{
    //--------------------------------------------------------------------
    // Class members 
    ExpressionNode* m_condition;
    StatementNode*  m_then; 
    // Optional! Could be nullptr
    StatementNode*  m_else; 

    //--------------------------------------------------------------------

    IfStatementNode (
        ExpressionNode* condition, 
        StatementNode* then, 
        StatementNode* _else = nullptr
    );
    
    //--------------------------------------------------------------------

    virtual ~IfStatementNode ();

    //--------------------------------------------------------------------

    virtual void 
    accept (IVisitor* visitor);

    //--------------------------------------------------------------------

};

//========================================================================

struct WhileStatementNode : StatementNode
{
    //--------------------------------------------------------------------
    // Class members 
    ExpressionNode* m_condition;
    StatementNode*  m_statement; 

    //--------------------------------------------------------------------

    WhileStatementNode (ExpressionNode* condition, StatementNode* statement);
    
    //--------------------------------------------------------------------

    virtual ~WhileStatementNode ();

    //--------------------------------------------------------------------

    virtual void 
    accept (IVisitor* visitor);

    //--------------------------------------------------------------------

};

//========================================================================

struct ForStatementNode : StatementNode
{
    //--------------------------------------------------------------------
    // Class members 
    ExpressionNode* m_init;
    ExpressionNode* m_condition;
    ExpressionNode* m_update;
    StatementNode*  m_statement; 

    //--------------------------------------------------------------------

    ForStatementNode (
        ExpressionNode* init, 
        ExpressionNode* condition, 
        ExpressionNode* update, 
        StatementNode* statement
    );
    
    //--------------------------------------------------------------------

    virtual ~ForStatementNode ();

    //--------------------------------------------------------------------

    virtual void 
    accept (IVisitor* visitor);

    //--------------------------------------------------------------------

};

//========================================================================

struct ExpressionStatementNode : StatementNode
{
    //--------------------------------------------------------------------
    // Class members 
    // Optional! Could be nullptr
    ExpressionNode* m_expression;

    //--------------------------------------------------------------------

    ExpressionStatementNode (ExpressionNode* expression);
    
    //--------------------------------------------------------------------

    virtual ~ExpressionStatementNode ();

    //--------------------------------------------------------------------

    virtual void 
    accept (IVisitor* visitor);

    //--------------------------------------------------------------------

};

//========================================================================

struct ReturnStatementNode : StatementNode
{
    //--------------------------------------------------------------------
    // Class members 
    // Optional! Could be nullptr
    ExpressionNode* m_expression;

    //--------------------------------------------------------------------

    ReturnStatementNode (ExpressionNode* expression = nullptr);
    
    //--------------------------------------------------------------------

    virtual ~ReturnStatementNode ();

    //--------------------------------------------------------------------

    virtual void 
    accept (IVisitor* visitor);

    //--------------------------------------------------------------------

};

//========================================================================

struct CompoundStatementNode : StatementNode
{
    //--------------------------------------------------------------------
    // Class members 
    std::vector<VariableDeclarationNode*> m_vars; // <-- should be a var declaration
    std::vector<StatementNode*>   m_statements; 

    //--------------------------------------------------------------------

    CompoundStatementNode (
        std::vector<VariableDeclarationNode*> vars, 
        std::vector<StatementNode*> statements
    );
    
    //--------------------------------------------------------------------

    virtual ~CompoundStatementNode ();

    //--------------------------------------------------------------------

    virtual void 
    accept (IVisitor* visitor);

    //--------------------------------------------------------------------

};

//========================================================================

struct ExpressionNode : Node
{
    //--------------------------------------------------------------------
    // Class members 

    //--------------------------------------------------------------------

    ExpressionNode ();
    
    //--------------------------------------------------------------------

    virtual ~ExpressionNode ();

    //--------------------------------------------------------------------

    virtual void 
    accept (IVisitor* visitor);

    //--------------------------------------------------------------------

};

//========================================================================

struct AdditiveExpressionNode : ExpressionNode
{
    //--------------------------------------------------------------------
    // Class members 
    ExpressionNode*            m_lhs;
    CMinusAST::AddOperator     m_addOp;
    ExpressionNode*            m_rhs; 

    //--------------------------------------------------------------------

    AdditiveExpressionNode (
        ExpressionNode* lhs, 
        CMinusAST::AddOperator op, 
        ExpressionNode* rhs
    ); 
    
    //--------------------------------------------------------------------

    virtual ~AdditiveExpressionNode ();

    //--------------------------------------------------------------------

    virtual void 
    accept (IVisitor* visitor);

    //--------------------------------------------------------------------

};

//========================================================================

struct MultiplicativeExpressionNode : ExpressionNode
{
    //--------------------------------------------------------------------
    // Class members 
    ExpressionNode*             m_lhs;
    CMinusAST::MultiplyOperator m_multOp;
    ExpressionNode*             m_rhs; 

    //--------------------------------------------------------------------

    MultiplicativeExpressionNode (
        ExpressionNode* lhs, 
        CMinusAST::MultiplyOperator op, 
        ExpressionNode* rhs
    );
    
    //--------------------------------------------------------------------

    virtual ~MultiplicativeExpressionNode ();

    //--------------------------------------------------------------------

    virtual void 
    accept (IVisitor* visitor);

    //--------------------------------------------------------------------

};

//========================================================================

struct RelationalExpressionNode : ExpressionNode
{
    //--------------------------------------------------------------------
    // Class members 
    ExpressionNode*               m_lhs;
    CMinusAST::RelationalOperator m_relOp;
    ExpressionNode*               m_rhs; 

    //--------------------------------------------------------------------

    RelationalExpressionNode (
        ExpressionNode* lhs, 
        CMinusAST::RelationalOperator op, 
        ExpressionNode* rhs
    );
    
    //--------------------------------------------------------------------

    virtual ~RelationalExpressionNode ();

    //--------------------------------------------------------------------

    virtual void 
    accept (IVisitor* visitor);

    //--------------------------------------------------------------------

};

//========================================================================

struct AssignmentExpressionNode : ExpressionNode
{
    //--------------------------------------------------------------------
    // Class members 
    VariableExpressionNode*    m_lhs;
    // CMinusAST::AssignOperator m_assignOp; <-- could be useful for += -= etc
    ExpressionNode*       m_rhs; 

    //--------------------------------------------------------------------

    AssignmentExpressionNode (VariableExpressionNode* lhs, ExpressionNode* rhs);
    
    //--------------------------------------------------------------------

    virtual ~AssignmentExpressionNode ();

    //--------------------------------------------------------------------

    virtual void 
    accept (IVisitor* visitor);

    //--------------------------------------------------------------------

};

//========================================================================

struct VariableExpressionNode : ExpressionNode
{
    //--------------------------------------------------------------------
    // Class members 
    CMinusAST::Type m_type;
    std::string m_id;
    int m_lineno;
    int m_columnno; 

    //--------------------------------------------------------------------

    VariableExpressionNode (
        std::string id,
        int lineno,
        int columnno
    );
    
    //--------------------------------------------------------------------

    virtual ~VariableExpressionNode ();

    //--------------------------------------------------------------------

    virtual void 
    accept (IVisitor* visitor);

    //--------------------------------------------------------------------

};

//========================================================================

struct SubscriptExpressionNode : VariableExpressionNode
{
    //--------------------------------------------------------------------
    // Class members 
    ExpressionNode* m_subscript;

    //--------------------------------------------------------------------

    SubscriptExpressionNode (
        std::string id, 
        int lineno, 
        int columnno,
        ExpressionNode* subscript
    );
    
    //--------------------------------------------------------------------

    virtual ~SubscriptExpressionNode ();

    //--------------------------------------------------------------------

    virtual void 
    accept (IVisitor* visitor);

    //--------------------------------------------------------------------

};

//========================================================================

struct CallExpressionNode : ExpressionNode
{
    //--------------------------------------------------------------------
    // Class members 
    CMinusAST::Type m_type;
    std::string m_id; 
    int m_lineno;
    int m_columnno;
    std::vector<ExpressionNode*> m_args;

    //--------------------------------------------------------------------

    CallExpressionNode (
        std::string id, 
        int lineno,
        int columnno,
        std::vector<ExpressionNode*> args
    );
    
    //--------------------------------------------------------------------

    virtual ~CallExpressionNode ();

    //--------------------------------------------------------------------

    virtual void 
    accept (IVisitor* visitor);

    //--------------------------------------------------------------------

};

//========================================================================

struct IntegerLiteralExpressionNode : ExpressionNode
{
    //--------------------------------------------------------------------
    // Class members 
    int m_value;

    //--------------------------------------------------------------------

    IntegerLiteralExpressionNode (int value);
    
    //--------------------------------------------------------------------

    virtual ~IntegerLiteralExpressionNode ();

    //--------------------------------------------------------------------

    virtual void 
    accept (IVisitor* visitor);

    //--------------------------------------------------------------------

};

//========================================================================

#endif