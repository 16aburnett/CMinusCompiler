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

std::vector<Token> 
tokenize (FILE*);

//========================================================================
int
main (int argc, char* argv[])
{
    FILE* srcfile = stdin;
    std::string destFilename = "Default.ast";
    if (argc > 1)
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

    // lex and tokenize the source code 
    std::vector<Token> tokens = tokenize(srcfile);

    // attempt to parse the tokens 
    // and obtain an ast 
    Parser parser (tokens, false); 
    Node* ast = parser.parse ();

    // reaches here if it was a successful parse 
    std::cout << "Valid!" << std::endl;
    
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

    // clean up resources
    delete ast; 

    return EXIT_SUCCESS;
}

//========================================================================
std::vector<Token> 
tokenize (FILE* srcfile)
{

    Lexer lexer (srcfile);
    std::vector<Token> tokens; 

    // process each token until the EOF token is reached 
    do
    {
        // Ask lexer for next token 
        tokens.push_back(lexer.getToken());

    } while (tokens.back().type != END_OF_FILE);

    return tokens; 
}

//========================================================================
