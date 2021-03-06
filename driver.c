// #include "parser.h"
#include "symboltable.h"

int main(int argc, char **argv)
{
	ntcount = 0;
	tcount = 0;
	char filename[] = "grammar_gaind.txt";
	loadGrammar(filename);
	// for(int i = 0; i < ntcount;i++)
	// 	printf("%s\t",nonterms[i]);
	// printf("Terms: %d\tNonterms: %d\n",tcount,ntcount);
	// printf("NONTERMINALS\n");
	// for(int i=0;i<ntcount;i++)
	// 	printf("%d\t%s\n",i,nonterms[i]);
	// printf("TERMINALS\n");
	// for(int i=0;i<tcount;i++)
	// 	printf("%d\t%s\n",i,terms[i]);
	// for(int i=0;i<NUM_RULES;i++)
	// {
	// 	printf("RULE %d:\t%d\n", i+1, grammar[i].nonterm);
	// 	rhsnode *itr = grammar[i].rhs;
	// 	while(itr!=NULL)
	// 	{
	// 		printf("Node:\t%d\t%d\n",itr->isT,itr->symbol);
	// 		itr = itr->next;
	// 	}
	// 	printf("\n");
	// }
	compute_FirstSets();
	/*// print first sets
	printf("FIRST SETS\n");
	for(int i=0;i<tcount+ntcount;i++)
	{
		if(i<tcount)
			printf("%d\t%s\t:\t",i,terms[firstsets[i].symbol]);
		else
			printf("%d\t%s\t:\t",i,nonterms[firstsets[i].symbol]);
		ffnode *itr = firstsets[i].list;
		while(itr!=NULL)
		{
			printf("%s\t",terms[itr->terminal]);
			itr = itr->next;
		}
		printf("\n");
	}*/
	compute_FollowSets();
	// printf("FOLLOW SETS\n");
	// for(int i=0;i<ntcount;i++)
	// {
	// 	printf("%d:\t",followsets[i].symbol);
	// 	ffnode *itr = followsets[i].list;
	// 	while(itr!=NULL)
	// 	{
	// 		printf("%d\t",itr->terminal);
	// 		itr = itr->next;
	// 	}
	// 	printf("\n");
	// }
	createParseTable();
	/*// print parse table
	printf("PARSE TABLE\n");
	for(int i=0;i<ntcount;i++)
	{
		printf("%s\n", nonterms[i]);
		for(int j=0;j<tcount+1;j++)
			if (parsetable[i][j]>-1)
				printf("%s : %d\n", terms[j], parsetable[i][j]+1);
		printf("\n\n");
	}*/
	// char file[] = "testcase3.txt";
	PTNode *root;
	// printParseTree(root);
	// printf("%s\n",file);
	// printf("check term list: %d\n", check_term_list(1,followsets[1].list));
	

	int choice;
	printf("\t\t********************\t\t\n");
	printf("(a) FIRST AND FOLLOW SETS GENERATED\n(b)LEXICAL and SYNTAX analysis\n(c)Runs on all testcases\n(d)Implemented twin buffers\n");
	printf("Enter choice:\n");
	printf("1. Generate comment free code\n");
	printf("2. Print token list generated by lexer\n");
	printf("3. Parse source code\n");
	printf("4. Parse source code and write tree to file\n");
	printf("5. Create AST\n");
	scanf("%d",&choice);
	switch(choice)
	{
		case 1:	break;
		case 2:	buffers = createBuffers(2,BUF_SIZE);
				FILE *fp = fopen(argv[1],"r");
				peek.buf = 0;
				peek.index = 0;
				linenum = 1;
				fp = getStream(fp,buffers[0],BUF_SIZE);
				tokenInfo *token;
				do
				{
					token = getNextToken(fp,2,BUF_SIZE);

					printf("%s\t\t%s\t\t%d\n",terms[token->id],token->lexeme,token->line);
				}while(token->id!=eof);
				break;
		case 3:	root = parseInputSourceCode(argv[1]);
				printParseTree(root);
				break;
		case 4:	root = parseInputSourceCode(argv[1]);
				FILE *treeFile = fopen(argv[2],"w+");
				writeParseTree(root,treeFile);
				fclose(treeFile);
				break;
		case 5: root = parseInputSourceCode(argv[1]);
				astNode *astRoot = ast_generate(root);
				printAST(astRoot);
				SymbolTable *st_main = createSymbolTable(astRoot/*->children->siblings->siblings->siblings*/, NULL);
				printSymbolTable_Node(st_main);
				break;
		default: printf("INVALID CHOICE!\n");
	}
	
	return 0;
}