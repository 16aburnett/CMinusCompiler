########################################################
# Makefile for CSCI 435 
# Author: Dr. Zoppetti, Amy Burnett, Lauren Deaver
########################################################

########################################################
# Variable definitions
########################################################
# C++ compiler
CXX := g++
#CXX := clang++

# Include directories, prefaced with "-I"
INCDIRS  := 

# C++ compiler flags
# Use the first for debugging, the second for release
CXXFLAGS := -g -Wall -std=c++17 $(INCDIRS)
#CXXFLAGS := -O3 -Wall -std=c++17 $(INCDIRS)

# Linker. For C++ should be $(CXX).
LINK := $(CXX)

# Linker flags. Usually none.
LDFLAGS := 

# Library paths, prefaced with "-L". Usually none.
LDPATHS := 

# Executable name. 
EXEC := CMinus

# Libraries used, prefaced with "-l".
LDLIBS := 

#############################################################
# Rules
#   Rules have the form
#   target : prerequisites
#         recipe
#############################################################

$(EXEC) : CMinusAst.o CMinus.o Lexer.o Parser.o PrintVisitor.o SymbolTable.o SymbolTableVisitor.o
	$(LINK) $(LDFLAGS) $^ -o $@ $(LDLIBS)

%.o : %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

#############################################################

.PHONY : clean
clean :
	$(RM) $(EXEC) a.out core
	$(RM) *.o *.d *~

#############################################################