/*
	Aayushmaan Jain
	2015A7PSS0043P
	lexerDef.h
*/

#define BUF_SIZE 4096
enum token{eof,ASSIGNOP,COMMENT,FUNID,ID,NUM,RNUM,STR,END,INT,REAL,STRING,MATRIX,MAIN,SQO,SQC,OP,CL,SEMICOLON,COMMA,IF,ELSE,ENDIF,
READ,PRINT,FUNCTION,PLUS,MINUS,MUL,DIV,SIZE,AND,OR,NOT,LT,LE,EQ,GT,GE,NE};
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