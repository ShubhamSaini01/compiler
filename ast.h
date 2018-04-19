#include "parser.h"
#include "astDef.h"
astNode *create_astNode(PTNode *);
astNode *ast_generate(PTNode *);
void statusNode(PTNode *);
void printAST(astNode *);
void printASTNode(astNode *);
astNode *joinroot_astNodes(astNode *, astNode *arr[], int);
astNode *join_astNodes(astNode *arr[], int);


