/*
	Aayushmaan Jain
	2015A7PSS0043P
	lexerDef.h
*/
// #ifndef lexerstructs
#define BUF_SIZE 4096
// enum token{EPSILON,MAIN,SQO,SQC,END,SEMICOLON,ID,ASSIGNOP,FUNCTION,FUNID,INT,REAL,STRING,MATRIX,COMMA,SIZE,IF,ENDIF,ELSE,READ,OP,CL,PRINT,PLUS,MINUS,MUL,DIV,NOT,NUM,RNUM,STR,AND,OR,LT,LE,EQ,GT,GE,NE,COMMENT,eof,ERROR};
enum token{EPSILON,MAIN,SQO,SQC,END,SEMICOLON,FUNCTION,ASSIGNOP,FUNID,ID,INT,REAL,STRING,MATRIX,COMMA,OP,CL,IF,ENDIF,ELSE,READ,PRINT,PLUS,MINUS,MUL,DIV,NOT,NUM,RNUM,STR,SIZE,AND,OR,LT,LE,EQ,GT,GE,NE,COMMENT,eof,ERROR};
struct tokenInfo
{
	//tagunion tag;
	int id;
	int line;
	char* lexeme;
	/*union
	{
		char* lexeme;
		int* num;	// stores numerical value if number/real.
		float* rnum;
	}*/
};
typedef struct tokenInfo tokenInfo;

struct lookahead
{
	int buf;
	int index;
};
typedef struct lookahead lookahead;

int linenum;// = 1;
int state;
char **buffers;
// buffer-pair lookahead
lookahead peek;
//peek.buf = 0;
//peek.index = 0;

// #endif
