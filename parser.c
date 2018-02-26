#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef
#include "lexer.h"
#include "lexerDef.h"

#define SYM_SIZE 30
#define NUM_RULES 86
#define TOTAL_NT 44
#define TOTAL_T 39

int ntcount = 0;	// #nonterminals
int tcount = 0;		// #terminals

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

int is_nonterm(char *str)
{
	// returns index in nonterminal table if nonterminal presemt, otherwise -1
	for(int i=0;i<ntcount;i++)
		if(strcmp(str,nonterms[i])==0)
			return i;
	return -1;
}

int is_term(char *str)
{
	// returns index in terminal table if terminal present, otherwise -1
	for(int i=0;i<tcount;i++)
		if(strcmp(str,terms[i])==0)
			return i;
	return -1;
}

int term_nonterm(char *str)
{
	/*return 1 if symbol is a terminal, 0 if nonterminal'*/
	if(strcmp(str,"EPSILON")==0)
		return -1;
	if(strlen(str)>2)
		if(str[0]=='<')
			return 0;
	return 1;
}

rhsnode *create_rhsnode(int tnt, int sym)
{
	rhsnode *temp = (rhsnode *)malloc(sizeof(rhsnode));
	temp->isT = tnt;
	temp->symbol = sym;
	temp->next = NULL;
	return temp;
}

int insertnodes(ffnode **dest, ffnode *src)
{
	// if(src==NULL)
	// 	return 0;
	int eps_flag = 0;
	int count = 0;
	ffnode *insert = src;
	ffnode *temp=NULL;
	while(insert!=NULL)
	{
		if(insert->terminal==0)
		{
			eps_flag = 1;
			insert = insert->next;
			continue;
		}
		else
		{
			temp = (ffnode *)malloc(sizeof(ffnode));
			temp->terminal = insert->terminal;
			temp->next = NULL;
		}
		int flag = 0;
		if(*dest == NULL)
		{
			*dest = temp;
			count++;
			insert = insert->next;
			continue;
		}
		ffnode *itr = *dest;
		ffnode *prev = NULL;
		while(itr!=NULL)
		{
			prev = itr;
			if(insert->terminal==itr->terminal)
			{
				flag = 1;
				break;
			}
			itr = itr->next;
		}
		if(flag==0)
		{
			if(*dest!=NULL)
			{
				prev->next = temp;
				count++;
			}
			// else
			// 	*dest = insert;
		}
		insert = insert->next;
	}
	// printf("eps: %d\n",eps_flag);
	return eps_flag;
}
int insert_eps(ffnode **dest, ffnode *src)
{
	int eps_flag = 0;
	int count = 0;
	ffnode *insert = src;
	ffnode *temp = NULL;
	while(insert!=NULL)
	{
		// if(insert->terminal==0)
		// {
		// 	eps_flag = 1;
		// 	insert = insert->next;
		// 	continue;
		// }
		// else
		// {
			temp = (ffnode *)malloc(sizeof(ffnode));
			temp->terminal = insert->terminal;
			temp->next = NULL;
		// }
		int flag = 0;
		if(*dest == NULL)
		{
			*dest = temp;
			count++;
			insert = insert->next;
			continue;
		}
		ffnode *itr = *dest;
		ffnode *prev = NULL;
		while(itr!=NULL)
		{
			prev = itr;
			if(insert->terminal==itr->terminal)
			{
				flag = 1;
				break;
			}
			itr = itr->next;
		}
		if(flag==0)
		{
			if(*dest!=NULL)
			{
				prev->next = temp;
				count++;
			}
			// else
			// 	*dest = insert;
		}
		insert = insert->next;
	}
	printf("eps1: %d\n",eps_flag);
	printf("other\n");
	return eps_flag;
}

