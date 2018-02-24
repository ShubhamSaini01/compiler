/*
	Aayushmaan Jain
	2015A7PSS0043P
	lexer.c
*/

/*	DOUBTS:
	What to do when we have 'abd45'?
*/

//#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexerDef.h"

// {
// 	char index[][] = ["eof","ASSIGNOP","COMMENT","FUNID","ID","NUM","RNUM","STR","END","INT","REAL","STRING","MATRIX","MAIN","SQO","SQC","OP","CL","SEMICOLON","COMMA","IF","ELSE","ENDIF",
// "READ","PRINT","FUNCTION","PLUS","MINUS","MUL","DIV","SIZE","AND","OR","NOT","LT","LE","EQ","GT","GE","NE"]
// }
char **createBuffers(int num, int size)
{
	char **buffers = (char **)malloc(num*sizeof(char*));
	// check after all malloc calls;
	for(int i=0;i<num;i++)
	{
	 	buffers[i] = (char *)malloc((size)*sizeof(char));
		buffers[i][size-1] = EOF;
	}
	return buffers;
}

FILE *getStream(FILE *fp, char *buffer, int buffersize)
{
	int count;
	if (!feof(fp))
		count = fread(buffer, 1, buffersize-1,fp);
	// printf("%d\n", count);
	// printf("BUFFER:%sEND\n",buffer);
	if(feof(fp)){
		// printf("\n\nENDOFFILE!\t%d\n\n",count);
		buffer[count] = EOF;
	}
	return fp;
}

tokenInfo setToken(int id, char *lexeme, int line)
{
	tokenInfo token;
	token.lexeme = (char *)malloc(BUF_SIZE*sizeof(char));		// max length = 20 (most probably). BUF_SIZE just to be on safer side.
	token.id = id;
	strcpy(token.lexeme, lexeme);
	token.line = line;
	//token.value = value;
	return token;
}

int checkReserveWord(char *lexeme)
{
	if(!strcmp(lexeme,"end"))
		return END;
	else if(!strcmp(lexeme,"int"))
		return INT;
	else if(!strcmp(lexeme,"real"))
		return REAL;
	else if(!strcmp(lexeme,"string"))
		return STRING;
	else if(!strcmp(lexeme,"matrix"))
		return MATRIX;
	else if(!strcmp(lexeme,"if"))
		return IF;
	else if(!strcmp(lexeme,"else"))
		return ELSE;
	else if(!strcmp(lexeme,"endif"))
		return ENDIF;
	else if(!strcmp(lexeme,"read"))
		return READ;
	else if(!strcmp(lexeme,"print"))
		return PRINT;
	else if(!strcmp(lexeme,"function"))
		return FUNCTION;
	else
		return -1;
}

