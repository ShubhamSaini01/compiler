#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
// #include "tree.h"
#include "parserDef.h"

typedef struct Stack
{
	int size;
	int capacity;
	PTNode **head;
}Stack;

Stack *create_stack(int);
bool isEmpty(Stack *);
void push(PTNode *,Stack **);
PTNode* pop(Stack *);
PTNode* getTop(Stack *);
void printStack(Stack *);
