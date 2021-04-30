// Filename:   Parser.cc
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
#include <initializer_list>

#include "Lexer.h"
#include "Parser.h"
#include "CMinusAst.h"

//========================================================================
// Constructs a parser to process the given vector of tokens 
// if doDebug is true, then debug messages will be outputted 

Parser::Parser(std::vector<Token> tokens, bool doDebug=false)
{
    m_tokens = tokens;
    m_currentToken = 0;
    m_level = 0;
    m_doDebug = doDebug;
}

//========================================================================
// advances the current token if it matches expected, otherwise, 
// throw an error

void
Parser::match (std::string function, 
    std::initializer_list<TokenType> expectedTokenTypes, 
    std::string additional="")
{
    for (TokenType expectedToken : expectedTokenTypes)
    {
        // if the current token is expected 
        if (m_tokens[m_currentToken].type == expectedToken)
        {
            // advance to the next token
            ++m_currentToken;
            // and stop trying to match 
            return; 
        }
    }
    // reaches here if token is unexpected 
    error (function, expectedTokenTypes, additional);
}

//========================================================================

void
Parser::error (std::string function, 
    std::initializer_list<TokenType> expectedTokenTypes,
    std::string additional="")
{
    printf ("Error while parsing '%s'\n", function.c_str());
    printf ("  Encountered: '%s' (line %d, column %d)\n", 
        m_tokens[m_currentToken].lexeme.c_str(), 
        m_tokens[m_currentToken].lineno,
        m_tokens[m_currentToken].columnno
    );
    printf ("  Expected   : ");
    if (expectedTokenTypes.size() > 0)
    {
        auto iter = expectedTokenTypes.begin ();
        printf ("%s", tokenToString(*iter++));
        for ( ; iter != expectedTokenTypes.end (); ++iter)
            printf (" or %s", tokenToString(*iter));
    }
    printf ("\n");
    // additional error info
    if (additional.size() > 0)
        printf ("  Additional : %s\n", additional.c_str ());
    exit(1);
}

//========================================================================

void 
Parser::enter (std::string name)
{
    if (!m_doDebug) return; 
    printSpaces (m_level++);
    printf ("+-%s: Enter, \t", name.c_str());
    printf ("Token == %s %d:%d\n", 
        m_tokens[m_currentToken].lexeme.c_str(),
        m_tokens[m_currentToken].lineno,
        m_tokens[m_currentToken].columnno
    );
}

//========================================================================

void 
Parser::leave (std::string name)
{
    if (!m_doDebug) return; 
    printSpaces (--m_level);
    printf ("+-%s: Leave, \t", name.c_str());
    printf ("Token == %s %d:%d\n", 
        m_tokens[m_currentToken].lexeme.c_str(),
        m_tokens[m_currentToken].lineno,
        m_tokens[m_currentToken].columnno
    );
}

//========================================================================

void
Parser::printSpaces (int level)
{
  while (level-- > 0)
    printf ("| ");
}

//========================================================================
// Attempts to parse the tokens 
// Starts by trying to match a <program>

Node*
Parser::parse()
{
    return program ();
}

//========================================================================
// <program> -> <declaration-list>

ProgramNode* 
Parser::program ()
{
    enter ("program");

    std::vector<DeclarationNode*> declarations = declarationList ();

    leave ("program");

    return new ProgramNode(declarations);
}

//========================================================================
/* Declarations - 1 or more declarations 
     a declaration can be predicted from a type specifier
     which is predicted by INT or VOID
     <declarationList> -> declaration { declaration }
*/ 
std::vector<DeclarationNode*>
Parser::declarationList ()
{
    enter ("declarationList");

    std::vector<DeclarationNode*> declarations; 
    // must have at least 1 declaration
    declarations.push_back(declaration());
    // 0 or more extra declarations 
    // declarations start with type-specifiers 
    // which always start with either INT or VOID
    while (m_tokens[m_currentToken].type == INT
        || m_tokens[m_currentToken].type == VOID)
    {
        // match the declaration 
        declarations.push_back(declaration());
    }

    // should end in EOF
    if (m_tokens[m_currentToken].type != END_OF_FILE)
        error (
            "declarationList", 
            {INT, VOID}, 
            "Only declarations are allowed at program scope."
        );

    leave ("declarationList");

    return declarations;
}