void loadGrammar(char *filename)
{
	// lhs *grammar = (lhs *)malloc(NUM_RULES*sizeof(lhs));
	// add eps(NULL) to list of terminals
	strcpy(terms[0],"EPSILON");
	tcount++;

	FILE *fp = fopen(filename,"r");
	char left[SYM_SIZE];
	char right[150];
	ssize_t read;
	char *token;
	int tnt;
	int i_lhs,i_tok,i_gmr = 0;	// index for entering rules in the DS for storing grammar
	if(fp==NULL)
	{
		printf("Error opening grammar file\n");
		exit(EXIT_FAILURE);
	}
	while((read = fscanf(fp,"%s ===> %[^\r\n\t]",left,right))>0)
	{
		printf("%s -> %s\n", left, right);
		//  if NT not already encountered, add it to list of NTs
		i_lhs = is_nonterm(left);
		if(i_lhs<0)
		{
			strcpy(nonterms[ntcount],left);
			i_lhs = ntcount;
			ntcount++;
		}
		grammar[i_gmr].nonterm = i_lhs;

		rhsnode *head = create_rhsnode(-1,0);		// create dummy rhsnode
		rhsnode *itr = head;
		token = strtok(right," ");
		while(token!=NULL)
		{
			// if(strcmp(token,"eps")==0)
			// {
			// 	tnt = -1;
			// 	i_tok = 0;
			// }
			tnt = term_nonterm(token);
			printf("%s %d\t",token,tnt);
			if(tnt!=0)	// is a terminal or eps
			{
				i_tok = is_term(token);
				if(i_tok<0)
				{
					strcpy(terms[tcount],token);
					i_tok = tcount;
					tcount++;
				}
			}
			else	// nonterminal
			{
				i_tok = is_nonterm(token);
				if(i_tok<0)
				{
					strcpy(nonterms[ntcount],token);
					i_tok = ntcount;
					ntcount++;
				}
			}
			rhsnode *temp = create_rhsnode(tnt,i_tok);
			itr->next = temp;
			itr = itr->next;
			token = strtok(NULL," ");
		}
		// itr = head;
		grammar[i_gmr].rhs = head->next;
		free(head);
		i_gmr++;
		printf("\n");
	}
}

void getFirst(int nt, int **nt_first_done)	// may need to change definitions **
{
	int i_nt = nt + tcount;
	int eps_flag;
	firstsets[i_nt].symbol = nt;
	// printf("NT:%d\n", nt);

	for(int i=0;i<NUM_RULES;i++)
	{
		// printf("Rule:%d\n", i+1);
		if(grammar[i].nonterm==nt)
		{
			rhsnode *itr = grammar[i].rhs;
			while(itr!=NULL)
			{
				// printf("Rule: %d\tRHS: %d TNT: %d\n",i+1,itr->symbol,itr->isT);
				// eps_flag = -1;
				if(itr->isT==1)	// terminal or eps
				{
					// printf("Insert terminal: %d %d\n",i_nt,itr->symbol);
					insertnodes(&firstsets[i_nt].list,firstsets[itr->symbol].list);
					eps_flag = 0;
					break;
				}
				else if(itr->isT==-1)
				{
					// printf("Insert eps: %d %d\n",i_nt,itr->symbol);
					insertnodes(&firstsets[i_nt].list,firstsets[itr->symbol].list);
					eps_flag=1;
					break;
				}
				else	// nonterminals
				{
					// printf("Insert nonterminal: %d %d ist:%d\n",i_nt,itr->symbol, itr->isT);
					if((*nt_first_done)[itr->symbol]==0)
						getFirst(itr->symbol,nt_first_done);
					eps_flag = insertnodes(&firstsets[i_nt].list,firstsets[tcount + itr->symbol].list);
					if(eps_flag==0)
						break;
				}
				itr = itr->next;
			}
			if(eps_flag==1)
			{
				ffnode *eps = (ffnode *)malloc(sizeof(ffnode));
				eps->terminal = 0;
				eps->next = NULL;
				// printf("Insert eps: %d %d\n",i_nt,itr->symbol);
				insert_eps(&firstsets[i_nt].list,eps);	// add eps to first set of nonterminal	
				// printf("damn:%d\n", firstsets[0]->list->terminal);
				eps_flag = 0;
			}
		}
	// printf("INSIDE GETFIRST after\n");
	// for(int i=0;i<tcount+ntcount;i++)
	// {
	// 	printf("%d\t%d\t:\t",i,firstsets[i].symbol);
	// 	ffnode *itr = firstsets[i].list;
	// 	while(itr!=NULL)
	// 	{
	// 		printf("yayay %d\t",itr->terminal);
	// 		itr = itr->next;
	// 	}
	// 	printf("\n");
	// }
	}
	(*nt_first_done)[nt] = 1;
	
}

