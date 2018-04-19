#include "symboltable.h"
#include <string.h>
#include <stdbool.h>

int hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    // printf("Buckets: %d\t%lu\n", ht_buckets,hash);
    return hash % ht_buckets;
}

bool is_stmt(astNode *node)
{
	if(node==NULL)
		return false;
	if(node->isT==0)
	{
		if(node->symbol==6 || node->symbol==16 || node->symbol==17 || node->symbol==8 || node->symbol==11 || node->symbol==5)
			return true;
	}
	else if(node->isT==1)
	{
		if(node->symbol==20 || node->symbol==21)
			return true;
	}
	return false;
}

bool search_hashtable(SymbolTable *st, unsigned char *str)
{
	STEntry **hashtable = st->hashtable;
	int key = hash(str);
	STEntry *itr = hashtable[key];
	while(itr)
	{
		if(strcmp(str,itr->token->lexeme)==0)
			return true;
		itr = itr->next;
	}
	if(st->parent!=NULL)
		return search_hashtable(st->parent, str);
	else
		return false;
}

void search_hashtable_AST(SymbolTable *st, astNode *root)
{
	if(root==NULL)
		return;
	if(root->children!=NULL){
		// printf("down\n");
		search_hashtable_AST(st, root->children);
		// printf("up\n");
	}
	if(root->isT==1 && (root->symbol==8 || root->symbol==9))
	{
		bool exist = search_hashtable(st, root->token->lexeme);
		if(!exist)	// ERROR
			printf("Line:%d: ERROR: symbol '%s' undefined.\n",root->token->line, root->token->lexeme);
		// else
			// printf("symbol %s defined in current scope.\n", root->token->lexeme);
	}
	root = root->children;
	// printf("down\n");
	while(root!=NULL && root->siblings!=NULL)
	{
		// printf("->\n");
		search_hashtable_AST(st, root->siblings);
		root = root->siblings;
	}
	// printf("up\n");
	return;
}

SymbolTable *initializeSymbolTable(astNode *func, /*astNode *inparams, astNode *outparams,*/ SymbolTable *parent)
{
	SymbolTable *st = (SymbolTable *)malloc(sizeof(SymbolTable));
	st->parent = parent;
	st->scope = (char *)malloc(20*sizeof(char));
	st->table_offset = 0;
	// initialize hashtable
	st->hashtable = (STEntry **)malloc(ht_buckets*sizeof(STEntry *));
	for(int i=0; i<ht_buckets; i++)
	{
		st->hashtable[i] = NULL;
	}
	// initialize input & output parameters and scope
	if(func->isT==0 && func->symbol==0)	// MAIN Function
	{
		st->inparams = NULL;
		st->outparams = NULL;
		strcpy(st->scope,"_main");
	}
	else
	{
		st->outparams = func->children->children;
		st->inparams = func->children->siblings->siblings->children;
		strcpy(st->scope,func->children->siblings->token->lexeme);
	}
	return st;
}

STEntry *createSTEntry(astNode *var, astNode *type)
{
	// printf("CREATING ENTRY FOR: %s %s [ast:%d token:%d]\n", terms[var->symbol], var->token->lexeme, var->symbol, var->token->id);
	STEntry *entry = (STEntry *)malloc(sizeof(STEntry));
	entry->token = var->token;
	entry->width = 0;	// DO : Change
	entry->offset = 0;	// DO : Change
	entry->type = type->symbol;
	entry->fndef = NULL;	// DO : Change
	entry->next = NULL;
	return entry;
}

STEntry *insertHashTable(STEntry **ht, astNode *var, astNode *type)
{
	int key = hash(var->token->lexeme);
	// printf("%s : %d\n", var->token->lexeme, key);
	STEntry *bucket = ht[key];
	if(ht[key]==NULL)
		ht[key] = createSTEntry(var, type);
	else
	{
		STEntry *bitr = ht[key];
		while(bitr->next!=NULL)
			bitr = bitr->next;
		bitr->next = createSTEntry(var, type);
	}
	// printf("MID EXPORE BUCKET\n");
	// STEntry *itr = ht[key];
	// while(itr)
	// {
	// 	printf("%s: %s;\t", terms[itr->token->id], itr->token->lexeme);
	// 	itr = itr->next;
	// }
}