//========================================================================
/* Declarations 
    Expands to a variable declaration 
        int x;
        int x[];
    or to a function declaration
        int add (int a, int b) {return a + b;}

    Can be predicted from the third lookahead 
    - ( means function declaration
    - [ or ; means varDeclaration

    <declaration> -> <typeSpecifier> ID ( <varDeclaration> | <funDeclaration> )
*/
DeclarationNode* 
Parser::declaration ()
{
    enter ("declaration");

    // setup declaration node 
    // declaration could be a var or a function 
    DeclarationNode* node; 

    // declaration starts with <typeSpec> 
    CMinusAST::Type type = typeSpecifier ();

    // save location information
    int lineno = m_tokens[m_currentToken].lineno;
    int columnno = m_tokens[m_currentToken].columnno;
    // and an ID follows 
    std::string var = m_tokens[m_currentToken].lexeme;
    match ("declaration", {ID});

    // <declaration> -> <typeSpecifier> ID <varDeclaration>
    if (m_tokens[m_currentToken].type == SEMI
        || m_tokens[m_currentToken].type == LBRACK)
    {
        node = varDeclaration (type, var, lineno, columnno);
    }
    // <declaration> -> <typeSpecifier> ID <funDeclaration>
    else if (m_tokens[m_currentToken].type == LPAREN)
    {
        node = funDeclaration (type, var, lineno, columnno);
    }
    // Unexpected token in lookahead
    else
    {
        // expected ; or [ or (
        error ("declaration", {SEMI, LBRACK, LPAREN});
    }

    leave ("declaration");

    return node; 
}

//========================================================================
/* Variable declaration 
    Example:
        int x;
        void y[];
    ** typeSpecifier and ID were already matched in declaration

    <varDeclaration> -> <typeSpecifier> ID [ '[' NUM ']' ] ;
*/
VariableDeclarationNode* 
Parser::varDeclaration (
    CMinusAST::Type type, 
    std::string var, 
    int lineno, 
    int columnno
)
{
    enter ("varDeclaration");

    VariableDeclarationNode* node; 

    // optional array designation
    if (m_tokens[m_currentToken].type == LBRACK)
    {
        match ("varDeclaration", {LBRACK});
        int size = m_tokens[m_currentToken].number;
        match ("varDeclaration", {NUM});
        match ("varDeclaration", {RBRACK});
        // this is an array declaration
        node = new ArrayDeclarationNode(type, var, lineno, columnno, size);
    }
    // not an array
    else
    {
        node = new VariableDeclarationNode(type, var, lineno, columnno);
    }

    // ends in a semi colon 
    match ("varDeclaration", {SEMI});

    leave ("varDeclaration");

    return node; 
}

//========================================================================
/* Type Specifier
    Denotes the type for variables and functions
    Example:
        int x; 
    'int' would be the type specifier 
    <typeSpecifier> -> INT | VOID
*/
CMinusAST::Type 
Parser::typeSpecifier ()
{
    enter ("typeSpecifier");

    CMinusAST::Type type; 

    // INT
    if (m_tokens[m_currentToken].type == INT)
    {
        match ("typeSpecifier", {INT});
        type = CMinusAST::INT;
    }
    // VOID
    else if (m_tokens[m_currentToken].type == VOID)
    {
        match ("typeSpecifier", {VOID});
        type = CMinusAST::VOID;
    }
    // Invalid
    else 
    {
        error ("typeSpecifier", {INT, VOID});
        type = CMinusAST::VOID; 
    }

    leave ("typeSpecifier");

    return type;

}

//========================================================================
/* Function declaration
    Example:
        void main (int argc) {return 0;}
    **typeSpecifier and ID have already been matched by declaration
    <funDeclaration> -> <typeSpecifier> ID '(' <params> ')' compoundStatement
*/
FunctionDeclarationNode*
Parser::funDeclaration (
    CMinusAST::Type type, 
    std::string var, 
    int lineno, 
    int columnno
)
{
    enter ("funDeclaration");

    match ("funDeclaration", {LPAREN});
    std::vector<ParameterNode*> p = params (); 
    match ("funDeclaration", {RPAREN});
    CompoundStatementNode* compound = compoundStatement ();

    leave ("funDeclaration");

    return new FunctionDeclarationNode(type, var, lineno, columnno, p, compound);
}

