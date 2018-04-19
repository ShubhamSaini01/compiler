
typedef struct astNode
{
	int isT;
	int symbol;
	// unsigned int tag;	// tag = 0 ==> no data.
	// union data
	// {
	// 	int num;		// tag = 1
	// 	float rnum;		// tag = 2
	// 	char str[20];	// tag = 3
	// 	char id[20];	// tag = 4
	// 	char funid[20];	// tag = 5
	// };
	tokenInfo *token;
	struct astNode *parent;
	struct astNode *children;
	struct astNode *siblings;
}astNode;