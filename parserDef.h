
#include <stdbool.h>
#include "lexer.h"

#define SYM_SIZE 30
#define NUM_RULES 88	//86
#define TOTAL_NT 46		//44
#define TOTAL_T 39		//39

int ntcount;	// #nonterminals
int tcount;	// #terminals

char nonterms[TOTAL_NT][SYM_SIZE];
char terms[TOTAL_T][SYM_SIZE];

typedef struct rhsnode
{
	int isT;	// whether symbol is a terminal (1)/ nonterminal (0)/ null (-1)
	int symbol;
	struct rhsnode* next;
}rhsnode;
// typedef struct rhsnode rhsnode;

typedef struct lhsnode
{
	// int ruleno;
	int nonterm;
	rhsnode *rhs;
}lhsnode;

typedef struct ffnode
{
	int terminal;	// -1 for eps
	struct ffnode* next;
}ffnode;
typedef struct ffheader
{
	int symbol;
	ffnode *list;
}ffheader;

lhsnode grammar[NUM_RULES];
ffheader *firstsets;
ffheader *followsets;

int **parsetable;

typedef struct parsetreenode
{
	tokenInfo *token;
	bool isLeaf;
	int isT;
	int symbol;
	// rhsnode *gmr;
	int rule;
	struct parsetreenode *parent;
	struct parsetreenode *children;
	struct parsetreenode *siblings;
}PTNode;

// PTNode *root;