//========================================================================
/* Parameters 
    This either matches a parameter list or void.
    This requires 2 steps of lookahead because VOID 
        is common between both cases 
    Examples:
        int main (void) {return 0;}
        int sum (void a, void b) {return a+b;}
    <params> -> <paramList> | VOID
*/
std::vector<ParameterNode*>  
Parser::params ()
{
    enter ("params");

    std::vector<ParameterNode*> ParameterNodes; 

    // <params> -> VOID
    if (m_tokens[m_currentToken].type == VOID
        && m_tokens[m_currentToken+1].type != ID)
    {
        match ("params", {VOID});
    }
    // <params -> <paramList> 
    else 
    {
        ParameterNodes = paramList ();
    }

    leave ("params");

    return ParameterNodes; 
}

//========================================================================
/* Parameter list 
    A list of parameters separated by commas 
    Examples: 
        void a, int b, void w
        int c
    Parameter lists should always be inside parentheses 
        so the lookahead is always COMMA
    <paramList> -> <param> { , <param> }

*/
std::vector<ParameterNode*> 
Parser::paramList ()
{
    enter ("paramList");

    std::vector<ParameterNode*> ParameterNodes; 

    // match at least one <param>
    ParameterNodes.push_back(param());
    // { , <param> }
    while (m_tokens[m_currentToken].type == COMMA)
    {
        match ("param", {COMMA});
        ParameterNodes.push_back(param());
    }

    leave ("paramList");

    return ParameterNodes; 
}

//========================================================================
/* Parameter
    a parameter consists of a type and an ID and optionally brackets for arrays 
    Examples:
        void x
        int nums[]
    <param> -> <typeSpecifier> ID [ '[' ']' ]
*/
ParameterNode* 
Parser::param ()
{
    enter ("param");

    ParameterNode* node; 

    CMinusAST::Type type = typeSpecifier ();

    // save location information
    int lineno = m_tokens[m_currentToken].lineno;
    int columnno = m_tokens[m_currentToken].columnno;
    std::string var = m_tokens[m_currentToken].lexeme;

    match ("param", {ID});
    
    // [ '[' ']' ]
    // is an array 
    if (m_tokens[m_currentToken].type == LBRACK)
    {
        match ("param", {LBRACK});
        match ("param", {RBRACK});
        node = new ParameterNode(type, var, lineno, columnno, true);
    }
    // not array 
    else
    {
        node = new ParameterNode(type, var, lineno, columnno, false);
    }

    leave ("param");

    return node;
}

//========================================================================
/* Compound statements
    A compound statement represents a block of code 
        where the declarations are at the top and statements follow 
    Examples:
        {int a; int b; b = 10; a = b + 7;}
    <compoundStatements> -> '{' <localDeclarations> <statementList> '}'
*/
CompoundStatementNode* 
Parser::compoundStatement ()
{
    enter ("compoundStatement");

    match ("compoundStatement", {LBRACE});
    std::vector<VariableDeclarationNode*> vars = localDeclarations ();
    std::vector<StatementNode*> statements = statementList ();
    match ("compoundStatement", {RBRACE});

    leave ("compoundStatement");

    return new CompoundStatementNode(vars, statements);
}

//========================================================================
/* local declarations 
    A local declaration consists of a type, identifier, and semicolon. 
    And there can be 0 or more of those declarations 
    Example:
        int x; 
        void y;
    Can be predicted from the type specifier being either INT or VOID
    <localDeclarations> -> { <typeSpecifier> ID <varDeclaration> }
*/
std::vector<VariableDeclarationNode*> 
Parser::localDeclarations ()
{
    enter ("localDeclarations");

    std::vector<VariableDeclarationNode*> decls; 

    // { <varDeclaration> }
    while (m_tokens[m_currentToken].type == INT 
        || m_tokens[m_currentToken].type == VOID)
    {
        // <typeSpecifier> 
        CMinusAST::Type type = typeSpecifier ();

        // save location information
        int lineno = m_tokens[m_currentToken].lineno;
        int columnno = m_tokens[m_currentToken].columnno;

        // ID
        std::string var = m_tokens[m_currentToken].lexeme;
        match ("declaration", {ID});

        // <varDeclaration>
        decls.push_back (varDeclaration (type, var, lineno, columnno));
    }

    leave ("localDeclarations");

    return decls; 
}

