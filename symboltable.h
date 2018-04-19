#ifndef SYMBOOLTABLE_H_INCLUDED
#define SYMBOOLTABLE_H_INCLUDED

#include <stdbool.h>
#include "ast.h"
#include "symboltableDef.h"

int hash(unsigned char *);
SymbolTable *initializeSymbolTable(astNode *,/* astNode *, astNode *,*/ SymbolTable *);
SymbolTable *createSymbolTable(astNode *, SymbolTable *);
STEntry *createSTEntry(astNode *, astNode *);
void insertSymbolTable(SymbolTable *, astNode *);
void insertHashTable(STEntry **, astNode *, astNode *);
void printSymbolTable_Node(SymbolTable *);
void printHashTable(STEntry **);
bool search_hashtable(SymbolTable *, unsigned char *);
void search_hashtable_AST(SymbolTable *, astNode *);


#endif