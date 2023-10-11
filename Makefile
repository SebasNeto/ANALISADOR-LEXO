etapa1: lex.yy.o parser.tab.o main.o
	gcc -o etapa1 lex.yy.o parser.tab.o main.o

main.o: main.c
	gcc -c main.c

parser.tab.c parser.tab.h: parser.y
	bison -d parser.y

lex.yy.c lex.yy.h: scanner.l parser.tab.h
	flex --header-file=lex.yy.h scanner.l

lex.yy.o: lex.yy.c
	gcc -c lex.yy.c

parser.tab.o: parser.tab.c
	gcc -c parser.tab.c

clean:
	rm -f *.o lex.yy.c etapa1 parser.tab.c parser.tab.h lex.yy.h