//========================================================================
/* List of statements 
    there can be 0 or more statements in a statement list 
    Example:
        x = 5; y = x * 2; while (x == 3) x = x + 1;
    Statements can start with ID, (, NUM, {, WHILE, IF, RETURN 
    or be empty 
    <statementList> -> { <statement> }
*/
std::vector<StatementNode*> 
Parser::statementList ()
{
    enter ("statementList");

    std::vector<StatementNode*> statements; 

    // Statements can start with ID, (, NUM, {, WHILE, IF, RETURN 
    while (m_tokens[m_currentToken].type == ID
        || m_tokens[m_currentToken].type == LPAREN
        || m_tokens[m_currentToken].type == NUM
        || m_tokens[m_currentToken].type == LBRACE
        || m_tokens[m_currentToken].type == IF
        || m_tokens[m_currentToken].type == WHILE
        || m_tokens[m_currentToken].type == RETURN)
    {
        statements.push_back(statement ());
    }

    leave ("statementList");

    return statements; 
}

//========================================================================
/* Statements 
    A statement expands to expressions, code blocks, conditionals,
        loops, and return statements. 
    Examples:
        x = 10 + 3;
        {x = y + z; print(x);}
        if (x == 0) x = 20;
        while (y == 0) x = x + 2;
        while (x == 3) ; 
        return 0;
    Each type of statement has a unique lookahead 
    <statement> -> <expressionStatement>
                |  <compoundStatement>
                |  <selectionStatement>
                |  <iterationStatement>
                |  <returnStatement>
*/
StatementNode* 
Parser::statement ()
{
    enter ("statement");

    StatementNode* stmt; 

    // <statement> -> <compoundStatement>
    // lookahead == {
    if (m_tokens[m_currentToken].type == LBRACE)
    {
        stmt = compoundStatement ();
    }
    // <statement> -> <selectionStatement>
    // lookahead == IF
    else if (m_tokens[m_currentToken].type == IF)
    {
        stmt = selectionStatement ();
    }
    // <statement> -> <iterationStatement>
    // lookahead == WHILE
    else if (m_tokens[m_currentToken].type == WHILE)
    {
        stmt = iterationStatement ();
    }
    // <statement> -> <returnStatement>
    // lookahead == RETURN
    else if (m_tokens[m_currentToken].type == RETURN)
    {
        stmt = returnStatement ();
    }
    // <statement> -> <expressionStatement>
    // lookahead == ID | ( | NUM | ;
    else if (m_tokens[m_currentToken].type == ID
        ||   m_tokens[m_currentToken].type == LPAREN
        ||   m_tokens[m_currentToken].type == NUM
        ||   m_tokens[m_currentToken].type == SEMI)
    {
        stmt = expressionStatement ();
    }
    // invalid token in lookahead
    else
    {
        error ("statement", {LBRACE, IF, WHILE, RETURN, ID, LPAREN, NUM});
    }

    leave ("statement");

    return stmt; 
}

//========================================================================
/* Expression statements 
    Expression statements are expressions that end in a semicolon 
    <expressionStatement> -> [ <expression> ] ; 
*/
ExpressionStatementNode* 
Parser::expressionStatement ()
{
    enter ("expressionStatement");

    ExpressionNode* expr = nullptr; 

    // if we dont see a semicolon then we expect there to be an expression
    if (m_tokens[m_currentToken].type != SEMI)
    {
        expr = expression ();
    }
    // match the semicolon
    match ("expressionStatement", {SEMI}, "You may be missing a semicolon");

    leave ("expressionStatement");

    return new ExpressionStatementNode(expr);
}

//========================================================================
/* Selection statement
    This consists of if statements and an optional matched else 
    <selectionStatement> 
        -> IF '(' <expression> ')' <statement> [ ELSE <statment>]
*/  
IfStatementNode* 
Parser::selectionStatement ()
{
    enter ("selectionStatement");

    // IF
    match ("selectionStatement", {IF});
    // '('
    match ("selectionStatement", {LPAREN});
    // <expression>
    ExpressionNode* cond = expression ();
    // ')'
    match ("selectionStatement", {RPAREN});
    // <statement>
    StatementNode* then = statement ();
    // [ ELSE <statement> ]
    // else component is optional
    StatementNode* else_ = nullptr;
    if (m_tokens[m_currentToken].type == ELSE)
    {
        match ("selectionStatement", {ELSE});
        else_ = statement ();
    }

    leave ("selectionStatement");

    return new IfStatementNode(cond, then, else_);
}

