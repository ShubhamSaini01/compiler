#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
// #include "tree.h"
// #include "lexerDef.h"
// #include "stack.h"
#include "parserDef.h"

// char terminals[39][30] = {"EPSILON","MAIN","SQO","SQC","END","SEMICOLON","ID","ASSIGNOP","FUNCTION","FUNID","INT","REAL","STRING","MATRIX","COMMA","SIZE","IF","ENDIF","ELSE","READ","OP","CL","PRINT","PLUS","MINUS","MUL","DIV","NOT","NUM","RNUM","STR","AND","OR","LT","LE","EQ","GT","GE","NE","COMMENT","eof","ERROR"};
// char nonterminals[44][30] = 
extern char nonterms[TOTAL_NT][SYM_SIZE];
extern char terms[TOTAL_T][SYM_SIZE];
typedef struct Stack
{
	int size;
	int capacity;
	PTNode **head;
}Stack;

Stack *create_stack(int c)
{
	Stack *stack = (Stack *)malloc(sizeof(Stack));
	stack->size = 0;
	stack->capacity = c;
	stack->head = (PTNode **)calloc(c,sizeof(PTNode *));
	return stack;
}
bool isEmpty(Stack *stack)
{
	if(stack->size==0){
		printf("STACK EMPTY\n");
		return true;
	}
	return false;
}
void push(PTNode *data,Stack **stack)
{
	if((*stack)->size==((*stack)->capacity -1))
	{
		(*stack)->capacity *= 2;
		*stack = realloc(*stack,(*stack)->capacity);
	}
	// data->next = (*stack)->head;
	(*stack)->head[(*stack)->size++] = data;
	// (*stack)->size++;
}

/*PTNode* pop(Stack **stack)
{
	int temp = (*stack)->size;
	if(temp<=0)
		return NULL;
	// PTNode *temp = 
	// (*stack)->size--;
	// (*stack)->head = (*stack)->head->next;
	// printf("Size fromm inside: %d\n",(*stack)->size);
	// printf("tmep old: %d\n",temp);
	temp = temp -1;
	if (temp==6582527)	{
		temp = 2;
		(*stack)->capacity = 8;
	}
	(*stack)->size  = temp;
	// printf("tmep: %d\n",temp);
	return ((*stack)->head[temp]);
}*/
PTNode* pop(Stack *stack)
{
	int temp = stack->size;
	if(temp<=0)
		return NULL;
	// PTNode *temp = 
	// (*stack)->size--;
	// (*stack)->head = (*stack)->head->next;
	// printf("Size fromm inside: %d\n",(*stack)->size);
	// printf("tmep old: %d\n",temp);
	temp = temp -1;
	// if (temp==6582527)	{
	// 	temp = 2;
	// 	stack->capacity = 8;
	// }
	stack->size  = temp;
	// printf("tmep: %d\n",temp);
	return stack->head[temp];
}
PTNode* getTop(Stack *stack)
{
	if(isEmpty(stack)==1){
		return NULL;
	}
	return stack->head[(stack->size)-1];
}
void printStack(Stack *stack)
{
	int i = stack->size -1;
	// PTNode *temp = stack->head[(*stack)->size];
	while(i>=0)
	{
		if(stack->head[i]->isT==1)
			printf("Cap: %d Node: %d %d %s\n",stack->size,stack->head[i]->isLeaf,stack->head[i]->isT,terms[stack->head[i]->symbol]);
		else
			printf("cap: %d Node: %d %d %s\n",stack->size,stack->head[i]->isLeaf,stack->head[i]->isT,nonterms[stack->head[i]->symbol]);

		i--;
	}
	printf("\n\n");
}
// int main()
// {
// 	PTNode *temp = (PTNode *)malloc(sizeof(PTNode));
// 	temp->token = NULL;
// 	temp->isLeaf = 1;
// 	// temp->gmr = NULL;
// 	temp->parent = NULL;
// 	temp->children = NULL;
// 	temp->siblings = NULL;
// 	Stack *stack = create_stack(1);
// 	push(temp,&stack);
// 	PTNode *temp1 = (PTNode *)malloc(sizeof(PTNode));
// 	temp1->token = NULL;
// 	temp1->isLeaf = 2;
// 	// temp1->gmr = NULL;
// 	temp1->parent = NULL;
// 	temp1->children = NULL;
// 	temp1->siblings = NULL;
// 	push(temp1,&stack);
// 	printf("PRINTING STACK\n");
// 	printStack(stack);
// 	PTNode *ptr = getTop(stack);
// 	printf("getTop Node: %d\n",ptr->isLeaf);
// 	printf("%d %d\n", stack->size, stack->capacity);
// 	ptr = pop(&stack);
// 	printf("pop Node: %d\n",ptr->isLeaf);
// 	printf("%d %d\n", stack->size, stack->capacity);
// 	ptr = pop(&stack);
// 	printf("pop Node: %d\n",ptr->isLeaf);
// 	printf("%d %d\n", stack->size, stack->capacity);
// 	ptr = pop(&stack);
// 	printf("pop Node: %d\n",ptr->isLeaf);
// 	printf("%d %d\n", stack->size, stack->capacity);
// 	return 0;
// }
