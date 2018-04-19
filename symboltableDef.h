#ifndef SYMBOLTABLEDEF_H_INCLUDED
#define SYMBOLTABLEDEF_H_INCLUDED

#define ht_buckets 100

// typedef struct
// {
// 	int width;
// 	int offset;
// 	int type;
// }ID;
// typedef struct
// {
// 	SymbolTable *st;
// 	astNode *inparams;
// 	astNode *outparams;
// }FUN;
struct STEntry;
struct SymbolTable;

typedef struct STEntry
{
	tokenInfo *token;
	// union
	// {
	// 	ID id;
	// 	FUN fun;
	// }
	// ID
	int width;
	int offset;
	int type;
	// FUNID
	struct SymbolTable *fndef;
	// linking
	struct STEntry *next;
}STEntry;

typedef struct  SymbolTable
{
	struct SymbolTable *parent;
	char *scope;
	astNode *inparams;
	astNode *outparams;
	STEntry **hashtable;
	int table_offset;
}SymbolTable;

// track symboltable errors like redeclarations
char symboltable_errors[500][500];
int error_count;
int symbolerror;

#endif