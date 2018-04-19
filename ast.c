#include "ast.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

astNode *create_astNode(PTNode * ptnode)
{
	astNode *node = (astNode *)malloc(sizeof(astNode));
	node->isT = ptnode->isT;
	node->symbol = ptnode->symbol;
	node->token = ptnode->token;
	node->parent = NULL;
	node->children = NULL;
	node->siblings = NULL;
	return node;
}

astNode *joinroot_astNodes(astNode *root, astNode *arr[], int len)
{
	int j = 0;
	while(j<len && arr[j]==NULL)
		j++;
	// printf("JOINNODES FIRST NON NULL: j = %d\n",j);
	root->children = arr[j];
	astNode *itr;
	for(int i = j; i < len; i++)
	{
		if(i==j)
			itr = arr[i];
		else if(arr[i]!=NULL)
		{
			itr->siblings = arr[i];
			itr = itr->siblings;	// itr = arr[i]
		}
		else
			continue;
		while(itr->siblings!=NULL)
		{
			itr->parent = root;
			itr = itr->siblings;
		}
		itr->parent = root;
	}
	return root;
}

astNode *join_astNodes(astNode *arr[], int len)
{
	int j = 0;
	while(j<len && arr[j]==NULL)
		j++;
	// printf("JOINNODES w/o ROOT FIRST NON NULL: j = %d\n",j);
	astNode *first = arr[j];
	astNode *itr;
	for(int i=j; i<len; i++)
	{
		if(i==j)
			itr = arr[i];
		else if(arr[i]!=NULL)
		{
			itr->siblings = arr[i];
			itr = itr->siblings;
		}
		else 
			continue;
		while(itr->siblings!=NULL)
			itr = itr->siblings;
	}
	return first;
}
astNode *ast_generate(PTNode *root)
{
	// statusNode(root);
	if(root==NULL)
		return NULL;
	if(root->children==NULL)	// Leaf Node
		return NULL;			// DO: replace this
	int rule = root->children->rule + 1;
	
	if(rule==1)		// mainFunction
	{
		astNode *mainFn = create_astNode(root);
		// printASTNode(node);
		astNode **arr = (astNode **)malloc(1*sizeof(astNode *));
		arr[0] = ast_generate(root->children->siblings->siblings->siblings);
		return joinroot_astNodes(mainFn, arr, 1);
	}
	else if(rule==2 || rule==33 || rule==38 || rule==81)
	{
		/*2.	<stmtsAndFunctionDefs> ===> <stmtOrFunctionDef> <moreStmtAndFunctionDefs>
		33.	<inputParameterList> ===> <var> <listVar>
		38. <otherStmts> ===> <stmt> <otherStmts>
		81. <rows> ===> <row> <more_rows>*/
		astNode **arr = (astNode **)malloc(2*sizeof(astNode *));
		arr[0] = ast_generate(root->children);
		arr[1] = ast_generate(root->children->siblings);
		return join_astNodes(arr,2);
	}
	else if(rule==4 || rule==20 || rule==34 || rule==37 || rule==39 || rule==44 || rule==47 ||rule==68 || rule==79 || rule==83 || rule==86 || rule==88)
	{
		/*4. <moreStmtAndFunctionDefs> ===> EPSILON
		20. <remainingList> ===> EPSILON
		34. <inputParameterList> ===> EPSILON
		37. <add_else> ===> EPSILON
		39. <otherStmts> ===> EPSILON
		44. <x> ===> EPSILON
		47.	<y> ===> EPSILON
		68. <matrix_may_be> ===> EPSILON
		79. <more_ids> ===> EPSILON
		83. <more_rows> ===> EPSILON
		86. <more_nums> ===> EPSILON
		88. <listVar> ==> EPSILON*/
		return NULL;
	}
	else if(rule==3 || (rule>=5 && rule<=12) || rule==22 || rule==23 || (rule>=28 && rule<=31) || rule==49 || rule==64 || rule==65)	
	{
		/*3.	<more_stmtsAndFunctionDefs> ===> <stmtsAndFunctionDefs>
		5.	<stmtOrFunctionDef> ===> <stmt>
		6.	<stmtOrFunctionDef> ===> <functionDef>
		7.	<stmt> ===> <declarationStmt> 
		8.	<stmt> ===> <assignmentStmt> 
		9.	<stmt> ===> <conditionalStmt> 
		10.	<stmt> ===> <ioStmt> 
		11.	<stmt> ===> <funCallStmt>
		12.	<funCallStmt> ===> <functionCall> SEMICOLON
		22.	<assignmentStmt> ===> <assignmentStmt1>
		23. <assignmentStmt> ===> <assignmentStmt2>
		28. <rightHandSide1> ===> <arithmeticExpression>
		29. <rightHandSide1> ===> <size_exp>
		30. <rightHandSide2> ===> <functionCall>
		31. <rightHandSide2> ===> <size_exp>
		49. <arithmeticExpression2> ===> <var>
		64.	<var> ===> <matrix_actual>
		65.	<var> ===> <functionCall>*/
		return ast_generate(root->children);
	}
	else if(rule==13)	// <functionDef> ===> FUNCTION SQO <parameter_list> SQC ASSIGNOP FUNID SQO <parameter_list> SQC <stmtsAndFunctionDefs> END SEMICOLON
	{
		astNode **arr = (astNode **)malloc(4*sizeof(astNode *));
		astNode *funcDef = create_astNode(root);
		astNode **arr1 = (astNode **)malloc(1*sizeof(astNode *));
		// temp = <parameter_list1>
		PTNode *temp = root->children->siblings->siblings;	
		astNode *paramlist1 = create_astNode(temp);

		arr1[0] = ast_generate(temp);
		arr[0] = joinroot_astNodes(paramlist1, arr1, 1);
		// temp = FUNID
		temp = temp->siblings->siblings->siblings;	
		arr[1] = create_astNode(temp);

		// temp = <parameter_list2>
		temp = temp->siblings->siblings;
		astNode *paramlist2 = create_astNode(temp);
		arr1[0] = NULL;
		arr1[0] = ast_generate(temp);
		arr[2] = joinroot_astNodes(paramlist2, arr1, 1);

		// temp = <stmtsAndFunctionDefs>
		temp = temp->siblings->siblings;
		arr[3] = ast_generate(temp);
		

		return joinroot_astNodes(funcDef, arr, 4);
	}
	else if(rule==14)	// <parameter_list> ===> <type> ID <remainingList>
	{
		astNode *param = create_astNode(root);
		astNode **arr = (astNode **)malloc(2*sizeof(astNode *));
		PTNode *temp = root->children;	// <type>
		arr[0] = ast_generate(temp);
		temp = temp->siblings;	// ID 
		arr[1] = create_astNode(temp);
		astNode **arr1 = (astNode **)malloc(2*sizeof(astNode *));
		arr1[0] = joinroot_astNodes(param,arr,2);
		temp = temp->siblings;	// <remainingList>
		arr1[1] = ast_generate(temp);
		return join_astNodes(arr1,2);
	}
	else if((rule>=15 && rule<=18) || rule==26 || (rule>=50 && rule<=53) || rule==58 || rule==59 || rule==61 || rule==62 || rule==63 || (rule>=69 && rule<=76))
	{
		/*15. <type> ===> INT
		16.	<type> ===> REAL
		17.	<type> ===> STRING
		18.	<type> ===> MATRIX
		26.	<leftHandSide1> ===> ID
		58.	<constrainedVars> ===> NUM
		59.	<constrainedVars> ===> RNUM
		61. <var> ===> NUM
		62. <var> ===> RNUM
		63. <var> ===> STR
		69. <logicalOp1> ===> AND
		70. <logicalOp1> ===> OR
		71. <relationalOp> ===> LT 
		72. <relationalOp> ===> LE
		73. <relationalOp> ===> EQ
		74. <relationalOp> ===> GT
		75. <relationalOp> ===> GE
		76. <relationalOp> ===> NE
		50. <operator1> ===> PLUS
		51.	<operator1> ===> MINUS
		52.	<operator2> ===> MUL
		53.	<operator2> ===> DIV*/
		return create_astNode(root->children);
	}
	else if(rule==19 || rule==27 || rule==48 || rule==78 || rule==82 || rule==87)
	{
		/*19.	<remainingList> ===> COMMA <parameter_list>
		27. <leftHandSide2> ===> SQO <var_list> SQC
		48. <arithmeticExpression2> ===> OP <arithmeticExpression> CL
		78. <more_ids> ===> COMMA <var_list>
		82. <more_rows> ===> SEMICOLON <rows>  
		87. <listVar> ==> COMMA <inputParameterList>*/
		return ast_generate(root->children->siblings);
	}
	else if(rule==85)	// <more_nums> ===> COMMA NUM <more_nums>
	{
		astNode **arr = (astNode **)malloc(2*sizeof(astNode *));
		arr[0] = create_astNode(root->children->siblings);
		arr[1] = ast_generate(root->children->siblings->siblings);
		return join_astNodes(arr, 2);
	}
	else if(rule == 80)	// <matrix_actual> ===> SQO <rows> SQC
	{
		astNode *matrix = create_astNode(root);
		astNode **arr = (astNode **)malloc(1*sizeof(astNode *));
		arr[0] = ast_generate(root->children->siblings);
		return joinroot_astNodes(matrix, arr, 1);
	}
	else if(rule==21)	// <declarationStmt> ===> <type> <var_list> SEMICOLON
	{
		astNode *declStmt = create_astNode(root);
		astNode **arr = (astNode **)malloc(2*sizeof(astNode *));
		arr[0] = ast_generate(root->children);
		arr[1] = ast_generate(root->children->siblings);
		return joinroot_astNodes(declStmt, arr, 2);
	}
	else if(rule==24 || rule==25)
	{
		/*24.	<assignmentStmt1> ===> <leftHandSide1> ASSIGNOP <rightHandSide1> SEMICOLON
		25.	<assignmentStmt2> ===> <leftHandSide2> ASSIGNOP <rightHandSide2> SEMICOLON*/
		astNode *assignStmt = create_astNode(root);
		astNode **arr = (astNode **)malloc(2*sizeof(astNode *));
		arr[0] = ast_generate(root->children);
		arr[1] = ast_generate(root->children->siblings->siblings);
		return joinroot_astNodes(assignStmt, arr, 2);
	}
	else if(rule==32)	// <functionCall> ===> FUNID OP <inputParameterList> CL
	{
		astNode *fnCall = create_astNode(root);
		astNode **arr = (astNode **)malloc(2*sizeof(astNode *));
		arr[0] = create_astNode(root->children);
		arr[1] = ast_generate(root->children->siblings->siblings);
		return joinroot_astNodes(fnCall, arr, 2);
	}
	else if(rule==35)	// <conditionalStmt> ===> IF OP <booleanExpression> CL <stmt> <otherStmts> <add_else> ENDIF SEMICOLON
	{
		astNode *condStmt = create_astNode(root);
		astNode **arr = (astNode **)malloc(4*sizeof(astNode *));
		PTNode *temp = root->children->siblings->siblings;	// = <booleanExpression
		arr[0] = ast_generate(temp);
		temp = temp->siblings->siblings;	// = <stmt>
		arr[1] = ast_generate(temp);
		temp = temp->siblings;	// = <otherStmts>
		arr[2] = ast_generate(temp);
		temp = temp->siblings;	// = <add_else>
		arr[3] = ast_generate(temp);
		return joinroot_astNodes(condStmt, arr, 4);
	}
	else if(rule==36)	// <add_else> ===> ELSE <stmt> <otherStmts>
	{
		astNode *addelse = create_astNode(root);
		astNode **arr = (astNode **)malloc(2*sizeof(astNode *));
		arr[0] = ast_generate(root->children->siblings);
		arr[1] = ast_generate(root->children->siblings->siblings);
		return joinroot_astNodes(addelse, arr, 2);
	}
	else if(rule==40 || rule==41)
	{
		/*40. <ioStmt> ===> READ OP ID CL SEMICOLON
		41. <ioStmt> ===> PRINT OP ID CL SEMICOLON*/
		astNode *ioStmt = create_astNode(root->children);
		astNode **arr = (astNode **)malloc(1*sizeof(astNode *));
		arr[0] = create_astNode(root->children->siblings->siblings);
		return joinroot_astNodes(ioStmt, arr, 1);
	}
	else if(rule==54)	// <booleanExpression> ===> OP <booleanExpression> CL <logicalOp1> OP <booleanExpression> CL
	{
		astNode *boolexp = create_astNode(root);
		astNode **arr = (astNode **)malloc(3*sizeof(astNode *));
		PTNode *temp = root->children->siblings;	// = <booleanExpression1>
		arr[0] = ast_generate(temp);
		temp = temp->siblings->siblings;	// = <logicalOp1>
		arr[1] = create_astNode(temp);
		temp = temp->siblings->siblings;	// = <booleanExpression2>
		arr[2] = ast_generate(temp);
		return joinroot_astNodes(boolexp, arr, 3);
	}
	else if(rule==55)	// <booleanExpression> ===> NOT OP <booleanExpression> CL
	{
		astNode *notboolexp = create_astNode(root->children);
		astNode **arr = (astNode **)malloc(1*sizeof(astNode *));
		arr[0] = ast_generate(root->children->siblings->siblings);
		return joinroot_astNodes(notboolexp, arr, 1);
	}
	else if(rule==56)	// <booleanExpression> ===> <constrainedVars> <relationalOp> <constrainedVars>
	{
		astNode *boolexp = create_astNode(root);	// DO: change this probably. conflict with rule 54.
		astNode **arr = (astNode **)malloc(3*sizeof(astNode *));
		PTNode *temp = root->children;	// = <constrainedVars>
		arr[0] = ast_generate(temp);
		temp = temp->siblings;	// = <relationalOp>
		arr[1] = ast_generate(temp);
		temp = temp->siblings;	// = <constrainedVars1>
		arr[2] = ast_generate(temp);
		return joinroot_astNodes(boolexp, arr, 3);
	}
	else if(rule==84)	// 84. <row> ===> NUM <more_nums>
	{
		astNode *constrainedVars = create_astNode(root);
		astNode **arr = (astNode **)malloc(2*sizeof(astNode *));
		arr[0] = create_astNode(root->children);
		arr[1] = ast_generate(root->children->siblings);
		return joinroot_astNodes(constrainedVars, arr, 2);
	}
	else if(rule==66)	// <size_exp> ===> SIZE ID
	{
		astNode *size = create_astNode(root);
		astNode **arr = (astNode **)malloc(1*sizeof(astNode *));
		arr[0] = create_astNode(root->children->siblings);
		return joinroot_astNodes(size, arr, 1);
	}
	else if(rule==67)	// <matrix_may_be> ===> SQO NUM COMMA NUM SQC
	{
		astNode *matrix = create_astNode(root);
		astNode **arr = (astNode **)malloc(2*sizeof(astNode *));
		PTNode *temp = root->children->siblings;	// = NUM
		arr[0] = create_astNode(temp);
		temp = temp->siblings->siblings;	// = NUM1
		arr[1] = create_astNode(temp);
		return joinroot_astNodes(matrix, arr, 2);
	}
	else if(rule==57 || rule==60 || rule==77)
	{
		/*57.	<constrainedVars> ===> ID <matrix_may_be>
		60. <var> ===> ID <matrix_may_be>
		77. <var_list> ===> ID <more_ids>*/
		astNode **arr = (astNode **)malloc(2*sizeof(astNode *));
		arr[0] = create_astNode(root->children);
		arr[1] = ast_generate(root->children->siblings);
		return join_astNodes(arr, 2);
	}
	else if(rule==42 || rule==45)
	{
		/*42.	<arithmeticExpression> ===> <arithmeticExpression1> <x>
		45.	<arithmeticExpression1> ===> <arithmeticExpression2> <y>*/
		astNode *arithexp12 = ast_generate(root->children);
		astNode *xy = ast_generate(root->children->siblings);
		if(xy!=NULL)
		{
			astNode **arr1 = (astNode **)malloc(2*sizeof(astNode *));
			arr1[0] = arithexp12;
			arr1[1] = xy->children;
			return joinroot_astNodes(xy, arr1, 2);
		}
		else
			return arithexp12;

	}
	else if(rule==43 || rule==46)
	{
		/*43.	<x> ===> <operator1> <arithmeticExpression1> <x>
		46.	<y> ===> <operator2> <arithmeticExpression2> <y>*/
		astNode *operator = ast_generate(root->children);	// astroot = operator
		astNode **arr = (astNode **)malloc(1*sizeof(astNode *));
		astNode *arithexp12 = ast_generate(root->children->siblings);
		astNode *xy = ast_generate(root->children->siblings->siblings);
		if(xy!=NULL)
		{
			astNode **arr1 = (astNode **)malloc(2*sizeof(astNode *));
			arr1[0] = arithexp12;
			arr1[1] = xy->children;
			arr[0] = joinroot_astNodes(xy, arr1, 2);
		}
		else
			arr[0] = arithexp12;
		return joinroot_astNodes(operator, arr, 1);
	}
	/*else if(rule==81)	// <rows> ===> <row> <more_rows>
	{
		astNode **arr = (astNode **)malloc(2*sizeof(astNode *));
		astNode *row = create_astNode(root->children->isT, root->children->symbol);
		astNode **arr1 = (astNode **)malloc(1*sizeof(astNode *));
		arr1[0] = ast_generate(root->children);
		arr[0] = joinroot_astNodes(row, arr1, 1);
		arr[1] = ast_generate(root->children->siblings);
		return join_astNodes(arr, 2);
	}*/
	else
	{
		if(root->isT!=0)
			printf("Make AST for %s (%d)\n", terms[root->symbol], rule);
		else
			printf("Make AST for %s (%d)\n", nonterms[root->symbol], rule);
		return NULL;
	}
}

void statusNode(PTNode *root)
{
	if(root==NULL)
		printf("NULL\n");
	if(root->isT!=0)
		printf("Generate AST Node: %s\n",terms[root->symbol]);
	else
		printf("Generate AST Node: %s\n",nonterms[root->symbol]);
}

inline void printASTNode(astNode *root)
{
	if(root->isT!=0)
	{
		if(root->token)
			printf("AST Node: %d %s(%d) %s\n",root->isT,terms[root->symbol], root->symbol, root->token->lexeme);
		else
			printf("AST Node: %d %s(%d) NULL\n",root->isT,terms[root->symbol], root->symbol);
	}
	else
		printf("AST Node: %d %s(%d) NONTERM\n",root->isT,nonterms[root->symbol], root->symbol);
}

void printAST(astNode *root)
{
	if(root->children!=NULL){
		printf("down\n");
		printAST(root->children);
		printf("up\n");
	}
	printASTNode(root);
	root = root->children;
	printf("down\n");
	while(root!=NULL && root->siblings!=NULL)
	{
		printf("->\n");
		printAST(root->siblings);
		root = root->siblings;
	}
	printf("up\n");
	return;
}