//========================================================================
/* Iteration statement
    This consists of loops like a while loop
    Example:
        while (x == 0) {y = y + 1;}
    <iterationStatement> -> WHILE '(' <expression> ')' <statement>
*/
WhileStatementNode*  
Parser::iterationStatement ()
{
    enter ("iterationStatement");

    // WHILE
    match ("iterationStatment", {WHILE});
    // (
    match ("iterationStatment", {LPAREN});
    // <expression>
    ExpressionNode* cond = expression ();
    // )
    match ("iterationStatment", {RPAREN});
    // <statement>
    StatementNode* then = statement ();

    leave ("iterationStatement");

    return new WhileStatementNode(cond, then);
}

//========================================================================
/* return statements
    Example:
        return x + 10; 
        return ;
    <returnStatement> -> RETURN [ <expression> ] ;
*/
ReturnStatementNode*  
Parser::returnStatement ()
{
    enter ("returnStatement");

    // RETURN
    match ("returnStatement", {RETURN});

    // [ <expression> ]
    // Optional
    ExpressionNode* expr = nullptr;
    if (m_tokens[m_currentToken].type != SEMI)
    {
        expr = expression ();
    }
    // ;
    match ("returnStatement", {SEMI});

    leave ("returnStatement");

    return new ReturnStatementNode(expr);
}

//========================================================================
/* Expression 
    Examples:
        x = 10
        y = x = 5
        6 + 3 * 4
        nums[k+i] = 10
        nums[k+i] + 10
    ** last two cases cause the most trouble
        there can be a variable amount of tokens in <var>
        so I am using an ad hoc approach of assuming it's <var> = 
        and restoring state to the start of <var> if I am wrong 
    <expression> -> { <var> = } <simpleExpression>
*/
ExpressionNode*  
Parser::expression ()
{
    enter ("expression");

    // Stores each matched assignment expression
    // x = y = z = 10 + 3;
    // is optional 
    AssignmentExpressionNode* root = nullptr;
    AssignmentExpressionNode* lastAssign = nullptr;

    // requires variable amount of lookahead because <var> and <simpleExpression>
    // both can start with ID and <var> can have a variable amount of tokens 
    // before '=' is seen 
    // My technique is to save the position of the start of the <var> and assume that 
    // it is <var> = 
    // if = is not present, then my assumption was wrong 
    // but I can restore the state and then match <simpleExpression> 
    while (m_tokens[m_currentToken].type == ID)
    {
        // there is a <var> 
        int varIndex = m_currentToken;
        // match the var 
        VariableExpressionNode* v = var ();
        // if there is an ASSIGN
        if (m_tokens[m_currentToken].type == ASSIGN)
        {
            // then the assumption was correct 
            // match the ASSIGN op and try again 
            match ("expression", {ASSIGN});
            // make a new assignment statement
            // save the var 
            AssignmentExpressionNode* rhs = new AssignmentExpressionNode(v, nullptr);
            // if this is the first assignment expression
            if (root == nullptr)
            {
                // make this the root of the assignment expressions
                root = rhs;
                // set up prev assign 
                lastAssign = root;
            }
            // if this is not the first assignment expression
            else
            {
                // add to the last assign expression
                lastAssign->m_rhs = rhs; 
                // advance last assign expression
                lastAssign = rhs; 
            }
            
        }
        // no ASSIGN op 
        else
        {
            // assumption was wrong, the <var> should be a part of 
            // a <simpleExpression> 
            // restore matched <var> 
            m_currentToken = varIndex; 
            // stop trying to match <var> = 
            break; 
        }
    }
    // should end in a simpleExpression
    ExpressionNode* expr = simpleExpression ();

    leave ("expression");

    // if there was at least one assign statement 
    if (root != nullptr)
    {
        // add this as the rhs of the last assign statement
        lastAssign->m_rhs = expr; 
        // the whole assignment statement becomes the
        // target expression to return
        return root; 
    }

    // otherwise expr is the only expression
    return expr; 
}