tokenInfo getNextToken(FILE *fp,int buffernum, int buffersize)
{
	state = 1;
	int flag = 0;
	int i=0;
	char *lexeme = (char *)malloc(BUF_SIZE*sizeof(char));		// max length = 20 (most probably). BUF_SIZE just to be on safer side.
	tokenInfo token;
	char ch;
	// NOTE: error possible if lexeme length greater than memory allocated here. Worst case: add check.
	while(1)	// !flag
	{
		ch = buffers[peek.buf][peek.index];
		// printf("char read: %c\n",ch);
		if(ch==EOF)
		{
			if(peek.index==(buffersize-1))		// sentinel character
				{
					peek.buf = (peek.buf + 1) % buffernum;
					fp = getStream(fp,buffers[peek.buf],buffersize);
					peek.index=0;
					continue;
				}
			else	// eof within buffer marks end of input
				{
					//flag = 1;	// NOTE: alt send some token to mark end of input.
					// printf("END OF FILE\n");
					return setToken(eof,"$",linenum);
				}	
		}
				
		switch(state)
		{
			case 1:	switch(buffers[peek.buf][peek.index])		//DO: change to ch
					{
						case '\n':	linenum++;	// fall through
									//printf("newline %d\n",linenum);
						case ' ':	//printf("space %d\n",linenum);
						case '\t':	//printf("tab %d\n", linenum);
						case '\r':	state=1;	// not necessary to write
									peek.index++;
									//printf("carriage return %d\n", linenum); 
									break;
						case '[':	state = 2;
									peek.index++;
									return setToken(SQO,"[",linenum);
						case ']':	state = 3;
									peek.index++;
									return setToken(SQC,"]",linenum);
						case '(':	state = 4;
									peek.index++;
									return setToken(OP,"(",linenum);
						case ')':	state = 5;
									peek.index++;
									return setToken(CL,")",linenum);
						case ';':	state = 6;
									peek.index++;
									return setToken(SEMICOLON,";",linenum);
						case ',':	state = 7;
									peek.index++;
									return setToken(COMMA,",",linenum);
						case '+':	state = 8;
									peek.index++;
									return setToken(PLUS,"+",linenum);
						case '-':	state = 9;
									peek.index++;
									return setToken(MINUS,"-",linenum);
						case '*':	state = 10;
									peek.index++;
									return setToken(MUL,"*",linenum);
						case '/':	state = 11;
									peek.index++;
									return setToken(DIV,"/",linenum);
						case '@':	state = 12;
									peek.index++;
									return setToken(SIZE,"@",linenum);
						case '_':	state = 13;
									lexeme[i++] = ch;
									peek.index++;
									break;
						case 'a':	// fall through
						case 'b':
						case 'c':
						case 'd':
						case 'e':
						case 'f':
						case 'g':
						case 'h':
						case 'i':
						case 'j':
						case 'k':
						case 'l':
						case 'm':
						case 'n':
						case 'o':
						case 'p':
						case 'q':
						case 'r':
						case 's':
						case 't':
						case 'u':
						case 'v':
						case 'w':
						case 'x':
						case 'y':
						case 'z':
						case 'A':
						case 'B':
						case 'C':
						case 'D':
						case 'E':
						case 'F':
						case 'G':
						case 'H':
						case 'I':
						case 'J':
						case 'K':
						case 'L':
						case 'M':
						case 'N':
						case 'O':
						case 'P':
						case 'Q':
						case 'R':
						case 'S':
						case 'T':
						case 'U':
						case 'V':
						case 'W':
						case 'X':
						case 'Y':
						case 'Z':	state = 15;
									lexeme[i++] = ch;
									peek.index++;
									break;
						case '0':
						case '1':
						case '2':
						case '3':
						case '4':
						case '5':
						case '6':
						case '7':
						case '8':
						case '9':	state = 17;
									lexeme[i++] = ch;
									peek.index++;
									break;
						case '"':	state = 21;
									lexeme[i++] = ch;
									peek.index++;
									break;
						case '.':	state = 24;
									peek.index++;
									break;
						case '<':	state = 36;
									peek.index++;
									break;
						case '>':	state = 38;
									peek.index++;
									break;
						case '=':	state = 40;
									peek.index++;
									break;
						case '#':	state = 44;
									peek.index++;
									break;
						default:	// ERROR handling
									// printf("Error: character not recognised\n");
									return setToken(ERROR,"Error: character not recognised\n",linenum);
					}
					break;
			case 13:
					if((65<=ch && ch>=90) || (97<=ch && ch>=122))
					{
						state = 14;
						lexeme[i++] = ch;
						peek.index++;
					}
					else	
					{
						//ERROR handling
						return setToken(ERROR,"Error: \n",linenum);
					}
					break;
			case 14:
					while((ch>=65 && ch<=90) || (ch>=97 && ch<=122) || (ch>=48 && ch<=57))
					{
						lexeme[i++] = ch;
						peek.index++;
						if(peek.index==(buffersize-1))		// sentinel character
						{
							peek.buf = (peek.buf + 1) % buffernum;
							fp = getStream(fp,buffers[peek.buf],buffersize);
							peek.index=0;
						}
						ch = buffers[peek.buf][peek.index];
						// printf("char read: %c\n",ch);

					}
					//check if function is _main
					if(!strcmp(lexeme,"_main"))
						return setToken(MAIN,lexeme,linenum);
					// printf("%s\n",lexeme);
					// else
					// printf("%s\n", lexeme);
					return setToken(FUNID,lexeme,linenum);
					break;
			case 15:
					while((ch>=65 && ch<=90) || (ch>=97 && ch<=122))
					{
						lexeme[i++] = ch;
						peek.index++;
						if(peek.index==(buffersize-1))		// sentinel character
						{
							peek.buf = (peek.buf + 1) % buffernum;
							fp = getStream(fp,buffers[peek.buf],buffersize);
							peek.index=0;
						}
						ch = buffers[peek.buf][peek.index];
						// printf("char read: %c\n",ch);

					}
					if(48<=ch && ch>=57)
					{
						state = 16;
						lexeme[i++] = ch;
						peek.index++;
						return setToken(ID,lexeme,linenum);
					}
					else
					{
						// check for keyword
						int reserve = checkReserveWord(lexeme);
						if(reserve!=-1)
							return setToken(reserve,lexeme,linenum);
						else
							return setToken(ID,lexeme,linenum);
					}
					break;
			case 17:
					while(ch>=48 && ch<=57)
					{
						lexeme[i++] = ch;
						peek.index++;
						if(peek.index==(buffersize-1))		// sentinel character
						{
							peek.buf = (peek.buf + 1) % buffernum;
							fp = getStream(fp,buffers[peek.buf],buffersize);
							peek.index=0;
						}
						ch = buffers[peek.buf][peek.index];
						// printf("char read: %c\n",ch);		
					}
					if(ch=='.')
					{
						state = 18;
						lexeme[i++] = ch;
						peek.index++;
					}
					// DOUBTS: How to tokenize 23a? '23' and 'a' or error?
					else
					{
						return setToken(NUM,lexeme,linenum);
					}
					break;
			case 18:
					if(48<=ch && ch>=57)
					{
						state = 19;
						lexeme[i++] = ch;
						peek.index++;
					}
					else
					{
						// DO: ERROR
						return setToken(ERROR,"Error: Real Number formatting not followed\n",linenum);

					}
					break;
			case 19:
					if(48<=ch && ch>=57)
					{
						state = 20;
						lexeme[i++] = ch;
						peek.index++;
						return setToken(RNUM,lexeme,linenum);
					}
					else
					{
						// DO: ERROR
						// printf("Error: Real Number formatting not followed\n");
						return setToken(ERROR,"Error: Real Number formatting not followed\n",linenum);
					}
					break;
			case 21:
					if(97<=ch && ch>=122)
					{
						state = 22;
						lexeme[i++] = ch;
						peek.index++;
					}
					else
					{
						// DO: ERROR
						return setToken(ERROR,"Error:  String formatting not followed\n",linenum);
					}
					break;
			case 22:
					while(ch>=97 && ch<=122)
					{
						lexeme[i++] = ch;
						peek.index++;
						if(peek.index==(buffersize-1))		// sentinel character
						{
							peek.buf = (peek.buf + 1) % buffernum;
							fp = getStream(fp,buffers[peek.buf],buffersize);
							peek.index=0;
						}
						ch = buffers[peek.buf][peek.index];
						// printf("char read: %c\n",ch);

					}
					if(ch=='"')
					{
						state = 23;
						lexeme[i++] = ch;
						peek.index++;
						return setToken(STRING,lexeme,linenum);		// DOUBTS: do we need to include '"' and '"' in the lexeme?
					}
					else
					{
						// DO: ERROR
						// printf("Error: String formatting not followed\n");
						return setToken(ERROR,"Error: String formatting not followed\n",linenum);
					}
					break;
			case 24:
					switch(ch)
					{
						case 'a':	state = 25;
						case 'o':	state = 29;
						case 'n':	state = 32;
									peek.index++;
									break;
						default:	// ERROR
									// printf("Error: Unexpected character received\n");
									return setToken(ERROR,"Error: Unexpected character received\n",linenum);
									break;
					}
					break;
			case 25:
					if(ch=='n')
					{
						state = 26;
						peek.index++;
					}
					else
					{
						// ERROR
						// printf("Error: Unexpected character received\n");
						return setToken(ERROR,"Error: Unexpected character received\n",linenum);
					}
					break;
			case 26:
					if(ch=='d')
					{
						state = 27;
						peek.index++;
					}
					else
					{
						// ERROR
						// printf("Error: Unexpected character received\n");
						return setToken(ERROR,"Error: Unexpected character received\n",linenum);
					}
					break;
			case 27:
					if(ch=='.')
					{
						state = 26;
						peek.index++;
						return setToken(AND,".and.",linenum);
					}
					else
					{
						// ERROR
						// printf("Error: Unexpected character received\n");
						return setToken(ERROR,"Error: Unexpected character received\n",linenum);
					}
					break;
			case 29:
					if(ch=='r')
					{
						state = 30;
						peek.index++;
					}
					else
					{
						// ERROR
						// printf("Error: Unexpected character received\n");
						return setToken(ERROR,"Error: Unexpected character received\n",linenum);
					}
					break;
			case 30:
					if(ch=='.')
					{
						state = 31;
						peek.index++;
						return setToken(OR,".or.",linenum);
					}
					else
					{
						// ERROR
						// printf("Error: Unexpected character received %d\n",state);
						return setToken(ERROR,"Error: Unexpected character received\n",linenum);
					}
					break;
			case 32:
					if(ch=='o')
					{
						state = 33;
						peek.index++;
					}
					else
					{
						// ERROR
						// printf("Error: Unexpected character received %d\n",state);
						return setToken(ERROR,"Error: Unexpected character received\n",linenum);
					}
					break;
			case 33:
					if(ch=='t')
					{
						state = 34;
						peek.index++;
					}
					else
					{
						// ERROR
						// printf("Error: Unexpected character received %d\n",state);
						return setToken(ERROR,"Error: Unexpected character received\n",linenum);
					}
					break;
			case 34:
					if(ch=='.')
					{
						state = 35;
						peek.index++;
						return setToken(NOT,".not.",linenum);
					}
					else
					{
						// ERROR
						// printf("Error: Unexpected character received %d\n",state);
						return setToken(ERROR,"Error: Unexpected character received\n",linenum);
					}
					break;
			case 36:
					if(ch=='=')
					{
						state = 37;
						peek.index++;
						return setToken(LE,"<=",linenum);
					}
					else
					{
						return setToken(LT,"<",linenum);
					}
					break;
			case 38:
					if(ch=='=')
					{
						state = 39;
						peek.index++;
						return setToken(GE,">=",linenum);
					}
					else
					{
						return setToken(GT,">",linenum);
					}
					break;
			case 40:
					if(ch=='=')
					{
						state = 43;
						peek.index++;
						return setToken(EQ,"==",linenum);
					}
					else if(ch=='/')
					{
						state = 41;
						peek.index++;
					}
					else
					{
						return setToken(ASSIGNOP,"=",linenum);
					}
					break;
			case 41:
					if(ch=='=')
					{
						state = 42;
						peek.index++;
						return setToken(NE,"=/=",linenum);
					}
					else
					{
						// ERROR
						// printf("Error: Unexpected character received %d\n",state);
						return setToken(ERROR,"Error: Unexpected character received\n",linenum);
					}
					break;
			case 44:	// DOUBTS: Necessary that the comment have atleast one character?
					if(ch!='\n')
					{
						// lexeme[i++] = ch;	
						state = 45;
						peek.index++;
					}
					else
					{
						// ERROR
						// printf("Error: Blank Comment\n");
						return setToken(ERROR,"Error: Blank Comment\n",linenum);
					}
					break;
			case 45:
					while(ch!='\n')
					{
						// lexeme[i++] = ch;
						peek.index++;
						if(peek.index==(buffersize-1))		// sentinel character
						{
							peek.buf = (peek.buf + 1) % buffernum;
							fp = getStream(fp,buffers[peek.buf],buffersize);
							peek.index=0;
						}
						ch = buffers[peek.buf][peek.index];
						// printf("char read: %c\n",ch);
					}
					// ch=='\n'
					state = 46;
					return setToken(COMMENT,"COMMENT",linenum);	//DOUBTS: Do we have to store the comments?
					break;
			default:
					//ERROR
					// printf("Error: Invalid state exception\n");
					return setToken(ERROR,"Error: Invalid state exception\n",linenum);
					break;
		}
	}

}

int main()
{
	int buffersize = 10;
	buffers =  createBuffers(2,buffersize);
	FILE *fp = fopen("hello.txt","r");
	peek.buf = 0;
	peek.index = 0;
	linenum = 1;
	fp = getStream(fp,buffers[0],buffersize);
	/*// Print buffers
	for(int i=0;i<1;i++)
	{
		//printf("%d\n",i+1);
		for(int j=0;j<buffersize;j++)
			printf("%c ",buffers[i][j]);
		printf("\n");
	}*/
	tokenInfo token;
	do
	{
		token = getNextToken(fp,2,buffersize);
		printf("%d %s %d\n",token.id,token.lexeme,token.line); 
	} while (token.id!=eof); 	
	
	return 0;
}