void compute_FirstSets()
{
	firstsets = (ffheader *)calloc((tcount+ntcount),sizeof(ffheader));
	// tracks nonterminals whose first sets have been computed. 
	int *nt_first_done = (int *)calloc(ntcount,sizeof(int));
	// first set of terminals are the terminals themselves
	for(int i=0;i<tcount;i++)
	{
		firstsets[i].symbol = i;
		ffnode *temp = (ffnode *)malloc(sizeof(ffnode));
		temp->terminal = i;
		temp->next = NULL;
		firstsets[i].list = temp;	
	}
	// for(int i=0;i<tcount+ntcount;i++)
	// {
	// 	printf("%d\t%d\t:\t",i,firstsets[i].symbol);
	// 	ffnode *itr = firstsets[i].list;
	// 	while(itr!=NULL)
	// 	{
	// 		printf("yayay %d\t",itr->terminal);
	// 		itr = itr->next;
	// 	}
	// 	printf("\n");
	// }
	// works fine till here
	for(int i=0;i<ntcount;i++)	// iterate over all nonterminals
	{
		if(nt_first_done[i]==0)
		{
			// printf("NONTERMINAL: %d\n", i);
			// getFirst(i,&firstsets, &nt_first_done);
			getFirst(i, &nt_first_done);

		}
	}
	// printf("SCOIND\n");
	// for(int i=0;i<tcount+ntcount;i++)
	// {
	// 	printf("%d\t%d\t:\t",i,firstsets[i].symbol);
	// 	ffnode *itr = firstsets[i].list;
	// 	while(itr!=NULL)
	// 	{
	// 		printf("yayay %d\t",itr->terminal);
	// 		itr = itr->next;
	// 	}
	// 	printf("\n");
	// }
	// printf("MARK\n");
	// for(int i=0;i<ntcount;i++)
	// 	printf("%d\t", nt_first_done[i]);
	// printf("\n");
	// return firstsets;
}

int check_eps_first(int sym)
{
	// returns 1 if eps present in first set of symbol
	ffnode *temp = firstsets[sym].list;
	while(temp!=NULL)
	{
		if(temp->terminal==0)
			return 1;
		temp = temp->next;
	}
	return 0;
}
int check_term_list(int term, ffnode *list)
{
	ffnode *temp = list;
	while(temp!=NULL)
	{
		if(temp->terminal==term)
			return 1;
		temp = temp->next;
	}
	return 0;
}
int check_sym_rule(int nt,rhsnode *rule)
{
	rhsnode *itr = rule;
	while(itr!=NULL)
	{
		if(itr->isT==0 && itr->symbol==nt)
			return 1;
		itr = itr->next;
	}
	return 0;
}