//========================================================================
/* variable
    Consists of an ID and optional subscript operator with expression 
    Examples:
        x
        y[x + 2]
    <var> -> ID [ '[' <expression> ']' ]
*/
VariableExpressionNode* 
Parser::var ()
{
    enter ("var");

    VariableExpressionNode* node = nullptr; 
     
    // save location information
    int lineno = m_tokens[m_currentToken].lineno;
    int columnno = m_tokens[m_currentToken].columnno;
    // match the ID
    std::string varname = m_tokens[m_currentToken].lexeme;
    match ("var", {ID});
    // optionally match the subscript operation
    // if there is an lbracket 
    if (m_tokens[m_currentToken].type == LBRACK)
    {
        match ("var", {LBRACK});
        ExpressionNode* expr = expression ();
        match ("var", {RBRACK});
        node = new SubscriptExpressionNode(varname, lineno, columnno, expr);
    }
    // just ID
    else
    {
        node = new VariableExpressionNode(varname, lineno, columnno);
    }

    leave ("var");

    return node; 
}

//========================================================================
/* Simple Expression
    Only one relational operation is allowed 
    Examples: 
        a == b
        b > 7
    <simpleExpression> 
        -> <additiveExpression> [ <relop> <additiveExpression> ]
*/
ExpressionNode*
Parser::simpleExpression ()
{
    enter ("simpleExpression");

    ExpressionNode* lhs = additiveExpression ();

    // optionally a boolean expression 
    if (m_tokens[m_currentToken].type == LT
        || m_tokens[m_currentToken].type == LTE
        || m_tokens[m_currentToken].type == GT
        || m_tokens[m_currentToken].type == GTE
        || m_tokens[m_currentToken].type == EQ
        || m_tokens[m_currentToken].type == NEQ)
    {
        CMinusAST::RelationalOperator relOp = relop (); 
        ExpressionNode* rhs = additiveExpression ();
        // is a relational expression
        lhs = new RelationalExpressionNode(lhs, relOp, rhs);
    }

    leave ("simpleExpression");

    return lhs;
}

//========================================================================
/* relational operator 
    <relop> -> LTE, LT, GT, GTE, EQ, NEQ
*/
CMinusAST::RelationalOperator 
Parser::relop ()
{
    enter ("relop");

    CMinusAST::RelationalOperator relOp; 

    if (m_tokens[m_currentToken].type == LT)
        relOp = CMinusAST::LT;
    else if (m_tokens[m_currentToken].type == LTE)
        relOp = CMinusAST::LTE;
    else if (m_tokens[m_currentToken].type == GT)
        relOp = CMinusAST::GT;
    else if (m_tokens[m_currentToken].type == GTE)
        relOp = CMinusAST::GTE;
    else if (m_tokens[m_currentToken].type == EQ)
        relOp = CMinusAST::EQ;
    else if (m_tokens[m_currentToken].type == NEQ)
        relOp = CMinusAST::NEQ;

    // Match LT, LTE, GT, GTE, EQ, or NEQ
    match ("relop", {LT, LTE, GT, GTE, EQ, NEQ});

    leave ("relop");

    return relOp; 
}

//========================================================================
/* Additive Expression
    Handles the precedence level for addition and subtraction 
    Examples:
        a + 5
        6 - 3
    <additiveExpression> -> <term> { <addop> <term> }
*/
ExpressionNode* 
Parser::additiveExpression ()
{
    enter ("additiveExpression");

    ExpressionNode* lhs = term ();

    // addition/subtraction
    while (m_tokens[m_currentToken].type == PLUS
        || m_tokens[m_currentToken].type == MINUS)
    {
        CMinusAST::AddOperator addOp = addop ();
        ExpressionNode* rhs = term ();
        // add this with the lhs 
        lhs = new AdditiveExpressionNode (lhs, addOp, rhs);
    }

    leave ("additiveExpression");

    return lhs;
}

//========================================================================
/*
    <addop> -> PLUS | MINUS
*/
CMinusAST::AddOperator 
Parser::addop ()
{
    enter ("addop");

    CMinusAST::AddOperator op;

    if (m_tokens[m_currentToken].type == PLUS)
        op = CMinusAST::ADD;
    else if (m_tokens[m_currentToken].type == MINUS)
        op = CMinusAST::MINUS;

    // match either PLUS or MINUS
    match ("addop", {PLUS, MINUS});

    leave ("addop");

    return op;
}

