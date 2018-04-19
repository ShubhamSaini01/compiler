#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lexerDef.h"
#include "parserDef.h"

typedef struct parsetreenode
{
	tokenInfo *token;
	bool isLeaf;
	rhsnode *gmr;
	struct parsetreenode *parent;
	struct parsetreenode *children;
	struct parsetreenode *siblings;
}PTNode;

PTNode *create_PTNode(tokenInfo *token, bool isLeaf, rhsnode *gmr)
{
	PTNode *node = (PTNode *)malloc(sizeof(PTNode));
	node->token = token;
	node->isLeaf = isLeaf;
	node->gmr = gmr;
	node->parent = NULL;
	node->children = NULL;
	node->siblings = NULL;
	return node;
}
// int main()
// {
// 	PTNode *node = create_PTNode(NULL,false,NULL);
// 	printf("%d\n", node->isLeaf);
// 	return 0;
// }