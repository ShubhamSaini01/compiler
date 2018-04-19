#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "lexerDef.h"
#include "parserDef.h"

typedef struct parsetreenode
{
	tokenInfo *token;
	int isLeaf;
	rhsnode *gmr;
	struct parsetreenode *parent;
	struct parsetreenode *children;
	struct parsetreenode *siblings;
}PTNode;

PTNode *create_PTNode(tokenInfo *, bool , rhsnode *);