//========================================================================
/* term
    multiplication/division precedence 
    <term> -> <factor> { <mulop> <factor> }
*/
ExpressionNode*  
Parser::term ()
{
    enter ("term");

    ExpressionNode* lhs = factor ();

    // multiplication/division
    while (m_tokens[m_currentToken].type == TIMES
        || m_tokens[m_currentToken].type == DIVIDE)
    {
        CMinusAST::MultiplyOperator op = mulop ();
        ExpressionNode* rhs = factor ();
        // add mult op to the expression chain
        lhs = new MultiplicativeExpressionNode(lhs, op, rhs);
    }

    leave ("term");

    return lhs; 
}

//========================================================================
// <mulop> -> TIMES | DIVIDE

CMinusAST::MultiplyOperator 
Parser::mulop ()
{
    enter ("mulop");

    CMinusAST::MultiplyOperator op; 

    if (m_tokens[m_currentToken].type == TIMES)
        op = CMinusAST::TIMES;
    else if (m_tokens[m_currentToken].type == DIVIDE)
        op = CMinusAST::DIVIDE;

    // match either TIMES or DIVIDE
    match ("mulop", {TIMES, DIVIDE});

    leave ("mulop");

    return op;
}

//========================================================================
/* factor 
    Parentheses precedence
    <factor> -> '(' <expression> ')' 
              | <var> 
              | <call>
              | NUM
*/
ExpressionNode* 
Parser::factor ()
{
    enter ("factor");

    ExpressionNode* expr = nullptr; 

    // <factor> -> '(' <expression> ')'
    if (m_tokens[m_currentToken].type == LPAREN)
    {
        match ("factor", {LPAREN});
        expr = expression ();
        match ("factor", {RPAREN});
    }
    // <factor> -> NUM
    else if (m_tokens[m_currentToken].type == NUM)
    {
        int value = m_tokens[m_currentToken].number;
        match ("factor", {NUM});
        expr = new IntegerLiteralExpressionNode(value);
    }
    // <factor> -> <call>
    // need 2 layers of lookahead because 
    // <call> and <var> both start with ID
    // but <call> needs an LPAREN after the ID
    else if (m_tokens[m_currentToken].type == ID
        && m_tokens[m_currentToken+1].type == LPAREN)
    {
        expr = call ();
    }
    // <factor> -> <var> 
    else if (m_tokens[m_currentToken].type == ID)
    {
        expr = var ();
    }
    // invalid token 
    else 
    {
        error ("factor", {LPAREN});
    }

    leave ("factor");

    return expr;
}

//========================================================================
// <call> -> ID '(' <args> ')'

CallExpressionNode* 
Parser::call ()
{
    enter ("call");

    // save location information
    int lineno = m_tokens[m_currentToken].lineno;
    int columnno = m_tokens[m_currentToken].columnno;
    std::string id = "";
    if (m_tokens[m_currentToken].type == ID)
        id = m_tokens[m_currentToken].lexeme;
    match ("call", {ID});

    match ("call", {LPAREN});

    std::vector<ExpressionNode*> a = args ();

    match ("call", {RPAREN});

    leave ("call");

    return new CallExpressionNode(id, lineno, columnno, a);
}

//========================================================================
/*  arguments 
    for function call
    can go to empty string 
    <args> -> [ <argList> ]
*/
std::vector<ExpressionNode*> 
Parser::args ()
{
    enter ("args");

    std::vector<ExpressionNode*> a; 

    // if no args, the next token will be RPAREN
    // [ <argList> ]
    if (m_tokens[m_currentToken].type != RPAREN)
    {
        a = argList ();
    }

    leave ("args");

    return a;
}

//========================================================================
/* a list of expressions separated by commas 
    <argList> -> <expression> { , <expression> }
*/
std::vector<ExpressionNode*> 
Parser::argList ()
{
    enter ("argList");

    std::vector<ExpressionNode*> a; 

    // <expression>
    a.push_back(expression ());
    // { , <expression> }
    while (m_tokens[m_currentToken].type == COMMA)
    {
        // , 
        match ("argList", {COMMA});
        // <expression>
        // save expression as an argument
        a.push_back (expression ());
    }

    leave ("argList");

    return a;
}

//========================================================================