void insertSymbolTable(SymbolTable *st, astNode *stmt)
{
	if(stmt->isT==0 && stmt->symbol==6)	// declarationStmt
	{
		// printf("declarationStmt\n");
		astNode *type = stmt->children;
		astNode *vars = stmt->children->siblings;
		while(vars!=NULL)
		{
			// printASTNode(vars);
			if(vars->isT==1 && (vars->symbol==8 || vars->symbol==9))
				insertHashTable(st->hashtable,vars, type);
			vars = vars->siblings;
		}
	}
	else if(stmt->isT==0 && (stmt->symbol==16 || stmt->symbol==17))	// assignmentStmt
	{
		// printf("assignmentStmt\n");
		search_hashtable_AST(st, stmt);
	}
	else if(stmt->isT==0 && stmt->symbol==8)	//conditionalStmt
	{
		// printf("conditionalStmt\n");
		search_hashtable_AST(st, stmt->children);	// booleanExpression
		astNode *itr = stmt->children->siblings;
		while(itr!=NULL && is_stmt(itr))		// CONFIRM : currently adding all declaration inside if to the parent function.
		{
			// printASTNode(itr);
			insertSymbolTable(st, itr);
			itr = itr->siblings;
		}
		if(itr->isT==0 && itr->symbol==29)	// <add_else>
		{
			itr = itr->children;	// statements under ELSE clause
			while(itr!=NULL)
			{
				insertSymbolTable(st, itr);
				itr = itr->siblings;
			}
		}

	}
	else if(stmt->isT==1 && (stmt->symbol==20 || stmt->symbol==21))	// ioStmt
	{
		// printf("ioStmt\n");
		bool exist = search_hashtable(st, stmt->children->token->lexeme);
		if(!exist)	// ERROR
			printf("Line:%d: ERROR: symbol '%s' undefined.\n",stmt->children->token->line, stmt->children->token->lexeme);
		// else
			// printf("symbol %s defined in current scope.\n", stmt->children->token->lexeme);
	}
	else if(stmt->isT==0 && stmt->symbol==11)	//funcCallStmt
	{
		// printf("funcCallStmt\n");
		// Checking for Function Definition
		bool exist = search_hashtable(st, stmt->children->token->lexeme);
		if(!exist)	// ERROR
			printf("Line:%d: ERROR: function '%s' undefined.\n",stmt->children->token->line, stmt->children->token->lexeme);
		// else
			// printf("Function %s defined in current scope.\n", stmt->children->token->lexeme);
		// Checking Parameters
		// printf("Checking Parameters...\n");
		astNode *pitr = stmt->children->siblings;
		while(pitr!=NULL)
		{
			search_hashtable_AST(st, pitr);
			pitr = pitr->siblings;
		}
		// printf("Done checking parameters\n");

	}
	else if(stmt->isT==0 && stmt->symbol==5)	//funcDef
	{
		// printf("funcDef\n");
		astNode *functype = (astNode *)malloc(sizeof(astNode));
		functype->symbol = FUNCTION;
		STEntry *func = insertHashTable(st->hashtable,stmt->children->siblings,functype);
		free(functype);
		func->fndef = createSymbolTable(stmt,st);
		printSymbolTable_Node(func->fndef);
	}
	else
	{
		printf("UNKNOWN KIND OF STMT:\t");
		printASTNode(stmt);
	}
}

SymbolTable *createSymbolTable(astNode *func, SymbolTable *parent)
{
	SymbolTable *st = initializeSymbolTable(func, parent);
	// Handle Parameters
	astNode *params = st->inparams;
	while(params)
	{
// STEntry *insertHashTable(STEntry **ht, astNode *var, astNode *type)
		insertHashTable(st->hashtable, params->children->siblings, params->children);
		params = params->siblings;
	}
	params = st->outparams;
	while(params)
	{
		insertHashTable(st->hashtable, params->children->siblings, params->children);
		params = params->siblings;
	}
	// Handle Statements
	astNode *itrstmt;
	if(func->isT==0 && func->symbol==0)
		itrstmt = func->children;
	else	// functions apart from _main
		itrstmt = func->children->siblings->siblings->siblings;
	while(itrstmt!=NULL)
	{
		insertSymbolTable(st,itrstmt);
		itrstmt = itrstmt->siblings;
	}
	// printf("HASHTABLE\n");
	// printHashTable(st->hashtable);
	return st;
}

void printSymbolTable_Node(SymbolTable *st)
{
	printf("************************************************************\n");
	printf("SCOPE: %s\n", st->scope);
	printf("INPARAMS:\t");
	if(st->inparams)
	{
		astNode *itr = st->inparams;
		while(itr)
		{
			printf("%s : %s;\t", terms[itr->children->symbol], itr->children->siblings->token->lexeme);
			itr = itr->siblings;
		}
		printf("\n");
	}
	else
		printf("NULL\n");
	printf("OUTPARAMS:\t");
	if(st->inparams)
	{
		astNode *itr = st->outparams;
		while(itr)
		{
			printf("%s : %s;\t", terms[itr->children->symbol], itr->children->siblings->token->lexeme);
			itr = itr->siblings;
		}
		printf("\n");
	}
	else
		printf("NULL\n");
	printf("PARENT: ");
	if(st->parent)
		printf("%s\n", st->parent->scope);
	else
		printf("NULL\n");
	printf("HASHTABLE:\n");
	printHashTable(st->hashtable);
	printf("************************************************************\n");
}

void printHashTable(STEntry **hashtable)
{
	for(int i = 0; i < ht_buckets; i++)
	{
		if(hashtable[i]==NULL)
			continue;
		printf("KEY: %d\n", i);
		STEntry *itr = hashtable[i];
		while(itr)
		{
			printf("%s (%s): %s;\t", terms[itr->type] ,terms[itr->token->id], itr->token->lexeme);
			itr = itr->next;
		}
		printf("\n");
	}
}