/*void getFollow(int nt, int **nt_follow_done)
{
	// printf("FOLLOW: %d\n", nt);
	followsets[nt].symbol = nt;
	for(int i=0;i<NUM_RULES;i++)
	{
		// printf("RULE NUM: %d\n",i);
		if(check_sym_rule(nt,grammar[i].rhs)==1)
		{	
			// printf("INSIDE!\n");
			rhsnode *itr = grammar[i].rhs;
			while(itr!=NULL)
			{
				// while(itr!=NULL || (!(itr->isT==0 && itr->symbol==nt)))
				// 	itr = itr->next;
				if(itr->isT==0 && itr->symbol==nt)
				{
					int flag = 0;
					rhsnode *next;
					if(itr->isT==0)
					{
						int itr_sym = itr->symbol;	
						next = itr->next;

						if(next != NULL)
						{
							if(next->isT==-1)
								printf("CAUTION!!\n");
							int next_sym = next->symbol;
							if(next->isT==0)
								next_sym += tcount;
							if(check_eps_first(next->symbol)==0){
								// printf("follow: %d - first of %d",itr_sym,next_sym);
								insertnodes(&followsets[itr_sym].list,firstsets[next_sym].list);
							}
							else
								flag = 1;
						}
						else
							flag = 1;
						if(flag==1)
						{
							if((*nt_follow_done)[grammar[i].nonterm]==0){
								// printf("follow: %d - follow  of %d",itr_sym,grammar[i].nonterm);
								getFollow(grammar[i].nonterm,nt_follow_done);
							}
							insertnodes(&followsets[itr_sym].list,followsets[grammar[i].nonterm].list);
						}
					}
				}
				itr = itr->next;
			}
		}
	}
	(*nt_follow_done)[nt] = 1;

}*/
void compute_FollowSets()
{
	followsets = (ffheader *)calloc((ntcount),sizeof(ffheader));
	int *nt_follow_done = (int *)calloc(ntcount,sizeof(int));
	ffnode *eoi = (ffnode *)malloc(sizeof(ffnode));
	eoi->terminal = -1; 	// special marker for end of input
	eoi->next = NULL;
	// add end of input marker to 1st nonterminal -> start symbol
	// int i = 0;
	followsets[0].symbol = 0;
	followsets[0].list = eoi;
	for(int i=1;i<ntcount;i++)
	{
		followsets[i].symbol = i;
		followsets[i].list = NULL;
	}
	int pmark = 0;
	int cmark = 1;
	// ffnode *temp = followsets[0].list;
	// printf("follow 0: %d\n", temp->terminal);
	/*for(int i=0;i<ntcount;i++)	// iterate over all nonterminals
	{
		if(nt_follow_done[i]==0)
		{
			// printf("NONTERMINAL: %d\n", i);
			// getFirst(i,&firstsets, &nt_first_done);
			getFollow(i, &nt_follow_done);

		}
	}*/
	int eps_flag;
	while(cmark-pmark>0)
	{
		pmark = cmark;
		cmark = 0;
		for(int i=0;i<NUM_RULES;i++)
		{
			// printf("RULE NUM: %d\n",i);
			rhsnode *itr = grammar[i].rhs;
			// eps_flag = 0;
			while(itr!=NULL)
			{
				if(itr->isT==0)
				{
					int itr_sym = itr->symbol;
					rhsnode* next = itr->next;
					eps_flag = 1;
					while(next!=NULL)
					{
						int next_sym = next->symbol;
						if(next->isT==-1)
							printf("CAUTION!!\n");
						else if(next->isT==0)
							next_sym += tcount;
						// if(check_eps_first(next_sym)==0)
							// if(firstsets[next_sym].list==NULL)
						// printf("INLCUDE FIRST %d %d\n",itr_sym,next_sym);
						eps_flag = insertnodes(&followsets[itr_sym].list,firstsets[next_sym].list);
						// if(eps_flag==0)	
						if(check_eps_first(next_sym)==0)
							break;
						// else
						// 	flag = 1;
						next = next->next;
					}
					if(eps_flag==1){
						// printf("INLCUDE FOLLOW %d %d\n",itr_sym,grammar[i].nonterm);
						insertnodes(&followsets[itr_sym].list,followsets[grammar[i].nonterm].list);				
					}
				}
				itr = itr->next;
			}
		}
		for(int i=0;i<ntcount;i++)
		{
			ffnode *itr = followsets[i].list;
			while(itr!=NULL)
			{
				cmark++;
				itr = itr->next;
			}
		}
	}
}

