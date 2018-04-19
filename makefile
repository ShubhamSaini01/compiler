compile: stack.o lexer.o parser.o ast.o symboltable.o driver.o
	gcc -o stage1exe stack.o parser.o lexer.o ast.o symboltable.o driver.o

driver.o: 
	gcc -c -g driver.c
parser.o: 
	gcc -c -g parser.c
lexer.o: 
	gcc -c -g lexer.c
stack.o:
	gcc -c -g stack.c
ast.o:
	gcc -c -g ast.c
symboltable.o:
	gcc -c -g symboltable.c

clean: 
	rm *.o
