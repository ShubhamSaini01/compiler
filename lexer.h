#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexerDef.h"
char** createBuffers(int,int);
FILE *getStream(FILE *, char *, int);
tokenInfo setToken(int, char *, int);
int checkReserveWord(char *);
tokenInfo getNextToken(FILE *, int, int);