/*void createParseTable()
{
	parsetable = (int **)malloc(ntcount*sizeof(int *));
	for(int i=0;i<ntcount;i++)
		parsetable[i] = (int *)malloc((tcount+1)*sizeof(int));
	for(int i=0;i<ntcount;i++)
		for(int j=0;j<tcount+1;j++)
			parsetable[i][j]=-1;
	// for(int i=0;i<ntcount;i++)
	// 	for(int j=0;j<tcount;j++)
	// 	{
	// 		for(int k=0;k<NUM_RULES;k++)
	// 		{
	// 			if(grammar[i].nonterm==i)
	// 			{
	// 				// rhsnode *temp = grammar[i].rhs;
	// 				// int i_temp = temp->symbol;
	// 				// if(temp->isT==0)
	// 					// i_temp += tcount;
	// 				if(check_term_list(j,firstsets[i_temp].list)==1 || (check_eps_first(i_temp)==1 && check_term_list(j,followsets[i].list)==1))
	// 					if(parsetable[i][j]==-2)
	// 						parsetable[i][j] = k;
	// 					else
	// 						printf("Overwriting: grammar incorrect %d %d %d new:%d!\n",i,j,parsetable[i][j],k);
	// 			}
	// 		}
	// 	}
	// 	// j=-1;
	for(int i=0;i<NUM_RULES;i++)
	{
		int eps_flag;
		rhsnode *itr = grammar[i].rhs;
		ffnode *first = NULL;
		while(itr!=NULL)
		{
			if(itr->isT!=0)	
			{
				insertnodes(&first,firstsets[itr->symbol].list);
				eps_flag = 0;
				break;
			}
			else
			{
				eps_flag = insertnodes(&first,firstsets[tcount + itr->symbol].list);
				if (eps_flag==0)
					break;
			}
			itr = itr->next;
		}
		if(eps_flag==1)
		{
			ffnode *eps = (ffnode *)malloc(sizeof(ffnode));
			eps->terminal = 0;
			eps->next = NULL;
			// printf("Insert eps: %d %d\n",i_nt,itr->symbol);
			insertnodes(&first,eps);	// add eps to first set of nonterminal	
			// printf("damn:%d\n", firstsets[0]->list->terminal);
			eps_flag = 0;
		}
		// PRINT
		// ffnode *temp = first;
		// printf("RULE %d:\t", i+1);
		// while(temp!=NULL)
		// {
		// 	printf("%d\t", temp->terminal);
		// 	temp = temp->next;
		// }
		// printf("\n");
		ffnode *temp = first;
		while(temp!=NULL)
		{
			if(temp->terminal!=0)
				parsetable[grammar[i].nonterm][temp->terminal] = i;
			else
			{
				ffnode *temp1 = followsets[grammar[i].nonterm].list;
				while(temp1!=NULL)
				{
					if(temp1->terminal!=-1)
					{
						if(parsetable[grammar[i].nonterm][temp1->terminal]==-1)
							parsetable[grammar[i].nonterm][temp1->terminal] = i;
						else
							printf("CAUTION: Overwriting\n");
					}
					else
					{
						if(parsetable[grammar[i].nonterm][tcount]==-1)
							parsetable[grammar[i].nonterm][tcount] = i;
						else
							printf("CAUTION: Overwriting\n");
					}
					temp1= temp1->next;
				}
			}
			temp = temp->next;
		}
	}
}*/
void createParseTable()
{
	parsetable = (int **)malloc(ntcount*sizeof(int *));
	for(int i=0;i<ntcount;i++)
		parsetable[i] = (int *)malloc((tcount+1)*sizeof(int));
	for(int i=0;i<ntcount;i++)
		for(int j=0;j<tcount+1;j++)
			parsetable[i][j]=-1;
	int eps_flag;
	rhsnode *itr = NULL;
	ffnode *temp = NULL;
	for(int i=0;i<NUM_RULES;i++)
	{
		itr = grammar[i].rhs;
		int lhs = grammar[i].nonterm;
		eps_flag = 1;
		while(itr!=NULL)
		{
			if(itr->isT==1)
			{
				temp = firstsets[itr->symbol].list;
				while(temp!=NULL)
				{
					parsetable[lhs][temp->terminal] = i;
					temp = temp->next;
				}
				eps_flag = 0;
				break;
				// parsetable[lhs][]
			}
			else if(itr->isT==0)
			{
				temp = firstsets[itr->symbol + tcount].list;
				while(temp!=NULL)	// do we mark EPSILON when firstset contains EPSILON?
				{
					parsetable[lhs][temp->terminal] = i;
					temp = temp->next;
				}
				if(check_eps_first(itr->symbol + tcount)==0)
				{
					eps_flag = 0;
					break;
				}
			}
			itr = itr->next;
		}
		if(eps_flag == 1)
		{
			temp = followsets[lhs].list;
			while(temp!=NULL)
			{
				parsetable[lhs][temp->terminal] = i;
				temp = temp->next;
			}
			if(check_term_list(-1,followsets[lhs].list)==1)
				parsetable[lhs][tcount] = i;
		}
	}
}
void parseInputSourceCode(char *testcaseFile)
{
	buffers = createBuffers(2,BUF_SIZE);
	FILE *fp = fopen(testcaseFile,"r");
	peek.buf = 0;
	peek.index = 0;
	linenum = 1;
	fp = getStream(fp,buffers[0],BUF_SIZE);
	tokenInfo token;
	do
	{
		token = getNextToken(fp,2,BUF_SIZE);
		printf("%d %s %d\n",token.id,token.lexeme,token.line); 
	}while(token.id!=eof);
}
int main()
{
	char filename[] = "grammar.txt";
	loadGrammar(filename);
	for(int i = 0; i < ntcount;i++)
		printf("%s\t",nonterms[i]);
	printf("%d\n",ntcount);
	printf("NONTERMINALS\n");
	for(int i=0;i<ntcount;i++)
		printf("%d\t%s\n",i,nonterms[i]);
	printf("TERMINALS\n");
	for(int i=0;i<tcount;i++)
		printf("%d\t%s\n",i,terms[i]);
	for(int i=0;i<NUM_RULES;i++)
	{
		printf("RULE %d:\t%d\n", i+1, grammar[i].nonterm);
		rhsnode *itr = grammar[i].rhs;
		while(itr!=NULL)
		{
			printf("Node:\t%d\t%d\n",itr->isT,itr->symbol);
			itr = itr->next;
		}
		printf("\n");
	}
	// ffnode *ptr = (ffnode *)malloc(sizeof(ffnode));
	// ptr->terminal = 1;
	// ptr->next = (ffnode *)malloc(sizeof(ffnode));
	// ptr->next->terminal = 2;
	// ptr->next->next = (ffnode *)malloc(sizeof(ffnode));
	// ptr->next->next->terminal = 3;
	// ptr->next->next->next = NULL;
	// // ffnode *ptr;
	// ffnode *ptr1 = (ffnode *)malloc(sizeof(ffnode));
	// ptr1->terminal = 4;
	// ptr1->next = (ffnode *)malloc(sizeof(ffnode));
	// ptr1->next->terminal = 0;
	// ptr1->next->next = (ffnode *)malloc(sizeof(ffnode));
	// ptr1->next->next->terminal = 6;
	// ptr1->next->next->next = NULL;
	// insertnodes(&ptr,ptr1);
	// ffnode *temp = ptr;
	// printf("HELLO!\n");
	// while(temp!=NULL)
	// {
	// 	printf("%d\t", temp->terminal);
	// 	temp = temp->next;
	// }
	// printf("\n");

	compute_FirstSets();
	printf("FIRST SETS\n");
	for(int i=0;i<tcount+ntcount;i++)
	{
		printf("%d\t%d\t:\t",i,firstsets[i].symbol);
		ffnode *itr = firstsets[i].list;
		while(itr!=NULL)
		{
			printf("%d\t",itr->terminal);
			itr = itr->next;
		}
		printf("\n");
	}
	// printf("%d %d",tcount,ntcount);
	compute_FollowSets();
	printf("FOLLOW SETS\n");
	for(int i=0;i<ntcount;i++)
	{
		printf("%d:\t",followsets[i].symbol);
		ffnode *itr = followsets[i].list;
		while(itr!=NULL)
		{
			printf("%d\t",itr->terminal);
			itr = itr->next;
		}
		printf("\n");
	}
	createParseTable();
	printf("SYMBOL TABLE\n");
	for(int i=0;i<ntcount;i++)
	{
		printf("%d\n", i);
		for(int j=0;j<tcount+1;j++)
			printf("%d\t", parsetable[i][j]);
		printf("\n\n\n");
	}
	char file[] = "hello.txt";
	parseInputSourceCode(file);

	// printf("check term list: %d\n", check_term_list(1,followsets[1].list));
	return 0;
}
#endif