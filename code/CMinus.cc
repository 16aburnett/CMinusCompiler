// Filename:   CMinus.cc
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

//========================================================================
// Local includes

#include "Lexer.h"
#include "Parser.h"
#include "CMinusAst.h"
#include "PrintVisitor.h"
#include "SymbolTableVisitor.h"

//========================================================================
// Using declarations

//========================================================================
int
main (int argc, char* argv[])
{
    //=== Source Code ====================================================
    FILE* srcfile = stdin;
    std::string destFilename = "Default.ast";
    // invalid usage
    if (argc > 2)
    {
        printf ("Usage:\n");
        printf ("\t%s\n", argv[0]);
        printf ("\t\t- Compiles CMinus code from stdin\n");
        printf ("\t%s <filename>\n", argv[0]);
        printf ("\t\t- Compiles CMinus code from <filename>\n");
        return EXIT_FAILURE; 
    }
    // file provided
    if (argc == 2)
    {
        srcfile = fopen (argv[1], "r");
        // get root name of file for the dest file
        std::string srcFilename = std::string(argv[1]);
        std::string rootName = srcFilename.substr(
            0, 
            srcFilename.find_last_of('.')
        );
        // build destination filename 
        destFilename = rootName.append(".ast");
    }

    //=== Lexing =========================================================
    // lex and tokenize the source code 
    Lexer lexer (srcfile);
    std::vector<Token> tokens = lexer.tokenize ();

    //=== Parsing ========================================================
    // attempt to parse the tokens 
    // and obtain an ast 
    Parser parser (tokens, false); 
    Node* ast = parser.parse ();

    //=== Semantic Analysis ==============================================
    // Ensures that each variable was declared only once
    SymbolTableVisitor symbolTableVisitor;

    // insert input and output built-in variables 
    VariableDeclarationNode* inputFunc = new VariableDeclarationNode (CMinusAST::INT, "input", 0, 0);
    VariableDeclarationNode* outputFunc = new VariableDeclarationNode (CMinusAST::VOID, "output", 0, 0);
    symbolTableVisitor.m_symbolTable.insert (inputFunc);
    symbolTableVisitor.m_symbolTable.insert (outputFunc);

    // check the ast 
    ast->accept (&symbolTableVisitor);
    // ensure it was successful
    if (!symbolTableVisitor.m_wasSuccessful)
        return EXIT_FAILURE;

    // reaches here if code passes semantic analyzer 
    std::cout << "Valid!" << std::endl;
    
    //=== Printing AST ===================================================
    // Print out the ast 
    PrintVisitor printVisitor; 
    ast->accept (&printVisitor);
    // grab output string
    std::string output = printVisitor.m_outputString.str();
    // remove ending newline
    output = output.substr(0, output.size()-1);
    // print string to dest file 
    std::cout << "Writing AST to \"";
    std::cout << destFilename;
    std::cout << "\"" << std::endl;
    std::ofstream destFile (destFilename);
    destFile << output; 

    //=== Code Generation ================================================
    // coming soon!
    
    // clean up resources
    delete ast; 

    return EXIT_SUCCESS;
}

//========================================================================
